//#include <ctype.h>
//#include <errno.h>
#include <getopt.h>
//#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
//#include <sys/types.h>
#include <sys/unistd.h>
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

//stubs
void printTokens();
void systemError(char*);
void executionError(char*);
void writeFile(char*, char*);
char* readFile(char*);
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
	return 1;
}

///////////////////////////////////////////////////////////////////////////

void parser () {	//for infile: open file; get file pointer; dup file; pass it to command		//for outfile:  open file.  output to file?
	int clear;
	for(clear = 0; clear<tokenCount; clear++)  //reset usage for number of tokens to be used
	{
		strcpy(tokenArray[clear].usage, "");
	}
    int infile;
	int outfile;
	//int pipe = 0;
	int meta_char;
	int command=0;
	if(tokenCount < 1)  //check buffer struct is not empty
	{
		//do nothing
		return;
	}
	else
	{   //check for piping
		/*meta_char = lookup('|');  
		if(meta_char != -1)
		{
			//then there is more than one command
			pipe = 2;
		}
		else
		{
			//there is only one command
			pipe = 1;
		}*/

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
			if(outfile < infile)
			{
				printf("Infile direction (<) must come before outfile direction (>)");
			}
			else
			{
				strcpy(tokenArray[outfile].usage, "outfile");
			}
		}
		int i;
		for(i=0; i<tokenCount; i++)  //check each token
		{ 
			if(strcmp(tokenArray[i].tokenType, "#") == 0)  //check for comment symbol
			{
				strcpy(tokenArray[i].usage, "meta");
				int j;
				for(j = i+1; j < tokenCount; j++)
				{
					strcpy(tokenArray[j].usage, "comment");
				}
				break;
			}
			if(strcmp(tokenArray[i].tokenType, "EOL") == 0)
			{
				strcpy(tokenArray[i].usage, "EOL");
				break;
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
					}
					else if(strcmp(tokenArray[i+1].tokenData, "off") == 0)   //check if debug is being turned off
					{	//exit debug mode
						debug = 0;
					}
					else
					{
						executionError("Invalid argument for debug cmd.");
					}
				}
				else if(strcmp(tokenArray[i].tokenType, "chdir") == 0) //check if token is built-in function change directory
				{
					strcpy(tokenArray[i].usage, "cmd");
					command = 1;
					strcpy(tokenArray[i+1].usage, "directory_name");
					//change directory
					chdir(tokenArray[i+1].tokenData);
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
						//int k;
						//for(k = 0; k<strlen(tokenArray[i].tokenData); k++)
						//{
						//	if(tokenArray[i].tokenData[k] == '/')
						//	//if(S_ISREG(tokenArray[i].tokenData.st_mode))
						//	{
						//		strcpy(tokenArray[i].usage, "directory_name");
						//	}
						//}
					}
				}
				else    //otherwise, there is an error
				{
					printf("Parser Error:  Could not determine usage");
				}
			} //end if


			/*------------------------------------------------------------------------*/
			//Handle commands 

			int pid;
			char argv[tokenCount-1];
			int argc = 0;
			char buf[254];
			int l;
			int c = 0;
			char* file;
			char*file2;
			
			//now can execute the function
			if(infile != -1)  
			{
				if(outfile != -1)
				{
					//there is both infile and outfile
					char out[255];
					if((pid = fork()) == 0) {
						if(file = open(tokenArray[infile].tokenData, O_RDWR | O_CREAT) > -1)
						{
							int in = dup(0);
							dup2(file, 0);
							fflush(0);
							close(file);
							dup2(0, in);
						
							if(file2 = open(tokenArray[outfile].tokenData, O_RDWR | O_CREAT) < 0)
							{
								printf("Error opening outfile.");
								return;
							}
							int out = dup(1);
							dup2(file2, 1);
							
							//strcpy(argv[argc], readfile(tokenArray[infile].tokenData));
							//argc++;
							for(l=0; l<tokenCount; l++)
							{
								if(strcmp(tokenArray[l].usage, "cmd") == 0)
								{
									c = l;
								}
							}
							strcpy(argv[0], tokenArray[c].tokenData);
							argc++;
							for(l=0; l<tokenCount; l++)
							{
								if(strcmp(tokenArray[l].usage, "arg") == 0)
								{
									strcpy(argv[argc], tokenArray[l].tokenData);
									argc++;
								}
							}

							if(execve(tokenArray[c].tokenData, argv, environ) < 0) {
								printf("%s: Command not found. \n", tokenArray[0].tokenData);
								close(in);
								close(file2);
							}
							else
							{
								fflush(1);
								close(file2);
								dup2(out, 1);
								close(out);
							}
							close(in);
						}
						else
						{
							printf("Error opening infile.");
						}
					}
				}
				else    //there is only infile
				{
					if((pid = fork()) == 0) {
						if(file = open(tokenArray[infile].tokenData, O_RDWR | O_CREAT) > -1)
						{
							int in = dup(0);
							dup2(file, 0);
							fflush(0);
							close(file);
							dup2(0, in);
						
							//strcpy(argv[argc], readfile(tokenArray[infile].tokenData));
							//argc++;
							for(l=0; l<tokenCount; l++)
							{
								if(strcmp(tokenArray[l].usage, "cmd") == 0)
								{
									c = l;
								}
							}
							strcpy(argv[0], tokenArray[c].tokenData);
							argc++;
							for(l=0; l<tokenCount; l++)
							{
								if(strcmp(tokenArray[l].usage, "arg") == 0)
								{
									strcpy(argv[argc], tokenArray[l].tokenData);
									argc++;
								}
							}
							if(execve(tokenArray[c].tokenData, argv, environ) < 0) {
								printf("%s: Command not found. \n", tokenArray[0].tokenData);
							}
							close(in);
				}
			}
			else
			{
				if(outfile != -1)
				{
					//there is only outfile
					if((pid = fork()) == 0) {
						if(file2 = open(tokenArray[outfile].tokenData, O_RDWR | O_CREAT) > -1)
						{
							int out = dup(1);		
							for(l=0; l<tokenCount; l++)
							{
								if(strcmp(tokenArray[l].usage, "cmd") == 0)
								{
									c = l;
								}
							}
							strcpy(argv[0], tokenArray[c].tokenData);
							argc++;
							for(l=0; l<tokenCount; l++)
							{
								if(strcmp(tokenArray[l].usage, "arg") == 0)
								{
									strcpy(argv[argc], tokenArray[l].tokenData);
									argc++;
								}
							}

							if(execve(tokenArray[c].tokenData, argv, environ) < 0) {
								printf("%s: Command not found. \n", tokenArray[0].tokenData);
								close(file2);
							}
							else
							{
								fflush(1);
								close(file2);
								dup2(out, 1);
								close(out);
							}
						}
				}
				else
				{
					//there is not an infile or an outfile
					for(l=0; l<tokenCount; l++)
					{
						if(strcmp(tokenArray[l].usage, "cmd") == 0)
						{
							c = l;
						}
					}
					strcpy(argv[0], tokenArray[c].tokenData);
					argc++;
					for(l=0; l<tokenCount; l++)
					{
						if(strcmp(tokenArray[l].usage, "arg") == 0)
						{
							strcpy(argv[argc], tokenArray[l].tokenData);
							argc++;
						}
					}
					if((pid = fork()) == 0) {
						if(execve(tokenArray[c].tokenData, argv, environ) < 0) {
							printf("%s: Command not found. \n", tokenArray[0].tokenData);
						}
					}					
				}
			}
		}
	}
	return;
}

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
}

void systemError(char *msg) //system error prints message to standard out and to standard error and exits shell
{
	fprintf(stdout, "%s: %s\n", msg, strerror(errno));
	exit(1);
}


void executionError(char *msg)  //execution error prints message to standard out and exits shell
{
	fprintf(stdout, "%s\n", msg);
	exit(1);
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

void writeFile(char* fileName, char* output) {

	int fd = open(fileName, O_RDWR | O_CREAT);

	write(fd,output, sizeof(output));
	close(fd);
	return;

}

char* readFile(char* fileName) {
	char* fileContents;

	FILE *filePtr;
	int fileSize;
	filePtr = fopen(fileName, "r");

	if (filePtr == NULL) {
	  fprintf(stderr, "Can't open input file %s!\n", fileName);
	  exit(1);
	}

	//file is good
	else {
		//read to end of file
		fseek(filePtr, 0, SEEK_END);
		//find length of file
		fileSize = ftell(filePtr);
		//move file pointer back to beginning
		rewind(filePtr);

		//allocate new char *
		fileContents = (char*) malloc(sizeof(char) * fileSize);
		//read contents of file
		fread(fileContents, 1, fileSize, filePtr);
		fclose(filePtr);
	}
	return fileContents;
}


