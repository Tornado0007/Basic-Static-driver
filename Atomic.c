#include <linux/module.h>
#include <linux/kernel.h>


#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 
//#include <linux/sched.h>               //task_struct
 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John");
MODULE_DESCRIPTION("Atomic");
MODULE_VERSION("1.0.0");

atomic_t my_global_variable = ATOMIC_INIT(0);


static struct task_struct *char_thread;
static struct task_struct *char_thread1;

int mydata =0;
int my_thread_fun(void *p);



int my_thread_fun(void *p)
{
	while(!kthread_should_stop())
	{		
		//Atomically add one to v
		atomic_inc(&my_global_variable); // single operation ; no much overhead 
		printk(KERN_INFO "my_thread_fun after inc  %lu\n", atomic_read(&my_global_variable));
		
			
		//atomic_read(&my_global_variable)
		atomic_add(14, &my_global_variable);
		printk(KERN_INFO "Adding 14 my_thread_fun after add   %lu\n", atomic_read(&my_global_variable));

		//Atomically subtract i from v
		atomic_sub(2,&my_global_variable);
		printk(KERN_INFO "Sub 2my_thread_fun after sub   %lu\n", atomic_read(&my_global_variable));

		//Atomically subtract one from v
		atomic_dec(&my_global_variable);
		printk(KERN_INFO "Decrement Atomic_dec after dec   %lu\n", atomic_read(&my_global_variable));
		
		//Atomically set v equal to i
		atomic_set(&my_global_variable, 14);			
		printk(KERN_INFO "Atomic_Set to 14 after set    %lu\n", atomic_read(&my_global_variable));
		
		// Function Atomically subtract i from v and return true if the result is zero; otherwise false 
		int res = atomic_sub_and_test(3, &my_global_variable);
		printk(KERN_INFO "Atomic Sub and Test by 3     %lu\n", res);

		// Function Atomically dec one from v and return true(1) if the result is zero; otherwise false (0)
		res = atomic_dec_and_test(&my_global_variable);
		printk(KERN_INFO "Atomic_dec_and_test    %lu\n", res);
		printk(KERN_INFO "Atomic Read    %lu\n", atomic_read(&my_global_variable));
			
		//Atomically add i to v and return true if the result is negative; otherwise false
		res = atomic_add_negative(8,&my_global_variable);
		printk(KERN_INFO "Atomic and Negative and by 8    %lu\n", res);
		printk(KERN_INFO "Atomic Read     %lu\n", atomic_read(&my_global_variable));

		//Atomically increment v by one and return true if the result is zero; false otherwise
		res = atomic_inc_and_test(&my_global_variable);
		printk(KERN_INFO "atomic_inc_and_test     %lu\n", res);
		printk(KERN_INFO "atomic_read      %lu\n", atomic_read(&my_global_variable));

		printk(KERN_INFO "*****************ATOMIC BIT WSIE OPERATORS *************");
		
		unsigned long word = 0;

		set_bit(0, &word);       /* bit zero is now set (atomically) */
		printk(KERN_INFO "SET 0th BIT      %x\n", word);

		set_bit(1, &word);        /* bit one is now set (atomically) */
		printk(KERN_INFO "SET_BIT 1th      %x\n", word);

		set_bit(2, &word);        /* bit two is now set (atomically) */
		printk(KERN_INFO "set_bit 2nd      %x\n", word);

		clear_bit(1, &word);     /* bit one is now unset (atomically) */
		printk(KERN_INFO "clear_bit 1th      %x\n", word);

		change_bit(0, &word);    /* bit zero is flipped; now it is unset (atomically) */
		printk(KERN_INFO "change_bit 0th      %x\n", word);

		//Atomically set the nr-th bit starting from addr and return the previous value
		res = test_and_set_bit(1, &word);
		printk(KERN_INFO "return value test_and_set_bit 1 to word    %lu\n", res);
		printk(KERN_INFO "getting the return test_and_set_bit to word      %x\n", word);

		//Atomically clear the nr-th bit starting from addr and return the previous value
		res = test_and_clear_bit(1,&word);
		printk(KERN_INFO "TEST AND CLEAR    %lu\n", res);
		printk(KERN_INFO "TEST AND CLEAR      %x\n", word);
		
		//Atomically return the value of the nr-th bit starting from addr
		res = test_bit(2,&word);
		printk(KERN_INFO "TEST BIT    %lu\n", res);  

		msleep(1000);
		
	}
	return 0;	
}

int my_thread_fun1(void *p)
{
	while(!kthread_should_stop())
	{
		
		atomic_inc(&my_global_variable);
		printk(KERN_INFO "my_thread_fun2  %lu\n", atomic_read(&my_global_variable));
		
		msleep(1000);
	}
	return 0;
}


static int __init my_simpledriver_init(void)
{
	printk(KERN_INFO"My sample driver start");

	
	char_thread = kthread_run(my_thread_fun,NULL,"my char thread");
	if(char_thread)
	{
		printk(KERN_INFO" create the thread");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}
#if 0
	char_thread1 = kthread_run(my_thread_fun1,NULL,"my char thread1");
	if(char_thread1)
	{
		printk(KERN_INFO" create the thread1");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}
#endif
	return 0;

}


static void __exit my_simpledriver_exit(void)
{
	printk(KERN_INFO"My samle driver exit\n");

	kthread_stop(char_thread);
	//kthread_stop(char_thread1);

}
module_init(my_simpledriver_init);
module_exit(my_simpledriver_exit);



