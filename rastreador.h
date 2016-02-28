#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>

#ifdef __i386__
	#include <linux/user.h>
#else
	#include <sys/reg.h> 
#endif

typedef enum {false, true} bool;

#ifdef __i386__
	#define SPACE 4 * ORIG_EAX
#else
	#define SPACE 8 * ORIG_RAX 
#endif


#ifdef __i386__
		int syscalls[338];
		
#else
		int syscalls[323];
#endif


void trace(pid_t hijo, bool verbose, bool verbose_pause);
void printV(char* message, long current_register, bool verbose);
void result(void);
void copyArgs(int value, int argc, char **argv);
