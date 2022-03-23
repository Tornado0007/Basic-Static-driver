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
#include <linux/mm.h>

#include <linux/proc_ns.h>



#define MEM_SIZE        1024            //Memory Size
#define WR_VALUE _IOW('a','a',int32_t*) //(copy_from_user)
#define RD_VALUE _IOR('a','b',int32_t*) //(copy_to_user)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johny");
MODULE_DESCRIPTION(" major and minor  driver");
MODULE_VERSION("1.0.0");

dev_t dev =0;
static struct class *dev_class;
static struct cdev my_cdev;
uint8_t *kernel_buffer =NULL;
int32_t value = 0;
char my_array[200]="try_proc_array\n";
static int len = 1;

static int      __init my_driver_init(void);
static void     __exit my_driver_exit(void);
static int      my_open(struct inode *inode, struct file *file);
static int      my_release(struct inode *inode, struct file *file);
static ssize_t  my_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  my_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

/***************** Procfs Functions *******************/
extern unsigned long max_mapnr;
extern struct pglist_data contig_page_data;
struct proc_dir_entry *proc_dir; 
struct proc_dir_entry *proc_file;


static int      open_proc(struct inode *inode, struct file *file); 
static int      release_proc(struct inode *inode, struct file *file); 
static ssize_t  read_proc(struct file *filp, char __user *buffer, size_t length,loff_t * offset); 
static ssize_t  write_proc(struct file *filp, const char *buff, size_t len,  loff_t * off);

char *buf;


static struct proc_ops proc_fops = { 
	.proc_open = open_proc, 
	.proc_read = read_proc, 
	.proc_write = write_proc, 
	.proc_release = release_proc 
 };




static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = my_read,
    .write      = my_write,
    .open       = my_open,
	.unlocked_ioctl = my_ioctl,
    .release    = my_release,
};
/*
** This fuction will be called when we open the procfs file
*/
static int open_proc(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "proc file opend.....\t");
        buf=kmalloc(100,GFP_KERNEL);
	return 0;
}
/*
** This fuction will be called when we close the procfs file
*/
static int release_proc(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "proc file released.....\n");
	kfree(buf);
	return 0;
}
/*

* This fuction will be called when we read the procfs file

*/

static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length,loff_t * offset)
{

  /********************************************************/
    
    int len = 0;
	unsigned long i;

	/* page information */
	struct page *page;
	unsigned long count_used=0, count_reserved=0;

	/* buddy system information */
	struct zone *zonevar;

	/* 
	 * print information about the pages in the system 
	 */
	
	for (i=0, page=mem_map; i<1000; i++, page++) {
		if (PageReserved(page))     /* reserved pages */
			count_reserved++;
		else if (page_count(page))  /* used pages */
			count_used++;
	}
		
	len += sprintf(buf+len, 
		       "page size     : %lu B\n"
		       "total pages   : %lu\n"
		       "reserved pages: %lu\n"
		       "used pages    : %lu\n"
		       "free memory   : %lu KB\n"
		       ,
		       PAGE_SIZE,
		       1000,
		       count_reserved,
		       count_used,
		       (unsigned long)((1000-count_reserved-count_used)*PAGE_SIZE)>>10);

	/*
	 * print information about the buddy system
	 */
 

	zonevar = &contig_page_data.node_zones[ZONE_NORMAL];

	len += sprintf(buf+len,
		       "ZONE %s\n start page : %lu\nfree pageset_high : %u\pageset_batch       : %u pages\n"
		       ,
		       zonevar->name,
		       zonevar->zone_start_pfn,
		       zonevar->pageset_high,
		       zonevar->pageset_batch);

	/*************************************************************
	 * Add code here to print the number of areas in each order of
	 * the free area array (in "zone"). Also, print the number of
	 * pages in each order. The total number of pages should be
	 * the same indicated by the "zone->free_pages" variable
	 * above.
	 *************************************************************/

	
	/**************************************************************/
    copy_to_user(buffer,my_array,200);
    return length;;

}
/*
** This fuction will be called when we write the procfs file
*/
static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    printk(KERN_INFO "proc file wrote.....len = %d \n",len);
    copy_from_user(my_array,buff,len);
    return len;
}

static int my_open(struct inode *inode, struct file *file)
{
	
	if((kernel_buffer = kmalloc(MEM_SIZE,GFP_KERNEL))==0)
	{
		printk(KERN_INFO "Cannot allocate memory in kernel\n");
		return -1;
	}
	printk(KERN_INFO"Driver open  function called .....\n");
	return 0;	
}
static int my_release(struct inode *inode, struct file *file)
{
	kfree(kernel_buffer);
	printk(KERN_INFO"Driver release function called ....\n");
	return 0;
}


static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	int ret =0;
	
	
		
	
	//Copy the data from the kernel space to the user-space
    ret = copy_to_user(buf, kernel_buffer, MEM_SIZE);
	if(ret > 0)
	{
		printk(KERN_INFO"writing data to user-space failed\n");
	}
	printk(KERN_INFO"Driver read function called .....\n");
	return 0;
}



static ssize_t my_write(struct file *filp, const char __user *buf,size_t len,loff_t *off)
{
	int ret = 0;
	//Copy the data to kernel space from the user-space
	copy_from_user(kernel_buffer, buf, len);
	if(ret > 0)
	{
		printk(KERN_INFO"Copy the data to kernel space from the user-space\n");
	}
	printk(KERN_INFO"Driver write function called .... \n");
	return len;
} 
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE:
                        copy_from_user(&value ,(int32_t*) arg, sizeof(value));
                        printk(KERN_INFO "Value = %d\n", value);
                        break;
                case RD_VALUE:
                        copy_to_user((int32_t*) arg, &value, sizeof(value));
                         break;
         }
         return 0;
 }

static int __init my_driver_init(void)
{
	if((alloc_chrdev_region(&dev,0,1,"my_Char"))<0)
	{
		printk(KERN_INFO"cannot allocate major and minor number");
		return -1;
	}
	printk(KERN_INFO"Major =%d Minor = %d \n",MAJOR(dev),MINOR(dev));
	
	//creating cdev structure
	cdev_init(&my_cdev,&fops);
	
	//Adding charriver to system
	if((cdev_add(&my_cdev,dev,1)) <0)
	{
		printk(KERN_INFO"cannot add the device to the system \n");
		goto r_class;
	}

	if((dev_class = class_create(THIS_MODULE,"my_Char_class")) == NULL){
		printk(KERN_INFO"Cannot create the struct class for device");
		goto r_class;
	}
	if((device_create(dev_class,NULL,dev,NULL,"my_Char_driver"))==NULL) {
		printk(KERN_INFO" cannot create the device\n");
		goto r_device;
	}
	
	proc_dir = proc_mkdir("my_proc_dir",NULL);

	/*Creating Proc entry*/
	proc_file = proc_create("Proc_file",0666,proc_dir ,&proc_fops);
	if(proc_file == NULL)
	{
		printk(KERN_ALERT "Error could not open %s\n","proc_file ");
	}

	printk(KERN_INFO"character drivre init sucess\n");

	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

static void __exit my_driver_exit(void)
{
	remove_proc_entry("my_proc",NULL);
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "kernel driver removed  ... done \n");

}
module_init(my_driver_init);
module_exit(my_driver_exit);
