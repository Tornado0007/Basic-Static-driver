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



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johny");
MODULE_DESCRIPTION(" major and minor  driver");
MODULE_VERSION("1.0.0");

#define MEM_SIZE        1024            //Memory Size
#define WR_VALUE _IOW('a','a',int32_t*) //(copy_from_user)
#define RD_VALUE _IOR('a','b',int32_t*) //(copy_to_user)

dev_t dev =0;

uint8_t *kernel_buffer =NULL;
int32_t value = 0;




static int      __init my_driver_init(void);
static void     __exit my_driver_exit(void);


//***************************Interrupts***************************************
#define IRQ_NO 2 
static int i = 0;
static irqreturn_t irq_handler(int irq,void *dev_id)
{
	printk(KERN_INFO"Timer Int : %d\n",++i);
	i++;
	return IRQ_HANDLED;
}
//***************************Interrupts End**************************************





static int __init my_driver_init(void)
{
	if((alloc_chrdev_region(&dev,0,1,"my_Char"))<0)
	{
		printk(KERN_INFO"cannot allocate major and minor number");
		return -1;
	}
	printk(KERN_INFO"Major =%d Minor = %d \n",MAJOR(dev),MINOR(dev));
	
	
	
	
	if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "my_interrupt_device", (void *)(irq_handler))) {
            printk(KERN_INFO "my_interrupt_device: cannot register IRQ ");
            return -1;
        }
	printk(KERN_INFO"character drivre init sucess\n");

	return 0;

}

static void __exit my_driver_exit(void)
{	
	free_irq(IRQ_NO,(void*)(irq_handler));

	
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "kernel driver removed  ... done \n");
}
module_init(my_driver_init);
module_exit(my_driver_exit);
