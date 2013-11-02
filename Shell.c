#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXPROMPT   128   /* max size of a prompt string */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[MAXPROMPT];     /* command line prompt */
int verbose = 0;            /* if true, print additional output */
char sbuf[MAXLINE];         /* for composing sprintf messages */
char *tokenArray[128];
char *tokenTypeArray[128];
int tokenCount = 0;

/*
 * main - The shell's main routine
 */
int main(int argc, char **argv)
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    strcpy(prompt, "iosh> ");

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    }

    exit(0); /* control never reaches here */
}


int scanner(char* line) {

	tokenCount = 0;

	char newToken = "";
	char newTokenType = "";
	for (i=line; *i; i++) {
		if (line[i] == " ") {
			tokenArray[tokenCount] = *newToken;
			tokenType[tokenCount] = "word";
			tokenCount++;
			newToken = "";
			newTokenType = "";
		}
		else if (line[i] == "<") {
			tokenArray[tokenCount] = *newToken;
			tokenType[tokenCount] = "metachar";
			tokenCount++;
			newToken = "";
			newTokenType = "";
		}
		else if (line[i] == "<") {

		}
		else if (line[i] == "#") {

		}
		else if (line[i] == '"') {
			while (line[i] != '"') {
				newToken
			};
		}
		else if (line[i] == '\n') {

		}
		else {

		}
	}

}
