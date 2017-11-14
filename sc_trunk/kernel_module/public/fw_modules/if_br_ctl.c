
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <linux/interrupt.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/spinlock.h>
#include <linux/version.h>

#if 0
#define dbgMsg  printk
#else
#define dbgMsg(...)
#endif
/*This module can bridge WAN port packet to LAN side,
 * and that lan port will directly connect to WAN side.
 * How to:
 * Inssert module,
 * echo WAN=xxx > /proc/net/if_br_ctl
 * echo LAN=xxx > /proc/net/if_br_ctl
 * Remeber remove the lan interface in lan-bridge.
 * HOOK in dev.c skb_rcv
 */
#define MULTI_GROUP_SUPPORT

extern int (*if_bridge_hook) (struct sk_buff *skb);

struct single_if_info {
	char name[16];
#ifdef MULTI_GROUP_SUPPORT
//#warning "OK, We use MULTI_GROUP_SUPPORT in if_br_ctl!"
	int group;
#endif
	int wan_if_flag;
	struct net_device *dev;
	struct single_if_info *next;
};

struct if_brctl_header{
	spinlock_t lock;
	struct single_if_info *head;
};

static struct if_brctl_header if_brctl_head_obj;


static void deliver_skb_by_if(struct sk_buff *skb, struct net_device *dev)
{
	struct sk_buff *skb_new = NULL;
	int ret;
	skb_new = skb_clone(skb, GFP_ATOMIC);
	if(!skb_new)
	{
		dbgMsg("<0>""Error when skb-clone !\n");
		return;
	}
	
	skb_push(skb_new, ETH_HLEN);
	skb_new->dev = dev;
	dbgMsg("indev %s outdev %s\n", skb->dev->name, skb_new->dev->name);
	ret = dev_queue_xmit(skb_new);
	dbgMsg("dev_queue_xmit return %d\n", ret);
	if (ret)
	{
		/* let gcc463 not complain. */
	}
}

static inline struct single_if_info * lookup_if_byaddr_inlist 
(struct if_brctl_header *br, const struct net_device *indev)
{
	struct single_if_info * bif;

	bif = br->head;
	while(bif)
	{
		if(bif->dev && bif->dev == indev)
		{
			return bif;
		}	
		bif = bif->next;
	}
	
	return NULL;

}

static inline struct single_if_info * lookup_if_byname_inlist 
(struct if_brctl_header *br, const char *name)
{
	struct single_if_info * bif;

	bif = br->head;
	while(bif)
	{
		if(strcmp(bif->name, name) == 0)
		{
			return bif;
		}	
		bif = bif->next;
	}
	
	return NULL;

}

static int if_bridge_handler(struct sk_buff *skb)
{

	struct net_device *indev = skb->dev;
	struct single_if_info *find_if_p = NULL;		//Pointer to indev in list
	struct single_if_info *tmp_p = NULL;			//Pointer to traverse list

	if ( skb->pkt_type == PACKET_HOST || skb->pkt_type == PACKET_LOOPBACK )
	{
		dbgMsg("<0>""%s: %s, ignore!\n", __FUNCTION__, skb->pkt_type == PACKET_HOST?"PACKET_HOST":"PACKET_LOOPBACK");
		return -1;
	}

	if(!indev)
	{
		dbgMsg("<0>""Can't find net-work interface dev!\n");
		return -1;
	}
	dbgMsg("INPUT Dev name : %s\n", skb->dev->name);
	
	spin_lock_bh(&if_brctl_head_obj.lock);
	if( (find_if_p = lookup_if_byaddr_inlist(&if_brctl_head_obj, indev) ) == NULL)
	{
		/*If can't find same if-dev, just return*/
		goto _out;
	}
	
	tmp_p = if_brctl_head_obj.head;
	while(tmp_p)
	{
		/* Common case: 
		 * WAN if: wan_if_flag = 1
		 * LNA if: wan_if_flag = 0
		 * When in dev is WAN, should pass packet to all lan dev
		 * When in dev is LAN, only need pass to wan dev
		 */
		
		if(tmp_p->dev && tmp_p->wan_if_flag != find_if_p->wan_if_flag
#ifdef MULTI_GROUP_SUPPORT
				&& tmp_p->group == find_if_p->group
#endif
				)
		{
			dbgMsg("tmp_p->name:%s, find_p->name:%s\n",tmp_p->name, find_if_p->name);
			deliver_skb_by_if(skb, tmp_p->dev);
		}
		tmp_p = tmp_p->next;
	}

_out:
	spin_unlock_bh(&if_brctl_head_obj.lock);
	return 0;
}


/* `GFP_KERNEL' Only allow this function to be called under process context. */
static struct single_if_info *add_if
(struct if_brctl_header *head_obj, const char *ifname, int is_wan
#ifdef MULTI_GROUP_SUPPORT
 , int group
#endif
 )
{
	struct single_if_info *bif;
	struct net_device * dev;
	//struct single_if_info *tmp_p;
	
	dev = dev_get_by_name(&init_net, ifname);

	if (!dev) {
		dbgMsg("<0>""BR-CTL-:%s not found. \n", ifname);
		return NULL;
	}
	
	bif = kmalloc(sizeof(struct single_if_info), GFP_KERNEL);
	if (!bif) {
		dev_put(dev);
		return NULL;
	}
	
	memset(bif, 0, sizeof(*bif));
	strncpy(bif->name, ifname, IFNAMSIZ);
	bif->wan_if_flag = is_wan;
#ifdef MULTI_GROUP_SUPPORT
	bif->group = group;
#endif
	bif->dev = dev;
	bif->next = head_obj->head;
	head_obj->head = bif;
	
#if 0
	tmp_p = head_obj->head;
	while(tmp_p){
		dbgMsg("<0>""%x\n", (unsigned int)tmp_p);
		tmp_p = tmp_p->next;
	}
#endif

	return bif;
}

static ssize_t if_br_ctl_read (struct file *filp, char __user *buf, 
								size_t count, loff_t * offp)
{
	char data[512];
	int len = 0;
	struct single_if_info *tmp_p = if_brctl_head_obj.head;
	
	/*Only return a ZERO, this func will be stopped.*/
	if (*offp > 0)
	return 0;
	
	spin_lock_bh(&if_brctl_head_obj.lock);
	while(tmp_p)
	{
#ifdef MULTI_GROUP_SUPPORT
		len += sprintf(data+len, "If-name:%s, wan:%d, group:%d, tmpp_addr:%x\n", 
						tmp_p->name, tmp_p->wan_if_flag, tmp_p->group, (unsigned int)tmp_p);
#else
		len += sprintf(data+len, "If-name:%s, wan:%d, tmpp_addr:%x\n", 
						tmp_p->name, tmp_p->wan_if_flag, (unsigned int)tmp_p);
#endif
		tmp_p = tmp_p->next;
	}
	spin_unlock_bh(&if_brctl_head_obj.lock);
	
	if (copy_to_user(buf, data, len)) {
		return -EFAULT;
	}
	*offp = len;
	return len;
}

static ssize_t if_br_ctl_write (struct file *filp, const char *buffer,
                                   size_t count, loff_t * offp)
{
	char line[128];
	int ret;
#ifdef MULTI_GROUP_SUPPORT
	int group = 0;
	char *ifname = NULL;
#endif
	
	if ( count >= sizeof(line) ) {
		dbgMsg("<0>""command too long.\n");
		return -EFAULT;
	}

	if (copy_from_user(line, buffer, count))
		return -EFAULT;
	
	if(count > 1)
		line[count-1] = 0;
	dbgMsg("<0>""command: %s, len:%d\n", line, count);
	
	ret = count;

#ifdef MULTI_GROUP_SUPPORT
	if (count <= 4)
		ret = -EFAULT;

	/* WAN0=XXX, WAN1=XXX, keep back-compative to WAN=XXX */
	if (line[3] == '=') {
		group = 0;
		ifname = (char *)line + 4;
	} else {
		group = line[3] - '0'; /* support 10 groups should be enough */
		ifname = (char *)line + 5;
	}
#endif
	
	spin_lock_bh(&if_brctl_head_obj.lock);
	switch (line[0]) {
		case 'W':
#ifdef MULTI_GROUP_SUPPORT
			if(!add_if(&if_brctl_head_obj, ifname, 1, group))
				ret = -EFAULT;
#else
			/* WAN=XXX, make it simple. */
			if (count <= 4 || !add_if(&if_brctl_head_obj, line + 4, 1) )
				ret = -EFAULT;
#endif
			break;
		case 'L':
#ifdef MULTI_GROUP_SUPPORT
			if(!add_if(&if_brctl_head_obj, ifname, 0, group))
				ret = -EFAULT;
#else
			/* LAN=XXX, make it simple. */
			if (count <= 4 || !add_if(&if_brctl_head_obj, line + 4, 0) ) 
				ret = -EFAULT;
#endif
			break;
		default:
			dbgMsg("Unknown command.\n");
			break;
	}
	spin_unlock_bh(&if_brctl_head_obj.lock);

	return ret;
}

static int if_netdev_event 
(struct notifier_block* self, unsigned long event, void* ptr)
{
	struct net_device *dev = (struct net_device*)ptr;
	struct single_if_info *bif;

	dbgMsg("<0>""received net_device (%s) event 0x%x \n", dev->name, (int)event);

	switch (event) {	
		case NETDEV_DOWN:
			spin_lock_bh(&if_brctl_head_obj.lock);	
			if ( (bif = lookup_if_byname_inlist(&if_brctl_head_obj, dev->name)) != NULL) {
				if (bif->dev)
					dev_put(bif->dev);
				bif->dev = NULL;
			}
			spin_unlock_bh(&if_brctl_head_obj.lock);
			break;
		case NETDEV_UP:
			spin_lock_bh(&if_brctl_head_obj.lock);
			if ((bif = lookup_if_byname_inlist(&if_brctl_head_obj, dev->name)) != NULL) {
				if (bif->dev)
					dev_put(bif->dev);
				bif->dev = dev_get_by_name(&init_net, bif->name);
			}
			spin_unlock_bh(&if_brctl_head_obj.lock);
			break;
		default:
			break;
	}

	return NOTIFY_DONE;
}


static struct file_operations fops = {
	.owner	= THIS_MODULE,
	.read	= if_br_ctl_read,
	.write	= if_br_ctl_write,
};

static struct notifier_block if_netdev_notifier = {
	.notifier_call = if_netdev_event,
};


/*
 * Init a list header for single_if_info, the pointer header 
 * will point to the first struct
 */
static void init_if_header(void)
{
	if_brctl_head_obj.head = NULL;
	spin_lock_init(&if_brctl_head_obj.lock);
}

static void put_all_dev(void)
{
	struct single_if_info * bif;

	bif = if_brctl_head_obj.head;
	while(bif)
	{
		if(bif->dev)
		{
			dev_put(bif->dev);
		}	
		bif = bif->next;
	}

}
static int __init init (void)
{
	
	struct proc_dir_entry *file;
	
	dbgMsg("<0>""If-bridge-hooker init!\n");
	
	init_if_header();
	
	// proc_create(name, mode, net->proc_net, fops);
	
	//if(!(file = proc_net_fops_create("if_br_ctl",0666,&fops)))
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	if(!(file = proc_create("if_br_ctl", 0666, init_net.proc_net, &fops)))
#else
	if(!(file = proc_net_fops_create(&init_net, "if_br_ctl", 0666, &fops)))
#endif
	{
		return -ENOMEM;
	}	
	
	register_netdevice_notifier(&if_netdev_notifier);

	if_bridge_hook = if_bridge_handler;
	dbgMsg("<0>""If-bridge-hooker init: Complete!\n");
	return 0;
}

static void __exit fini (void)
{
	if_bridge_hook = NULL;
	unregister_netdevice_notifier(&if_netdev_notifier);
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	remove_proc_entry("if_br_ctl", init_net.proc_net /* parent dir */);
#else
	proc_net_remove(&init_net, "if_br_ctl");
#endif
	/*free the dev pointer*/
	put_all_dev();
}

module_init(init);
module_exit(fini);
MODULE_LICENSE("GPL");
