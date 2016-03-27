#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/list.h>

struct birthday {
              	int day;
		int month;
		int year;
		struct list_head list;
		};

struct birthday *person_1;
struct birthday *person_2;
struct birthday *person_3;
struct birthday *person_4;
struct birthday *person_5;

static LIST_HEAD(birthday_list);

/*struct birthday *ptr, *next 

/*list for each entry safe(ptr,next,&birthday list,list) {
/* on each iteration ptr points */
/* to the next birthday struct
list del(&ptr->list);
kfree(ptr);
}*/
/* This function is called when the module is loaded. */
int simple_init(void)
{       person_1= kmalloc(sizeof(*person_1), GFP_KERNEL);
	person_1->day = 1;
	person_1->month = 1;
	person_1->year = 1995;
	INIT_LIST_HEAD(&person_1->list);
	list_add_tail(&person_1->list, &birthday_list);


	person_2= kmalloc(sizeof(*person_2), GFP_KERNEL);
	person_2->day = 2;
	person_2->month= 2;
	person_2->year = 1995;
	INIT_LIST_HEAD(&person_2->list);
	list_add_tail(&person_2->list, &birthday_list);


	person_3 = kmalloc(sizeof(*person_3), GFP_KERNEL);
	person_3->day = 3;
	person_3->month= 3;
	person_3->year = 1995;
	INIT_LIST_HEAD(&person_3->list);
	list_add_tail(&person_3->list, &birthday_list);


	person_4= kmalloc(sizeof(*person_4), GFP_KERNEL);
	person_4->day = 4;
	person_4->month= 4;
	person_4->year = 1995;
	INIT_LIST_HEAD(&person_4->list);
	list_add_tail(&person_4->list, &birthday_list);


	person_5= kmalloc(sizeof(*person_5), GFP_KERNEL);
	person_5->day = 5;
	person_5->month= 5;
	person_5->year = 1995;
	INIT_LIST_HEAD(&person_5->list);
	list_add_tail(&person_5->list, &birthday_list);

       	printk(KERN_INFO "Loading Module\n");
      	 struct birthday *ptr;
       	list_for_each_entry(ptr, &birthday_list, list) {
     	/* on each iteration ptr points */
    	 /* to the next birthday struct */
       printk(KERN_INFO "The birthday date is: %d/%d/%d (Month/Date/Year)\n",ptr->month,ptr->day,ptr->year);

         }
       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
        struct birthday *ptr,*next;
        list_for_each_entry_safe(ptr,next,&birthday_list,list) {
        /* on each iteration ptr points */
        /* to the next birthday struct */
	list_del(&ptr->list);
        if(list_empty(&birthday_list))
             printk(KERN_INFO "The link list has been removed");
        else
              printk(KERN_INFO "The link list hasn't been removed yet");
	}
       

	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

