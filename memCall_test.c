// mem_user.c test program

#include <linux/unistd.h>
#include <linux/stdio.h>

#define __NR_my_syscall 359

main(int argc, char *argv[]) {
{
    int pid = atoi(argv[1]);  // Converting a string argument to an integer 
    int addr = atoi(argv[2]);  
	long address = syscall(__NR_my_syscall, pid, addr);

	if (address == -1)
		printf("page is not available.\n");
	else
		printf("PID:%d\tPhysical Address:0x%lx.\n", pid, address);
	return 0;
}

//int main (int argc, char* argv[])
