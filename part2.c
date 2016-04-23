#include <linux/syscalls.h>
#include <linux/kernel.h> /* Needed for KERN_INFO*/
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/linkage.h>
#include <linux/highmem.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <asm/pgtable_types.h>
#include <asm/pgtable.h>
#include <asm/tlbflush.h>
#include <linux/pagemap.h>
#include <linux/rmap.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <asm/page.h>
#include <linux/rwsem.h>
#include <linux/hugetlb.h>




asmlinkage long sys_my_syscall(int pid, unsigned long addr){

	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	spinlock_t *ptl;
	pte_t *ptep;
	pte_t pte;
	swp_entry_t swp;

	struct task_struct *task;
	unsigned long pfn_pframe; // need unsigned to logical shift will work correctly
	unsigned long offset;
	unsigned long offs_shifted;
	unsigned long physAddr;
	int shift = 12;

	// loop each of the tasks
	for_each_process(task){
		if (pid == task->pid){ // matching PID

			// checks for valid page table in the page global Directory
			pgd = pgd_offset(task->mm, addr);
    			if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd))) 
				return -1;

			// checks for valid page table in the page upper Directory
			pud = pud_offset(pgd, addr);
			if (pud_none(*pud) || unlikely(pud_bad(*pud)))
				return -1;

			// checks for valid page table in the page middle Directory
			pmd = pmd_offset(pud, addr);
			if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
				return -1;

			// checks for valid page table 
			ptep = pte_offset_map_lock(task->mm, pmd, addr, &ptl);
			
			/* From discusion board: First obtain the physical page frame number 
			by pte_pfn(). Then obtain the offset address inside a page from the 
			virtual address, i.e the offset is just the lower 12 bits of the 
			virtual address on 32 bit machine. After that you can build the 
			physical address by shift the pfn left  by 12 bits and put the 
			offset 12 bits there. */
			pte = *ptep;
			pte_unmap_unlock(ptep,ptl);
			if (pte_present(pte)){
				pfn_pframe= pte_pfn(pte); 	// page frame
				offset = (addr & 0x00000FFF); // obtain the offset address 
				offs_shifted = pfn_pframe << shift;
				physAddr = offs_shifted|offset; 
				return physAddr;				
			}
			/* If the page is not present, i.e. pte_present() returns false, 
			then you need to figure out whether the page is not allocated and
			mapped at all, or it is swapped out, by testing pte_none(). 
			If it is swapped out, just return the value of pte. 
			That value is the swap id.*/
			else{
				if (pte_none(pte))
					return -1;
				else{
					swp = pte_to_swp_entry(pte); // swap offset
					return swp_offset(swp);
				}
			}	

		}
	}
	return -1; 
}
