//#include <ctype.h>
//#include <errno.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/_default_fcntl.h>
#include <sys/errno.h>
//#include <sys/reent.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/wait.h>

//#include <sys/wait.h>
//#include <unistd.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXPROMPT   128   /* max size of a prompt string */


struct token {
	char tokenData[128];
	char tokenType[128];
	char usage[128];
};

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[MAXPROMPT];     /* command line prompt */
int verbose = 0;            /* if true, print additional output */
int debug = 0;
int tokenCount = 0;
struct token tokenArray[128];
int NOTFOUND = 999;

//stubs
void printTokens();
void systemError(char*);
void executionError(char*);
int lookUp(char);
void parser();

/*
 * main - The shell's main routine
 */
int main(int argc, char **argv)
{
    char cmdline[MAXLINE];

    strcpy(prompt, "iosh> ");

    /* Execute the shell's read/eval loop */
    while (1) {

		printf("%s", prompt);
		fflush(stdout);

		if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
			executionError("fgets error");

		if (feof(stdin)) { /* End of file (ctrl-d) */
			fflush(stdout);
			exit(0);
		}

		/* Evaluate the command line */
		scanner(cmdline);
		parser();
		fflush(stdout);
		fflush(stdout);
		if (debug == 1) {
			printTokens();
		}
	}

		exit(0); /* control never reaches here */
}

///////////////////////////////////////////////////////////////////////////

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
				if (!strcmp(tokenData,"quit") || !strcmp(tokenData,"exit")){
					strcpy(tokenArray[tokenCount].tokenType,"quit");
				}
				else if (!strcmp(tokenData,"debug")) {
					strcpy(tokenArray[tokenCount].tokenType,"debug");
				}
				else if (!strcmp(tokenData,"chdir")|| !strcmp(tokenData,"cd")) {
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
		else if (cmdline[i] == '<' || cmdline[i] == '>' || cmdline[i] == '#') {
			if (strlen(tokenData) > 0) {
				strcat(tokenData, "\0");
				memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
				if (!strcmp(tokenData,"quit") || !strcmp(tokenData,"exit")){
					strcpy(tokenArray[tokenCount].tokenType,"quit");
				}
				else if (!strcmp(tokenData,"debug")) {
					strcpy(tokenArray[tokenCount].tokenType,"debug");
				}
				else if (!strcmp(tokenData,"chdir")|| !strcmp(tokenData,"cd")) {
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
			strcat(tokenData, &cmdline[i]);
			memcpy(tokenArray[tokenCount].tokenData, tokenData, sizeof(tokenData));
			strcpy(tokenArray[tokenCount].tokenType,"meta");
			memset(tokenData, 0, sizeof(tokenData));
			tokenCount++;
		}
		else if (cmdline[i] == '"') {
			tokenData[0] = cmdline[i+1];
			i++;
			i++;
			while (cmdline[i] != '"' && cmdline[i] != '\n') {
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
			if (!strcmp(tokenData,"quit") || !strcmp(tokenData,"exit")){
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
			else if (!strcmp(tokenData,"chdir")|| !strcmp(tokenData,"cd")) {
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
	return 1;
}

///////////////////////////////////////////////////////////////////////////

void parser () {	//for infile: open file; get file pointer; dup file; pass it to command		//for outfile:  open file.  output to file?
	int clear;
	for(clear = 0; clear<tokenCount; clear++)  //reset usage for number of tokens to be used
	{
		strcpy(tokenArray[clear].usage, "");
	}
    int infile = 999;
	int outfile = 999;
	int meta_char;
	int command=0;
	if(tokenCount > 1) //check buffer struct is not empty
	{
	    //check for infile
		meta_char = lookUp('<');
		if(meta_char != -1)
		{
			strcpy(tokenArray[meta_char].usage, "meta");
			infile = meta_char -1;
			strcpy(tokenArray[infile].usage, "infile");
		}

		//check for outfile
		meta_char = lookUp('>');
		if(meta_char != -1)
		{
			strcpy(tokenArray[meta_char].usage, "meta");
			outfile = meta_char + 1;
			if(outfile < infile && infile != 999)
			{
				printf("Infile direction (<) must come before outfile direction (>)\n");
			}
			else
			{
				strcpy(tokenArray[outfile].usage, "outfile");
			}
		}
		int i = 0;//check each token
		for(i; i < tokenCount; i++)
		{ 
			if(tokenArray[i].tokenData[0] == '#')  //check for comment symbol
			{
				strcpy(tokenArray[i].usage, "meta");
				int j;
				for(j = i+1; j < tokenCount; j++)
				{
					strcpy(tokenArray[j].usage, "comment");
				}
			}
			if(strcmp(tokenArray[i].tokenType, "EOL") == 0)
			{
				strcpy(tokenArray[i].usage, "EOL");
			}
			//first check for built-in commands and execute them immediately
			if(strcmp(tokenArray[i].usage, "") == 0)  //if usage not assigned, check for usage
			{	
				if(strcmp(tokenArray[i].tokenType, "setprompt") == 0)
				{
					strcpy(tokenArray[i].usage, "cmd");
					command = 1;
					strcpy(tokenArray[i+1].usage, "arg");
					//set the prompt
					strcpy(prompt, tokenArray[i+1].tokenData);
					strcat(prompt, ">");
					return;
				}
				else if(strcmp(tokenArray[i].tokenType, "debug") == 0)  //check if token is built-in function debug
				{
					strcpy(tokenArray[i].usage, "cmd");
					command = 1;
					strcpy(tokenArray[i+1].usage, "arg");
					if(strcmp(tokenArray[i+1].tokenData, "on") == 0)  //check if debug is being turned on
					{
						//enter debug mode
						debug = 1;
						strcpy(tokenArray[i+2].usage, "EOL");
						return;
					}
					else if(strcmp(tokenArray[i+1].tokenData, "off") == 0)   //check if debug is being turned off
					{	//exit debug mode
						debug = 0;
						strcpy(tokenArray[i+2].usage, "EOL");
						return;
					}
					else
					{
						executionError("Invalid argument for debug cmd.");
						return;
					}
				}
				else if(strcmp(tokenArray[i].tokenType, "chdir") == 0) //check if token is built-in function change directory
				{
					strcpy(tokenArray[i].usage, "cmd");
					command = 1;
					strcpy(tokenArray[i+1].usage, "directory_name");
					//change directory
					char* ptr = tokenArray[i+1].tokenData;
					if (chdir(ptr) > -1)
					{
						printf("Directory changed to %s\n", tokenArray[i+1].tokenData);
						return;
					}
					else
					{
						fprintf(stdout, "%s: %s\n", "Directory change failed", strerror(errno));
						return;
					}
				}
				else if(strcmp(tokenArray[i].tokenType, "quit") == 0) //check if token is quit
				{
					strcpy(tokenArray[i].usage, "cmd");
					command = 1;
					//quit by exiting the shell
					exit(0);
				}
				else if(strcmp(tokenArray[i].tokenType, "string") == 0)  //if type is a string, must be an argument
				{
					strcpy(tokenArray[i].usage, "arg");
				}
				else if(strcmp(tokenArray[i].tokenType, "word") == 0)  //if type is word, determine cmd, arg, or dir
				{
					if(command == 0) //check if there is command yet (true or false)
					{
						strcpy(tokenArray[i].usage, "cmd");
						command = 1; //command is true
					}
					else
					{
						strcpy(tokenArray[i].usage, "arg");
					}
				}
				else    //otherwise, there is an error
				{
					printf("Parser Error:  Could not determine usage\n");
				}
			} //end if(strcmp(tokenArray[i].usage, "") == 0)
		}//end for(i; i < tokenCount; i++)

		/*------------------------------------------------------------------------*/
		//Handle commands

		int pid;
		char* args[tokenCount];
		int argCount = 0;
		int l;
		int file;
		int file2;

		//Build arguments array for exec call
		for(l=0; l<tokenCount; l++)
		{
			//search for command and add to start of array
			if(strcmp(tokenArray[l].usage, "cmd") == 0)
			{
				args[0] = tokenArray[l].tokenData;
			}
		}
		argCount++;
		for(l=0; l<tokenCount; l++)
		{
			//add arguments
			if(strcmp(tokenArray[l].usage, "arg") == 0)
			{
				args[argCount] = tokenArray[l].tokenData;
				argCount++;
			}
		}
		//add terminator to array
		args[argCount] = NULL;

		//now can execute the function
		if((pid = fork()) == 0)
		{
			if (infile < NOTFOUND)
			{
				if((file = open(tokenArray[infile].tokenData, O_RDONLY)) > -1)
				{
					dup2(file, 0);
				}
				else
				{
					systemError("Failure");
				}
			}

			if (outfile < NOTFOUND)
			{
				if((file2 = open(tokenArray[outfile].tokenData, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR)) > -1)
				{
					dup2(file2, 1);
				}
				else
				{
					systemError("Failure");
				}
			}

			if(execvp(args[0], args) < 0)
			{
				systemError("Failure");
			}
			else
			{
				//should exec before getting here
				exit(1);
			}
		} //if((pid = fork()) == 0)
		else
		{
			int status;
			waitpid(pid, &status, 0);
		}
	} //if(tokenCount > 1)
	return;
} // end of function

////////////////////////////////////////////////////////////////////

void setPrompt(char* string) { //pass string to set prompt to
	strcpy(prompt, string);	//sets prompt to string
}

void setDebug(int value) {
	debug = value;
}

void printTokens() { //for debug
	int i;
	for (i = 0; i < tokenCount; i++) {
		printf("Token is %s \n", (tokenArray[i]).tokenData);
		printf("Token type is %s\n", (tokenArray[i]).tokenType);
		printf("Token usage is %s\n\n", (tokenArray[i]).usage);
	}
	sleep(1);
}

void systemError(char *msg) //system error prints message to standard out and to standard error and exits shell
{
	fprintf(stdout, "%s: %s\n", msg, strerror(errno));
	exit(1);
}


void executionError(char *msg)  //execution error prints message to standard out and exits shell
{
	fprintf(stdout, "%s\n", msg);
}

int lookUp(char x)  //pass character to look up or find in tokens
{
	int found = -1;
	int i;
	for(i=0; i<tokenCount; i++)
	{
		if(tokenArray[i].tokenData[0] == x)
		{
			found = i;
		}
	}
	return found;  //returns index of found token
}




