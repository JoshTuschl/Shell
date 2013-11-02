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