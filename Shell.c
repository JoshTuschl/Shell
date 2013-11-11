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

struct token {
	char* tokenData;
	char* tokenType;
};

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[MAXPROMPT];     /* command line prompt */
int verbose = 0;            /* if true, print additional output */
char sbuf[MAXLINE];         /* for composing sprintf messages */
int debug = 0;
int tokenCount = 0;
struct token tokenArray[128];

//stubs

void printTokens();
void helpMessage(void);
void system_error(char*);
void execution_error(char*);


/*
 * main - The shell's main routine
 */
int main(int argc, char **argv)
{
    char c;
    char cmdline[MAXLINE];
    int enable_prompt = 1;

    strcpy(prompt, "iosh> ");

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            helpMessage();
            break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
            break;
        case 'p':             /* don't print a prompt */
            enable_prompt = 0;  /* handy for automatic testing */
            break;
        default:
            helpMessage();
        }
    }

    /* Execute the shell's read/eval loop */
    while (1) {

		/* Read command line */
		if (enable_prompt) {
			printf("%s", prompt);
			fflush(stdout);
		}

		if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
			execution_error("fgets error");

		if (feof(stdin)) { /* End of file (ctrl-d) */
			fflush(stdout);
			exit(0);
		}

		/* Evaluate the command line */
		scanner(cmdline);
		printTokens();
		//eval(cmdline);
		fflush(stdout);
		fflush(stdout);
		}

		exit(0); /* control never reaches here */
}


int scanner(const char* cmdline) {

	tokenCount = 0;
	struct token newToken;

	int i;
	for (i=0; i < strlen(cmdline); i++) {
		if (cmdline[i] == ' ') {
			if (strlen(newToken.tokenData) > 0) {
				strcat(newToken.tokenData, "\0");
				tokenArray[tokenCount].tokenData = newToken.tokenData;
				tokenArray[tokenCount].tokenType = "word";
				newToken.tokenData = "";
				newToken.tokenType = "";
				tokenCount++;
			}
		}
		else if (cmdline[i] == '<') {
			if (strlen(newToken.tokenData) > 0) {
				strcat(newToken.tokenData, "\0");
				tokenArray[tokenCount].tokenData = newToken.tokenData;
				tokenArray[tokenCount].tokenType = "word";
				newToken.tokenData = "";
				newToken.tokenType = "";
				tokenCount++;
			}
			strcat(newToken.tokenData, "<\0");
			tokenArray[tokenCount].tokenData = newToken.tokenData;
			tokenArray[tokenCount].tokenType = "meta";
			newToken.tokenData = "";
			newToken.tokenType = "";
			tokenCount++;
		}
		else if (cmdline[i] == '>') {
			if (strlen(newToken.tokenData) > 0) {
				strcat(newToken.tokenData, "\0");
				tokenArray[tokenCount].tokenData = newToken.tokenData;
				tokenArray[tokenCount].tokenType = "word";
				newToken.tokenData = "";
				newToken.tokenType = "";
				tokenCount++;
			}
			strcat(newToken.tokenData, ">\0");
			tokenArray[tokenCount].tokenData = newToken.tokenData;
			tokenArray[tokenCount].tokenType = "meta";
			newToken.tokenData = "";
			newToken.tokenType = "";
			tokenCount++;
		}
		else if (cmdline[i] == '#') {
			if (strlen(newToken.tokenData) > 0) {
				strcat(newToken.tokenData, "\0");
				tokenArray[tokenCount].tokenData = newToken.tokenData;
				tokenArray[tokenCount].tokenType = "word";
				newToken.tokenData = "";
				newToken.tokenType = "";
				tokenCount++;
			}
			strcat(newToken.tokenData, "#\0");
			tokenArray[tokenCount].tokenData = newToken.tokenData;
			tokenArray[tokenCount].tokenType = "meta";
			newToken.tokenData = "";
			newToken.tokenType = "";
			tokenCount++;

		}
		else if (cmdline[i] == '"') {
			while (cmdline[i] == '"') {
				strcat(newToken.tokenData, &cmdline[i]);
				i++;
			};
			tokenArray[tokenCount].tokenData = newToken.tokenData;
			tokenArray[tokenCount].tokenType = "string";
			newToken.tokenData = "";
			newToken.tokenType = "";
			tokenCount++;
		}
		else if (cmdline[i] == '\n') {
			if (strcmp(newToken.tokenData,"quit")){
				strcat(newToken.tokenData, "\0");
				tokenArray[tokenCount].tokenData = newToken.tokenData;
				tokenArray[tokenCount].tokenType = "quit";
				newToken.tokenData = "";
				newToken.tokenType = "";
				tokenCount++;
			}
			else if (strcmp(newToken.tokenData,"debug")) {
				strcat(newToken.tokenData, "\0");
				tokenArray[tokenCount].tokenData = newToken.tokenData;
				tokenArray[tokenCount].tokenType = "debug";
				newToken.tokenData = "";
				newToken.tokenType = "";
				tokenCount++;
			}
			else if (strcmp(newToken.tokenData,"chdir")) {
				strcat(newToken.tokenData, "\0");
				tokenArray[tokenCount].tokenData = newToken.tokenData;
				tokenArray[tokenCount].tokenType = "chdir";
				newToken.tokenData = "";
				newToken.tokenType = "";
				tokenCount++;
			}
			else if (strcmp(newToken.tokenData,"setprompt")) {
				strcat(newToken.tokenData, "\0");
				tokenArray[tokenCount].tokenData = newToken.tokenData;
				tokenArray[tokenCount].tokenType = "setprompt";
				newToken.tokenData = "";
				newToken.tokenType = "";
				tokenCount++;
			}
			else {
				if (strlen(newToken.tokenData) > 0) {
					strcat(newToken.tokenData, "\0");
					tokenArray[tokenCount].tokenData = newToken.tokenData;
					tokenArray[tokenCount].tokenType = "word";
					newToken.tokenData = "";
					newToken.tokenType = "";
					tokenCount++;
				}

			}
			tokenArray[tokenCount].tokenData = newToken.tokenData;
			tokenArray[tokenCount].tokenType = "EOL";
			newToken.tokenData = "";
			newToken.tokenType = "";
			tokenCount++;
		}
		else {
			const char* character = &cmdline[i];
			strcat(newToken.tokenData, character);
		}
	}
}

void printTokens() {
	int i;
	for (i = 0; i < tokenCount; i++) {
		struct token curr = tokenArray[i];
		printf("Token is %s ", curr.tokenData);
		printf("Token type is %s\n", curr.tokenType);
	}
}


void helpMessage(void)
{
	printf("Usage: shell [-hvp]\n");
	printf("   -h   print help message\n");
	printf("   -v   verbose printing enabled\n");
	printf("   -p   command prompt off\n");
	exit(1);
}


void system_error(char *msg)
{
	fprintf(stdout, "%s: %s\n", msg, strerror(errno));
	exit(1);
}


void execution_error(char *msg)
{
	fprintf(stdout, "%s\n", msg);
	exit(1);
}

