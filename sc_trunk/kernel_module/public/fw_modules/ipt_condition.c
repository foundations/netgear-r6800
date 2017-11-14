/*-------------------------------------------*\
|          Netfilter Condition Module         |
|                                             |
|  Description: This module allows firewall   |
|    rules to match using condition variables |
|    stored in /proc files.                   |
|                                             |
|  Author: Stephane Ouellette     2002-10-22  |
|          <ouellettes@videotron.ca>          |
|                                             |
|  History:                                   |
|    2003-02-10  Second version with improved |
|                locking and simplified code. |
|                                             |
|  This software is distributed under the     |
|  terms of the GNU GPL.                      |
\*-------------------------------------------*/

#include<linux/module.h>
#include<linux/proc_fs.h>
#include<linux/spinlock.h>
#include<linux/string.h>
#include<asm/atomic.h>
#include<linux/netfilter_ipv4/ip_tables.h>
//#include<linux/netfilter_ipv4/ipt_condition.h>
#include "ipt_condition.h"
#include <linux/version.h>

#ifndef CONFIG_PROC_FS
#error  "Proc file system support is required for this module"
#endif


MODULE_AUTHOR("Stephane Ouellette <ouellettes@videotron.ca>");
MODULE_DESCRIPTION("Allows rules to match against condition variables");
MODULE_LICENSE("GPL");

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
#include "../../../linux/fs/proc/internal.h"
#endif

struct condition_variable {
	struct condition_variable *next;
	struct proc_dir_entry *status_proc;
	atomic_t refcount;
        int enabled;   /* TRUE == 1, FALSE == 0 */
};


static rwlock_t list_lock;
static struct condition_variable *head = NULL;
static struct proc_dir_entry *proc_net_condition = NULL;


#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static ssize_t ipt_condition_read_info(struct file *file, char __user *user, size_t count, loff_t *f_pos)
{
	int len = 0;
	int i_ret = 0;
	char buffer[32];

	struct proc_dir_entry* entry = PDE(file->f_path.dentry->d_inode);
	struct condition_variable *var =
	    (struct condition_variable *) (entry->data);

	if (*f_pos > 0) {
		len = 0;
	} else {

		buffer[0] = (var->enabled) ? '1' : '0';
		buffer[1] = '\n';
		len = 2;

		i_ret = copy_to_user(user, buffer, len);
		if (i_ret) {
			printk("copy to buffer failed, ret:%d\n", i_ret);
			len = -EFAULT;
			goto err_exit;
		}
		*f_pos += len;
	}
err_exit:
	return len;
}
#else
static int
ipt_condition_read_info(char *buffer, char **start, off_t offset,
			int length, int *eof, void *data)
{
	struct condition_variable *var =
	    (struct condition_variable *) data;

	if (offset == 0) {
		*start = buffer;
		buffer[0] = (var->enabled) ? '1' : '0';
		buffer[1] = '\n';
		return 2;
	}

	*eof = 1;
	return 0;
}
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static ssize_t ipt_condition_write_info(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
	struct proc_dir_entry* entry = PDE(file->f_path.dentry->d_inode);
	struct condition_variable *var =
	    (struct condition_variable *) (entry->data);

	if (length) {
	        /* Match only on the first character */
		switch (buffer[0]) {
		case '0':
			var->enabled = 0;
			break;
		case '1':
			var->enabled = 1;
		}
	}

	return (int) length;
}
#else
static int
ipt_condition_write_info(struct file *file, const char *buffer,
			 unsigned long length, void *data)
{
	struct condition_variable *var =
	    (struct condition_variable *) data;

	if (length) {
	        /* Match only on the first character */
		switch (buffer[0]) {
		case '0':
			var->enabled = 0;
			break;
		case '1':
			var->enabled = 1;
		}
	}

	return (int) length;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
static bool
match(const struct sk_buff *skb,
		       struct xt_action_param *par)
#else
static bool
match(const struct sk_buff *skb,
		      struct xt_match_param *par)
#endif
{
	const struct condition_info *info =
	    (const struct condition_info *) par->matchinfo;
	struct condition_variable *var;
	int condition_status = 0;

	read_lock(&list_lock);

	for (var = head; var; var = var->next) {
		if (strcmp(info->name, var->status_proc->name) == 0) {
			condition_status = var->enabled;
			break;
		}
	}

	read_unlock(&list_lock);

	return condition_status ^ info->invert;
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static const struct file_operations ipt_condition_fops= {
	.read		= ipt_condition_read_info,
	.write		= ipt_condition_write_info,
};
#endif

int
checkentry(const struct xt_mtchk_param *par)
{
	struct condition_info *info = (struct condition_info *) par->matchinfo;
	struct condition_variable *var, *newvar;
    
	//if (match->matchsize != IPT_ALIGN(sizeof(struct condition_info)))
	//	return 0;

	/* The first step is to check if the condition variable already exists. */
	/* Here, a read lock is sufficient because we won't change the list */
	read_lock(&list_lock);

	for (var = head; var; var = var->next) {
		if (strcmp(info->name, var->status_proc->name) == 0) {
			atomic_inc(&var->refcount);
			read_unlock(&list_lock);
			return 0;
		}
	}

	read_unlock(&list_lock);

	/* At this point, we need to allocate a new condition variable */
	newvar = kmalloc(sizeof(struct condition_variable), GFP_KERNEL);

	if (!newvar)
		return -ENOMEM;

	/* Create the condition variable's proc file entry */
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	newvar->status_proc = proc_create(info->name, 0644, proc_net_condition, &ipt_condition_fops);
#else
	newvar->status_proc = create_proc_entry(info->name, 0644, proc_net_condition);
#endif

	if (!newvar->status_proc) {
	  /*
	   * There are two possibilities:
	   *  1- Another condition variable with the same name has been created, which is valid.
	   *  2- There was a memory allocation error.
	   */
		kfree(newvar);
		read_lock(&list_lock);

		for (var = head; var; var = var->next) {
			if (strcmp(info->name, var->status_proc->name) == 0) {
				atomic_inc(&var->refcount);
				read_unlock(&list_lock);
				return 0;
			}
		}

		read_unlock(&list_lock);
		return -ENOMEM;
	}

	atomic_set(&newvar->refcount, 1);
	newvar->enabled = 0;
//	newvar->status_proc->owner = THIS_MODULE;
	newvar->status_proc->data = newvar;
	wmb();
#if LINUX_VERSION_CODE <= KERNEL_VERSION(3,10,13)
	newvar->status_proc->read_proc = ipt_condition_read_info;
	newvar->status_proc->write_proc = ipt_condition_write_info;
#endif

	write_lock(&list_lock);

	newvar->next = head;
	head = newvar;

	write_unlock(&list_lock);

	return 0;
}


static void
destroy(const struct xt_mtdtor_param *par)
{
	struct condition_info *info = (struct condition_info *) par->matchinfo;
	struct condition_variable *var, *prev = NULL;

	//if (match->matchsize != IPT_ALIGN(sizeof(struct condition_info)))
	//	return;

	write_lock(&list_lock);

	for (var = head; var && strcmp(info->name, var->status_proc->name);
	     prev = var, var = var->next);

	if (var && atomic_dec_and_test(&var->refcount)) {
		if (prev)
			prev->next = var->next;
		else
			head = var->next;

		write_unlock(&list_lock);
		remove_proc_entry(var->status_proc->name, proc_net_condition);
		kfree(var);
	} else
		write_unlock(&list_lock);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
static struct xt_match condition_match = {
#else
static struct ipt_match condition_match = {
#endif
	.name = "condition",
	.match = &match,
	.checkentry = &checkentry,
	.destroy = &destroy,
	.me = THIS_MODULE,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	.family         = AF_INET,
	.matchsize      = sizeof(struct condition_info),
#endif
};


static int __init
init(void)
{
	int errorcode;

	rwlock_init(&list_lock);
	proc_net_condition = proc_mkdir("ipt_condition", init_net.proc_net);

	if (proc_net_condition) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	        errorcode = xt_register_match(&condition_match);
#else
	        errorcode = ipt_register_match(&condition_match);
#endif
		if (errorcode)
			remove_proc_entry("ipt_condition", init_net.proc_net);
	} else
		errorcode = -EACCES;

	return errorcode;
}


static void __exit
fini(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	xt_unregister_match(&condition_match);
#else
	ipt_unregister_match(&condition_match);
#endif
	remove_proc_entry("ipt_condition", init_net.proc_net);
}

module_init(init);
module_exit(fini);
