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
	char tokenData[128];
	char tokenType[128];
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
		fflush(stdout);
		fflush(stdout);
		}

		exit(0); /* control never reaches here */
}


int scanner(const char* cmdline) {

	tokenCount = 0;
	char tokenData[128];
	memset(tokenData, 0, sizeof(tokenData));

	int i;
	for (i=0; i < strlen(cmdline); i++) {
		if (cmdline[i] == ' ') {
			if (strlen(tokenData) > 0) {
				strcat(tokenData, "\0");
				memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
				if (!strcmp(tokenData,"quit")){
					strcpy(tokenArray[tokenCount].tokenType,"quit");
				}
				else if (!strcmp(tokenData,"debug")) {
					strcpy(tokenArray[tokenCount].tokenType,"debug");
				}
				else if (!strcmp(tokenData,"chdir")) {
					strcpy(tokenArray[tokenCount].tokenType,"chdir");
				}
				else if (!strcmp(tokenData,"setprompt")) {
					strcpy(tokenArray[tokenCount].tokenType,"setprompt");
				}
				else {
					strcpy(tokenArray[tokenCount].tokenType,"word");
				}
				memset(tokenData, 0, sizeof(tokenData));
				tokenCount++;
			}
		}
		else if (cmdline[i] == '<') {
			if (strlen(tokenData) > 0) {
				strcat(tokenData, "\0");
				memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
				if (!strcmp(tokenData,"quit")){
					strcpy(tokenArray[tokenCount].tokenType,"quit");
				}
				else if (!strcmp(tokenData,"debug")) {
					strcpy(tokenArray[tokenCount].tokenType,"debug");
				}
				else if (!strcmp(tokenData,"chdir")) {
					strcpy(tokenArray[tokenCount].tokenType,"chdir");
				}
				else if (!strcmp(tokenData,"setprompt")) {
					strcpy(tokenArray[tokenCount].tokenType,"setprompt");
				}
				else {
					strcpy(tokenArray[tokenCount].tokenType,"word");
				}
				memset(tokenData, 0, sizeof(tokenData));
				tokenCount++;
			}
			strcat(tokenData, "<\0");
			memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
			strcpy(tokenArray[tokenCount].tokenType,"meta");
			memset(tokenData, 0, sizeof(tokenData));
			tokenCount++;
		}
		else if (cmdline[i] == '>') {
			if (strlen(tokenData) > 0) {
				strcat(tokenData, "\0");
				memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
				if (!strcmp(tokenData,"quit")){
					strcpy(tokenArray[tokenCount].tokenType,"quit");
				}
				else if (!strcmp(tokenData,"debug")) {
					strcpy(tokenArray[tokenCount].tokenType,"debug");
				}
				else if (!strcmp(tokenData,"chdir")) {
					strcpy(tokenArray[tokenCount].tokenType,"chdir");
				}
				else if (!strcmp(tokenData,"setprompt")) {
					strcpy(tokenArray[tokenCount].tokenType,"setprompt");
				}
				else {
					strcpy(tokenArray[tokenCount].tokenType,"word");
				}
				memset(tokenData, 0, sizeof(tokenData));
				tokenCount++;
			}
			strcat(tokenData, ">\0");
			memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
			strcpy(tokenArray[tokenCount].tokenType,"meta");
			memset(tokenData, 0, sizeof(tokenData));
			tokenCount++;
		}
		else if (cmdline[i] == '#') {
			if (strlen(tokenData) > 0) {
				strcat(tokenData, "\0");
				memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
				if (!strcmp(tokenData,"quit")){
					strcpy(tokenArray[tokenCount].tokenType,"quit");
				}
				else if (!strcmp(tokenData,"debug")) {
					strcpy(tokenArray[tokenCount].tokenType,"debug");
				}
				else if (!strcmp(tokenData,"chdir")) {
					strcpy(tokenArray[tokenCount].tokenType,"chdir");
				}
				else if (!strcmp(tokenData,"setprompt")) {
					strcpy(tokenArray[tokenCount].tokenType,"setprompt");
				}
				else {
					strcpy(tokenArray[tokenCount].tokenType,"word");
				}
				memset(tokenData, 0, sizeof(tokenData));
				tokenCount++;
			}
			strcat(tokenData, "#\0");
			memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
			strcpy(tokenArray[tokenCount].tokenType,"meta");
			memset(tokenData, 0, sizeof(tokenData));
			tokenCount++;

		}
		else if (cmdline[i] == '"') {
			tokenData[0] = cmdline[i+1];
			i++;
			i++;
			while (cmdline[i] != '"') {
				int index = strlen(tokenData);
				tokenData[index] = cmdline[i];
				i++;
			};
			memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
			strcpy(tokenArray[tokenCount].tokenType,"string");
			memset(tokenData, 0, sizeof(tokenData));
			tokenCount++;
		}
		else if (cmdline[i] == '\n') {
			if (!strcmp(tokenData,"quit")){
				strcat(tokenData, "\0");
				memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
				//tokenArray[tokenCount].tokenData = tokenData;
				strcpy(tokenArray[tokenCount].tokenType,"quit");
				memset(tokenData, 0, sizeof(tokenData));
				tokenCount++;
			}
			else if (!strcmp(tokenData,"debug")) {
				strcat(tokenData, "\0");
				memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
				strcpy(tokenArray[tokenCount].tokenType,"debug");
				memset(tokenData, 0, sizeof(tokenData));
				tokenCount++;
			}
			else if (!strcmp(tokenData,"chdir")) {
				strcat(tokenData, "\0");
				memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
				strcpy(tokenArray[tokenCount].tokenType,"chdir");
				memset(tokenData, 0, sizeof(tokenData));
				tokenCount++;
			}
			else if (!strcmp(tokenData,"setprompt")) {
				strcat(tokenData, "\0");
				memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
				strcpy(tokenArray[tokenCount].tokenType,"setprompt");
				memset(tokenData, 0, sizeof(tokenData));
				tokenCount++;
			}
			else {
				if (strlen(tokenData) > 0) {
					strcat(tokenData, "\0");
					memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
					strcpy(tokenArray[tokenCount].tokenType,"word");
					memset(tokenData, 0, sizeof(tokenData));
					tokenCount++;
				}

			}
			memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
			strcpy(tokenArray[tokenCount].tokenType,"EOL");
			memset(tokenData, 0, sizeof(tokenData));
			tokenCount++;
		}
		else {
			int index = strlen(tokenData);
			if (tokenData[0] != '\0'){
				tokenData[index] = cmdline[i];
			}
			else {
				tokenData[0] = cmdline[i];
			}
		}
	}
}

void setPrompt(char string) {
	strcpy(prompt, string);
}

void setDebug(int value) {
	debug = value;
}

void printTokens() {
	int i;
	for (i = 0; i < tokenCount; i++) {
		printf("Token is %s ", (tokenArray[i]).tokenData);
		printf("Token type is %s\n", (tokenArray[i]).tokenType);
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

