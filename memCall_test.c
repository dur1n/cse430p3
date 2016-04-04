#include <linux/unistd.h>
#include <linux/stdio.h>

#define __NR_my_syscall 359

main(int argc, char *argv[]) {
{
    int addr = atoi(argv[1]);  // Converting a string argument to an integer 
    int arg2 = atoi(argv[2]);  
	long address = syscall(__NR_my_syscall, pid, addr);

	if (address == -1)
		printf("page is not available.\n");
	else
		printf("address is %ld.", address);

	return 0;
}

//int main (int argc, char* argv[])
