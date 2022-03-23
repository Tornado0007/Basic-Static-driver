/*
 * pageinfo-skel.c
 *
 * Creates /proc/pageinfo. The module outputs information about the
 * pages in the system (total pages, used pages, etc). It also outputs
 * information about the buddy system.
 *
 * You need to add code to print the number of areas in each of the
 * normal zone's free area lists. See pageinfo_proc_read() for more
 * information.
 *
 */
#include <linux/mmzone.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>


/* Data */
extern struct pglist_data contig_page_data;
static const char *pageinfo_device = "pageinfo";

/* Private functions */

extern struct proc_dir_entry *proc_create_data(const char *, umode_t,
                                               struct proc_dir_entry *,
                                               const struct proc_ops *,
                                               void *);


static int pageinfo_proc_read(char*, char**, off_t, int, int*, void*);

/**************************************************************************/
/*                           INIT AND EXIT                                */
/**************************************************************************/

static int __init pageinfo_init(void)
{
	struct proc_dir_entry *proc_entry;
	/* create /proc/pageinfo entry */
	proc_entry = create_proc_read_entry
		(pageinfo_device,     /* name of the entry */
		 0,                   /* default mode */
		 NULL,                /* no parent directory */
		 pageinfo_proc_read,  /* proc read operation handler */
		 NULL);               /* client data */
	if (proc_entry == NULL)
		return(-EBUSY);
	return(0);
} // pageinfo_init()

static void __exit pageinfo_exit(void)
{
	remove_proc_entry(pageinfo_device, NULL /* parent dir */);
} // pageinfo_exit()

/**************************************************************************/
/*                      /PROC ENTRY OPERATIONS                            */
/**************************************************************************/

static int pageinfo_proc_read(char *buf, char **start, off_t offset, 
			 int count, int *eof, void *data)
{
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
	
	for (i=0, page=mem_map; i<max_mapnr; i++, page++) {
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
		       max_mapnr,
		       count_reserved,
		       count_used,
		       (unsigned long)((max_mapnr-count_reserved-count_used)*PAGE_SIZE)>>10);

	/*
	 * print information about the buddy system
	 */
 

	zonevar = &contig_page_data.node_zones[ZONE_NORMAL];

	len += sprintf(buf+len,
		       "ZONE %s\n start page : %lu\nfree pages : %lu\nsize       : %lu pages\n"
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

	*eof = 1;
	return(len);
} // pageinfo_proc_read()




module_init(pageinfo_init);
module_exit(pageinfo_exit);
MODULE_LICENSE("GPL");
