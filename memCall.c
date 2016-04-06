#include <linux/kernel_stat.h>
#include <linux/mm.h>
#include <linux/hugetlb.h>
#include <linux/mman.h>
#include <linux/swap.h>
#include <linux/highmem.h>
#include <linux/pagemap.h>
#include <linux/ksm.h>
#include <linux/rmap.h>
#include <linux/export.h>
#include <linux/delayacct.h>
#include <linux/init.h>
#include <linux/writeback.h>
#include <linux/memcontrol.h>
#include <linux/mmu_notifier.h>
#include <linux/kallsyms.h>
#include <linux/swapops.h>
#include <linux/elf.h>
#include <linux/gfp.h>
#include <asm/io.h>
#include <asm/pgalloc.h>
#include <asm/uaccess.h>
#include <asm/tlb.h>
#include <asm/tlbflush.h>
#include <asm/pgtable.h>

asmlinkage long sys_my_syscall(int pid, long addr){

	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	spinlock_t *ptl;
	pte_t *ptep = NULL
	pte_t pte;

	struct task_struct *task;

	// loop each of the tasks
	for_each_process(task){
		if (pid == task->pid){ // matching PID

			// checks for valid page table in the page global Directory
			pgd = pgd_offset(task->mm, addr);
    		if (pgd == NULL || pgd_none(*pgd) || unlikely(pgd_bad(*pgd))) 
				return -1;

			// checks for valid page table in the page upper Directory
			pud = pud_offset(pgd, addr);
			if (pud == NULL || pud_none(*pud) || unlikely(pud_bad(*pud))){
				return -1;

			// checks for valid page table in the page middle Directory
			pmd = pmd_offset(pud, addr);
			if (pmd == NULL || pmd_none(*pmd) || unlikely(pmd_bad(*pmd))){
				return -1;

			// checks for valid page table 
			ptep = pte_offset_map_lock(task->mm, pmd, addr, &ptl);
			if (ptep == NULL || pte_none(*ptep) || ptl == NULL)
				return -1;

			pte = *ptep;
			if (!pte_present(pte))
				return -1;
			else {
				// not sure how to translate: 
				
				// mem address
				// SHIFT LEFT 12 bits
				// add last 12 bits of virtual address
				// to the memory address of pte
				
				return physAddr;				
			}
			else{

				if (pte_none(pte))
					return -1;

				return pte_pfn(pte);
			}

		}
	}

	return -1; 
}
