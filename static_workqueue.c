#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fcntl.h> /*Helps fix O_ACCMODE*/
#include <linux/sched.h> /*Helps fix TASK_UNINTERRUPTIBLE */
#include <linux/fs.h> /*Helps fix the struct intializer */

#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>

#include <linux/proc_fs.h>    // proc file system
#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/proc_ns.h>

#include <linux/kobject.h>

#include <linux/interrupt.h>

#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 
#include <linux/sched.h>               //task_struct

#include <linux/timer.h>
#include <linux/jiffies.h>

#include <linux/workqueue.h> // Required for workqueues

#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johny");
MODULE_DESCRIPTION(" work queue  driver");
MODULE_VERSION("1.0.0");

void workq_fn(struct work_struct *work); 
DECLARE_WORK(workq,workq_fn); 


void workq_fn(struct work_struct *work)
{
	
	long c;
	
	
	printk(KERN_INFO"my work queue static function");
	
	if(in_interrupt())
        printk(KERN_INFO "Running as an interrupt");
	else
		 printk(KERN_INFO "Running as an processt");
		 
	atomic_long_set(&(workq.data),10);	
	printk(KERN_INFO "In workq function %u",atomic_long_read(&(workq.data)));
	
	
}

static int __init my_workqueue_init(void)
{
	schedule_work(&workq);
	printk(KERN_INFO"My workqueue init static function");
return 0;
}

static void __exit my_workqueue_exit(void)
{
	flush_scheduled_work();
	printk(KERN_INFO"My workqueue exit  static function");
}

module_init(my_workqueue_init);
module_exit(my_workqueue_exit);

