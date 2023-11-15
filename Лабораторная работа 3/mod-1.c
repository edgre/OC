
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>


int init_module(void)
{
	printk ("Welcome to the Tomsk State University\n");
	return 0;
}

void cleanup_module(void)
{
	printk ("Tomsk State University forever!\n");
}

MODULE_LICENSE("GPL");


