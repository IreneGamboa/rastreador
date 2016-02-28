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

#define VERBOSE_PAUSE "-V" 
#define VERBOSE      "-v"

void initialize(void);
void execute(char *comand, char **argv);
void trace(pid_t child, bool verbose, bool pause);
void printV(char *message, long register_f, bool verbose);
void result(void)
void result(void)
