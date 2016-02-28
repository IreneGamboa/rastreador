#include "rastreador.h"

int main(int argc, char **argv){
	bool verbose_pause;
	bool verbose;
	long orig_eax;
	long eax;
	pid_t hijo;
	
	if(strcmp(argv[1], "-v") == 0){
		verbose = true;
		verbose_pause = false;
	}else if(strcmp(argv[1], "-V") == 0){
		verbose = true;
		verbose_pause = true;
	}else{
		verbose = false;
		verbose_pause = false;
	}
	
	hijo = fork();
	
	if(hijo == 0){
		if(strcmp(argv[1], "-v") || strcmp(argv[1], "-V")){
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

void trace(pid_t hijo, bool verbose, bool verbose_pause){
	bool running = true;
	bool syscall = true;
	bool first = false;
	long orig_rax;
		
	while(1){
		wait(NULL);
		orig_rax = ptrace(PTRACE_PEEKUSER, hijo, SPACE, NULL);
		
		if(orig_rax == -1){
			printV("Saliendo de la ejecución de System Call\n", 01, verbose);
			break;
		}else{
			if(syscall && first){
				printV("Se hizo una llamada al sistema %ld \n", orig_rax, verbose);
				syscall = false;
				sum_syscalls[orig_rax]++;
			}else if(!syscall && first){
				printV("Saliendo de la llamada %s \n", orig_rax, verbose);
				syscall = true;
			}else{
				first = true;
			}
			ptrace(PTRACE_SYSCALL, hijo, NULL, NULL);
			if(verbose_pause){
				printf("Presione Enter para continuar\n");
				getchar();
			}
		}
	}	
}

void printV(char* message, long current_register, bool verbose){
	if(verbose){
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
			printf("|Código: %d\t|\tContador: %d\t|", count, sum_syscalls[count]);
		}
		count++;
	}
}


