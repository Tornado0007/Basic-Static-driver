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
MODULE_DESCRIPTION(" delayed work queue  driver");
MODULE_VERSION("1.0.0");

unsigned int onesec;
static struct workqueue_struct *own_workqueue;
 
static void workqueue_fn(struct work_struct *work); 

static DECLARE_DELAYED_WORK(work, workqueue_fn);
 
/*Workqueue Function*/
static void workqueue_fn(struct work_struct *work)
{
    printk(KERN_INFO "Executing my own Workqueue Function\n");
	printk(KERN_INFO "mydelayed  work %u jiffies\n", (unsigned)onesec);
    return;
        
}



static int __init my_workqueue_init(void)
{
         onesec = msecs_to_jiffies(1000);
	 printk(KERN_INFO "mydelayed  loaded %u jiffies\n", (unsigned)onesec);
	 if(own_workqueue==NULL)
	 {
		printk(KERN_INFO "Creating workqueue ");
		own_workqueue = create_workqueue("own_wq");	
	 }
	 if(own_workqueue)
	 {
		 printk(KERN_INFO "delayed workqueue ");
		 queue_delayed_work(own_workqueue,&work,onesec);
	 }
	

	printk(KERN_INFO"My own workqueue init hi hi hi  function");
return 0;
}

static void __exit my_workqueue_exit(void)
{
	/* Delete workqueue */
    destroy_workqueue(own_workqueue);
	printk(KERN_INFO"My own workqueue exit  bye bye bye function");
}

module_init(my_workqueue_init);
module_exit(my_workqueue_exit);



