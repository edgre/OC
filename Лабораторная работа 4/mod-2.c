#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

int num=0;

#define profcs_name "tsulab"
static struct proc_dir_entry *our_proc_file = NULL;
static ssize_t procfile_read(
struct file *file_pointer, char __user *buffer,
size_t buffer_length, loff_t* offset)
{
static bool finished = 0;
char s[100];
if (finished)
{
finished=0;
return 0;
}

num++;
int len=sprintf(s, "This file has been read %d times \n", num);
copy_to_user(buffer, s, len);
pr_info("procfile read %s\n",
file_pointer->f_path.dentry->d_name.name);
finished = 1;
return len;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION (5,6,0)
static const struct proc_ops proc_file_fops = {
.proc_read = procfile_read,
};
#else
static const struct file_operations proc_file_fops = {
.read=procfile_read,
};
#endif



static int __init procfsl_init(void)
{
	our_proc_file = proc_create(
	profcs_name, 0644, NULL, &proc_file_fops);
	return 0;
}

void __exit procfsl_exit(void)
{
	proc_remove (our_proc_file);
	pr_info ("/proc/%s removed\n", profcs_name);
}
module_init(procfsl_init);
module_exit(procfsl_exit);
MODULE_LICENSE("GPL");


