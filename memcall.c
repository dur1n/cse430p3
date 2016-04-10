#include <linux/unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define __NR_my_syscall 359


main(int argc, char *argv[]) {
	char *c;
    	int pid = atoi(argv[1]);  // Converting a string argument to an integer 
    	unsigned long addr = strtoul(argv[2], &c, 16); // strtoul() convert into long
	long address = syscall(__NR_my_syscall, pid, addr);

	if (address == -1)
		printf("page is not available.\n");
	else
		printf("PID:%d\tPhysical Address:0x%lx\n", pid, address);
	return 0;
}
