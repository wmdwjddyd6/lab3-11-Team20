#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "common.h"
//오류처리.
void fatal(char *message) {
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}

void syserr(char *message) {
	fprintf(stderr, "Error: %s (%d", message, errno);
	exit(1);
}

//자식의 실행이 끝나길 기다리는 함수.
void waitfor(int pid) {
	int wpid, status;
	while ((wpid = wait(&status)) != pid && wpid != ERROR);
}

//프롬프트 출력.
void print_prompt() {
	printf("Team20 >> ");

}

//명령 처리.
BOOLEAN shellcmd(int ac, char *av[], int sourcefd, int destfd) {
	char *path;

	if(!strcmp(av[0], "exit")) {
		cmd_exit();
	}
	else if(!strcmp(av[0], "ls")) {
		cmd_ls(ac, av);
	}
	else if(!strcmp(av[0], "pwd")) {
		cmd_pwd();
	}
	else if(!strcmp(av[0], "cd")) {
		cmd_cd(ac, av);
	}
	else if(!strcmp(av[0], "mkdir")) {
		cmd_mkdir(ac, av);
	}
	else if(!strcmp(av[0], "rmdir")) {
		cmd_rmdir(ac, av);
	}
	else if(!strcmp(av[0], "cp")) {
		cmd_cp(ac, av);
	}
	else if(!strcmp(av[0], "rm")) {
		cmd_rm(ac, av);
	}
	else if(!strcmp(av[0], "mv")) {
		cmd_mv(ac, av);
	}
	else if(!strcmp(av[0], "cat")) {
		cmd_cat(ac, av);
	}
	else if(!strcmp(av[0], "ln")) {
		cmd_ln(av[1], av[2]);
	}
	else {
		return FALSE;
	}

	if (sourcefd != 0 || destfd != 1) {
		fprintf(stderr, "Ilegal redirection or pipeline.\n");
	}
	return TRUE;
}
