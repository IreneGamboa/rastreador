#include "rastreador.h"

int main(int argc, char **argv){
	int verbose_pause;
	int verbose;
	long orig_eax;
	long eax;
	pid_t hijo;
	
	if(strcmp(argv[1], "-v") == 0){
		verbose = 1;
		verbose_pause = 0;
	}else if(strcmp(argv[1], "-V") == 0){
		verbose = 1;
		verbose_pause = 1;
	}else{
		verbose = 0;
		verbose_pause = 0;
	}
	
	hijo = fork();
	
	if(hijo == 0){
		if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-V") == 0){
			copyArgs(2, argc, argv);
		}else{
			copyArgs(1, argc, argv);
		}
		
	}else{
		trace(hijo, verbose, verbose_pause);
		result();
	}
	return 0;
}

void copyArgs(int value, int argc, char **argv){
	char **args = malloc(argc * sizeof(char*));
	for(int i = value; i < argc; i++){
		args[i-value] = malloc(strlen(argv[i])+1);
		strcpy(args[i-value],argv[i]);
	}
	args[argc-1] = 0;
	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	execve(args[0], &args[0], NULL);
}

void trace(pid_t hijo, int verbose, int verbose_pause){
	int running = 1;
	int syscall = 1;
	int first = 0;
	long orig_rax;
		
	while(1){
		wait(NULL);
		orig_rax = ptrace(PTRACE_PEEKUSER, hijo, SPACE, NULL);
		
		if(orig_rax == -1){
			printV("Saliendo de la ejecución de System Call\n", 0, verbose);
			break;
		}else{
			if(syscall == 1  && first == 1){
				printV("Se hizo una llamada al sistema %ld \n", orig_rax, verbose);
				syscall = 0;
				sum_syscalls[orig_rax]++;
			}else if(!syscall == 1 && first == 1){
				printV("Saliendo de la llamada %ld \n", orig_rax, verbose);
				syscall = 1;
			}else{
				first = 1;
			}
			ptrace(PTRACE_SYSCALL, hijo, NULL, NULL);
			if(verbose_pause == 1){
				printf("Presione Enter para continuar\n");
				getchar();
			}
		}
	}	
}

void printV(char* message, long current_register, int verbose){
	if(verbose == 1){
		printf(message, current_register);
	}
}

void result(){
	#ifdef __i386__
		int max = 338;
	#else
		int max = 323;
	#endif
	
	int count = 0;
	while(count<max){
		if(sum_syscalls[count]>0){
			printf("|Código: %d\t---\tContador: %d\t--", count, sum_syscalls[count]);
		}
		count++;
	}
}


