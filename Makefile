CONFIG_MODULE_SIG=n
#CONFIG_GENERIC_HARDIRQ=y

#obj-m :=chr_drv_static.o
#obj-m :=chr_drv_dynamic.o
#obj-m :=chr_drv_udev.o
#obj-m :=Ioctl.o
#obj-m :=interrupts_1.o
#obj-m :=kthread_create.o
#obj-m :=mutex_code_prio.o
#obj-m :=mutex_try.o
#obj-m :=spinlock.o
#obj-m :=mutex_killable.o
#obj-m :=Atomic.o
#obj-m :=listinterrupt.o
#obj-m :=sem_code2.o
#obj-m :=sysfs_file.o
#obj-m :=procfs_file.o
#obj-m :=delayed_workqueue.o
#obj-m :=own_workedQueue.o
#obj-m :=static_workqueue.o
#obj-m :=tasklet_dynamic.o
#obj-m :=tasklet.o
#obj-m :=tasklet_dynamic_interrupt.o
#obj-m :=memdesc.o
#obj-m :=vmmemk.o
obj-m :=procfs_pageinfo.o

KDIR= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR)  M=$(shell pwd) modules
clean:
	make -C $(KDIR)  M=$(shell pwd) clean
