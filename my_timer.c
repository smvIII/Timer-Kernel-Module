#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/time64.h> 
#include <linux/timekeeping.h>

MODULE_LICENSE("Dual BSD/GPL");

static struct proc_dir_entry* proc_entry;
static bool read;
static bool initial_time_saved = false;
static struct timespec64 initial_time;


static ssize_t my_read(struct file* file, char* ubuf, size_t count, loff_t *ppos)
{
	if (read)
	{
		read = false;
		return 0;
	}

	printk(KERN_INFO "my_timer read\n");
	struct timespec64 current_time;
	ktime_get_real_ts64(&current_time);

	if (!initial_time_saved)
	{
		initial_time = current_time;
		initial_time_saved = true;
	}

	long epochTimesec = current_time.tv_sec;
	long epochTimensec = current_time.tv_nsec;

	// Calculate elapsed time
	//struct timespec64 elapsed_time;
	long elapsedTimesec = current_time.tv_sec - initial_time.tv_sec;
	long elapsedTimensec = current_time.tv_sec - initial_time.tv_nsec;

	//handle negative nanosecond substraction  
	if (elapsedTimensec < 0)
	{
		elapsedTimesec -= 1;
		elapsedTimensec += 1000000000;
	}

	int len = sprintf(ubuf, "current time: %ld.%ld\nelapsed time: %ld.%ld\n", epochTimesec,
		       	epochTimensec, elapsedTimesec, elapsedTimensec);
	read = true;
	return len;
	
}

static struct proc_ops procfile_fops =
{
	.proc_read = my_read,
}; 

static int my_timer_init(void)
{
	printk(KERN_ALERT "my_timer init");
	proc_entry = proc_create("timer", 0666, NULL, &procfile_fops);
	if (proc_entry == NULL)
		return -ENOMEM;
		
	return 0;
}

static void my_timer_exit(void)
{
	printk(KERN_ALERT "my_timer exit");	
	proc_remove(proc_entry);
	return;
}

module_init(my_timer_init);
module_exit(my_timer_exit);

