#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

//명령어 파싱
SYMBOL parse(int *waitpid, BOOLEAN makepipe, int *pipefdp) {
	SYMBOL symbol, term;
	int argc, sourcefd, destfd;
	int pid, pipefd[2];
	char *argv[MAXARG+1], sourcefile[MAXFNAME];
	char destfile[MAXFNAME];
	char word[MAXWORD];
	BOOLEAN append;

	argc = 0;			
	sourcefd = 0;		
	destfd = 1;			

	
	while (TRUE) {
		//입력 단어 분석.
		switch (symbol = getsymbol(word)) {
			//일반문자의경우.
			case S_WORD :			
				if(argc == MAXARG) {
					fprintf(stderr, "Too many args.\n");
					break;
				}

				argv[argc] = (char *) malloc(strlen(word)+1);
				
				if(argv[argc] == NULL) {
					fprintf(stderr, "Out of arg memory.\n");
					break;
				}

				strcpy(argv[argc], word);
				argc++;
				continue;
			// < 경우.
			case S_LT   : 

				if(makepipe) {
					fprintf(stderr, "Extra <.\n");
					break;
				}

				if(getsymbol(sourcefile) != S_WORD) {
					fprintf(stderr, "Illegal <.\n");
					break;
				}

				sourcefd = BADFD;
				continue;
			// > 경우.
			case S_GT   :
			case S_GTGT :

				if(destfd != 1) {
					fprintf(stderr, "Extra > or >>.\n");
					break;
				}

				if(getsymbol(destfile) != S_WORD) {
					fprintf(stderr, "Illegal > or >>.\n");
					break;
				}

				destfd = BADFD;
				append = (symbol == S_GTGT);
				continue;
			//|, &, 줄바꿈의 경우.
			case S_BAR  :
			case S_AMP  :
			case S_NL   :
			
				argv[argc] = NULL;

				if(symbol == S_BAR) {
					if(destfd != 1) {
						fprintf(stderr, "> or >> conflicts with |.\n");
						break;
					}

					term = parse(waitpid, TRUE, &destfd);
				}

				else {
					term = symbol;
				}

				if (makepipe) {
					if (pipe(pipefd) == ERROR){
						syserr("pipe");
					}
					*pipefdp = pipefd[1];
					sourcefd = pipefd[0];
				}

				pid = execute(argc, argv, sourcefd, sourcefile, destfd, destfile, append, term == S_AMP);

				if (symbol != S_BAR) {
					*waitpid = pid;
				}

				if (argc == 0 && (symbol != S_NL || sourcefd > 1)) {
					fprintf(stderr, "Missing command.\n");
				}

				while (--argc >= 0) {
					free(argv[argc]);
				}

				return term;
			//명령이 잘못된 경우 종료.
			case S_EOF : 
				exit(0);
		} 
	}    
}
