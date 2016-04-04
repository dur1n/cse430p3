#include <linux/syscalls.h>
#include <linux/kernel.h> 
#include <linux/sched.h>
#include <linux/highmem.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/rmap.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <asm/page.h>


asmlinkage long sys_my_syscall(int pid, long addr){

	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	// TO DO

	struct task_struct *task;
	// loop through each of the tasks
	for_each_process(task){
		if (pid == task->pid){
			// TODO: check for page address? 
			// ...
			// return SOMETHING
			}
		}
	}

	return -1; // returns -1 if no address available
}
