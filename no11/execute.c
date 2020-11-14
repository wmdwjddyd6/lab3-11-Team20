#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

//명령어 실행 함수
int execute(int ac, char *av[], int sourcefd, char *sourcefile, int destfd, char *destfile, BOOLEAN append, BOOLEAN backgrnd) {
	int pid;

	if (ac == 0 || shellcmd(ac, av, sourcefd, destfd)) {
		return 0;
	}

	pid = fork();

	//error 발생
	if (pid == ERROR) { 
		fprintf(stderr, "Cannot create new process.\n");
		return 0;
	}

	//자식
	else if (pid == 0) {
		redirect(sourcefd, sourcefile, destfd, destfile, append, backgrnd);
		execvp(av[0], av);
		fprintf(stderr, "Cannot execute %s\n", av[0]);
		exit(0);
	}

	//부모
	else {
		//읽기 닫기
		if(sourcefd > 0 && close(sourcefd) == ERROR) {
			syserr("close sourcefd");
		}
		//쓰기 닫기
		if(destfd > 1 && close(destfd) == ERROR) {
			syserr("close destfd");
		}
		//background경우
		if(backgrnd) {
			printf("%d\n", pid);
		}
		return pid;
	}
}
