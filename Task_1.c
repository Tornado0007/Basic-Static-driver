/*
 * tasks.c
 *
 * This kernel module creates the entry /proc/tasks. Reading from this
 * file will output information about each task in the kernel's
 * process list. The module does not implement any file operation.
 *
 * A task is identified by a task_struct structure
 * (include/linux/sched.h). The head of the tasks list is located by
 * the init_task symbol, which is declared in include/asm/processor.h
 *
 * Notice the use of the for_each_task(task) macro, defined in
 * sched.h.  Also notice how we need to protect the tasks list from
 * being modified while we are reading it - read_lock(&tasklist_lock). 
 * The tasklist_lock variable is defined in kernel/sched.c.
 *
 * REQUIREMENTS
 *
 * None.
 *
 * AUTHOR
 *
 * Emanuele Altieri
 * ealtieri@cs.smith.edu
 * 
 * Last Modified: June 3, 2002
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

#include <linux/sched.h>        /* task_struct structure */
#include <asm/processor.h>      /* init_task symbol */

/* Data */

static struct file_operations tsk_fops = { /* NULL */ };
static const char *tsk_device = "tasks";
static int tsk_major;

/* Private functions */

static int tsk_proc_read(char*, char**, off_t, int, int*, void*);

/**************************************************************************/
/*                          INITIALIZATION                                */
/**************************************************************************/

static int __init tsk_init(void)
{
	struct proc_dir_entry *proc_entry;
	/* register the device */
	SET_MODULE_OWNER(&tsk_fops);
	if ((tsk_major = register_chrdev(0, tsk_device, &tsk_fops)) < 0)
		return(-EBUSY);
	/* create /proc/tasks entry */
	proc_entry = create_proc_read_entry
		(tsk_device,     /* name of the entry             */
		 0,              /* default mode                  */
		 NULL,           /* no parent directory           */
		 tsk_proc_read,  /* proc read operation handler   */
		 NULL);          /* client data                   */
	if (proc_entry == NULL) {
		unregister_chrdev(tsk_major, tsk_device);
		return(-EBUSY);
	}
	return(0);
} // tsk_init()

static void __exit tsk_exit(void)
{
	unregister_chrdev(tsk_major, tsk_device);
	remove_proc_entry(tsk_device, NULL /* parent dir */);
} // tsk_exit()

static int tsk_proc_read(char *buf, char **start, off_t offset, 
			 int count, int *eof, void *data)
{
	struct task_struct *task;
	int len = 0;
	len += sprintf(buf+len, "PID\tSTATE\n");
	read_lock(&tasklist_lock);   /* protect task list */
	for_each_task(task) {
		len += sprintf(buf+len, "%d\t%lu\n", task->pid, task->state);
	}
	read_unlock(&tasklist_lock);
	*eof = 1;
	return(len);
} // tsk_proc_read()


module_init(tsk_init);
module_exit(tsk_exit);