/*
 
This is the exercise of <OS Concepts 9/e> programming projects in Chapter2. 

Programmer: Chao-Chun(Chris) Cheng @ ECpE Iowa State University

Date: 2016.07.19

*/




#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h> // fix error: implicit declaration of function ‘kmalloc’/'kfree'

struct birthday
{
	int day;
	int month;
	int year;
	struct list_head list;
};
struct birthday *ptr, *next, *person; 
static LIST_HEAD(birthday_list); // declare a list_head object

/* This function is called when the module is loaded. */
int simple_init(void)
{
	printk(KERN_INFO "Loading Module\n");
	int i = 0;
	
/*===========================================================================================
Initialize the linked-list.
=============================================================================================*/
		
	for(i = 0; i < 5; i++)
	{
		person = kmalloc(sizeof(*person), GFP_KERNEL);
		person->day = 30 - i;
		person->month = 10 - i;
		person->year = 2000 + i;
		INIT_LIST_HEAD(&person->list);
		list_add_tail(&person->list, &birthday_list);
	}

/*===========================================================================================
Check and traverse the linked-list.
=============================================================================================*/
	list_for_each_entry_safe(ptr, next, &birthday_list, list)
	{
		printk("Person #%d:%d-%d-%d \n", 5-i, ptr->year, ptr->month, ptr->day);
		i--;
	}

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
/*===========================================================================================
Delete and free the linked-list.
=============================================================================================*/
	int i;

	list_for_each_entry_safe(ptr, next, &birthday_list, list)
	{
		list_del(&ptr->list);
		kfree(ptr);
	}

//Check again

	list_for_each_entry_safe(ptr, next, &birthday_list, list)
	{
		printk("Person #%d:%d-%d-%d \n", 5-i, ptr->year, ptr->month, ptr->day);
	}
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Ch2 Programming Projects");
MODULE_AUTHOR("Chao-Chun(Chris) Cheng");

