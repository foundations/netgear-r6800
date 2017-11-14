/*
 *  Send serial output to the telnet interface.
 *  We do this by printing the string to the tty associated with the current task.
 *
 *  Q: How to print all message to my telnet interface?
 *  A: Just telnet to DUT and insert this module(You may need to remove it first if module exist already.).
 *
 *  Q: How can I port this code to other platform?
 *  A: 1. copy this module
 *     2. Add hook function in serial driver and tty_io.c
 *
 *  NOTE: NEVER insert this module from console interface!
 *                                                          Argon Cheng
 *                                                          2009-12-17
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>	/* For current */
#include <linux/tty.h>		/* For the tty declarations */
#include <linux/version.h>	/* For LINUX_VERSION_CODE */

#if 1 //def _DEBUG_
#define DBG_PRINT(fmt, arg...)  printk("<0>[print2telnet]" fmt, ##arg)
#else
#define DBG_PRINT(fmt, arg...)
#endif

static struct tty_struct *my_tty=NULL;
//extern void (*telnet_putc)(char);                   /* in drivers/serial/<serial driver.c> */
extern void (*telnet_string)(char*, int);                   /* in drivers/serial/<serial driver.c> */
extern void (*clean_my_tty)(struct tty_struct *);   /* in drivers/char/tty_io.c */
extern struct tty_struct *get_current_tty(void);    /* in drivers/char/tty_io.c */


/*
 * print char to @my_tty
 */
void print_char(char c) {

	if (my_tty != NULL) {
        ((my_tty->driver)->ops->write) (my_tty,	/* The tty itself */
#if ( LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,9) )
            0,	/* Don't take the string from user space */
#endif
            &c,	1);
    }
}

/* it seem the print_char may not be called at mtk platform, so use print_string which will be called in tty write function in serial driver. */
static void print_string(char *buf, int count) {
	if (my_tty != NULL) {
        ((my_tty->driver)->ops->write) (my_tty,	/* The tty itself */
#if ( LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,9) )
            0,	/* Don't take the string from user space */
#endif
            buf,	count);
    }
}

static void clean_tty(struct tty_struct *tty) {
    if(my_tty == tty) {
        DBG_PRINT("Clean tty <%p>\n", tty);
        my_tty = NULL;
    }
    return;
}

static int __init print2telnet_init(void)
{
#if ( LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,5) )
	my_tty = current->tty;
#else
	my_tty = current->signal->tty;
#endif
    DBG_PRINT("Set my_tty to <%p>\n", my_tty);

    /* Init function hook in serial driver */
    //telnet_putc = print_char;
     telnet_string = print_string;
    clean_my_tty = clean_tty;
	return 0;
}

static void __exit print2telnet_exit(void)
{
	//telnet_putc = NULL;
	telnet_string = NULL;
	clean_my_tty = NULL;
}

module_init(print2telnet_init);
module_exit(print2telnet_exit);

