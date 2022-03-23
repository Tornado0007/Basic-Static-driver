#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>

static int my_tasklet_function(void);
static void my_tasklet_exit(void);




static void my_tasklet_handler(unsigned long flag);

DECLARE_TASKLET(my_tasklet,my_tasklet_handler,0);

static void my_tasklet_handler(unsigned long flag)
{
	tasklet_disable(&my_tasklet);
	printk(KERN_INFO"My tasklet handler function");
	tasklet_enable(&my_tasklet);
}


static int  my_tasklet_function(void)
{
	printk(KERN_INFO"My tasklet function start ");
	//Schedule the tasklet 
	tasklet_schedule(&my_tasklet);
	return 0;
}

static void  my_tasklet_exit()
{
	tasklet_kill(&my_tasklet);
	printk(KERN_INFO"My tasklet function exit");
}

module_init(my_tasklet_function);
module_exit(my_tasklet_exit);
