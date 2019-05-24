/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * gsh.c: implementing the shell's interface functions
*/

#include "gsh.h"
#include "parser.h"
#include "internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

// Maximum amount of commands that will be executed
#define MAX_COMMANDS 5
#define MAX_ARGS     3

struct gsh
{
	int isRunning;
	int childs;
	// sigset_t newmask;
};

struct gsh shell;

// Defining signal handlers:

// Used to awake the shell when the foreground program terminates:
void SIGUSR1_Handler() {}

// Used to controll the number of alive children:
void SIGCHLD_Handler()
{
	shell.childs -= 1;
}

// Ctrl + c handler:
void SIGINT_Handler()
{
	// Ctrl + c only works if there are no living children:
	if(shell.childs)
	{
		fprintf(stderr, "\nOOPS :O... Looks like i still have childs alive so i can't die. (So responsible! :D)\n");
		printf("gsh> ");
	}
	else
	{
		raise(SIGKILL);
	}
}

// Ctrl + z handler:
void SIGTSTP_Handler()
{
	// Stopping all children:
	kill(0,SIGTSTP);
}

/**
 * Defining function to execute the program creator/controller program:
 * 
 * Inputs: all the line arguments that where cought in the user input.
 * 
 * Side effects: creates a child process responsible to execute the requested
 * programs.
*/
static int GSH_Controller(char *args[]);

int GSH_Init()
{
	// Welcome message:
	printf("Welcome to gsh: the Linux Group SHell :D\n");
	printf("Written by Alan Diniz, Rafael Belmock and Israel Santos.\n\n");
	
	shell.childs = 0;
	shell.isRunning = 1; // The shell is now running

	setpgid(0,0); // The shell is lider of a new group

	signal(SIGINT, SIGINT_Handler);
	signal(SIGTSTP, SIGTSTP_Handler);
	signal(SIGUSR1, SIGUSR1_Handler);
	signal(SIGCHLD, SIGCHLD_Handler);
	
	return 1;
}

int GSH_IsRunning()
{
	return shell.isRunning;
}

void GSH_ReadAndExecute()
{
	printf("gsh> ");

	// Getting the command tokens:
	char *line = GSH_ReadLine();
	char **tokens = GSH_SplitLine(line);

	// Iterating through the tokens:
	int i = 0, commands = 1;
	int internal = 0; // Tells if current command is an internal operation
	
	while (tokens[i] != NULL && !internal)
	{
		// Ignoring operator '->':
		if (strncmp(tokens[i], "->", 2) == 0)
		{
			commands++;
			// Getting the next token:
			i++;
			// Cheking if there is a identation error:
			if(tokens[i] == NULL)
			{
				fprintf(stderr, "OOPS :O... missing second argument of operator ->.\n");
				break;
			}
		}

		// If the current command is still the same:
		// Checking if the command is an internal operation:
		if (strncmp(tokens[i], "exit", 4) == 0) // Exit command
		{
			internal = 1;
			if (commands == 1 && tokens[i+1] == NULL && GSH_Exit())
			{
				shell.isRunning = 0;
				break;
			}
			else
			{
				fprintf(stderr, "OOPS :O... exit command wasn't used properlly.\n");
				fprintf(stderr, "Try executing it on it's own and it'll probably work. :D\n");
			}
			
		}
		else if (strncmp(tokens[i], "mywait", 6) == 0) // MyWait command
		{
			internal = 1;
			if (commands == 1 && tokens[i+1] == NULL)
			{
				GSH_MyWait();
			}
			else
			{
				fprintf(stderr, "OOPS :O... mywait command wasn't used properlly.\n");
				fprintf(stderr, "Try executing it on it's own and it'll probably work. :D\n");
			}
			
		}

		// Going to the next token:
		i++;
	}
	// Passing the arguments to the controller program:
	if (!internal) GSH_Controller(tokens);

	free(tokens);
	free(line);
}

void GSH_Finish()
{
	// Use this to free memory if necessary
}

static int GSH_Controller(char *args[])
{
	shell.childs += 1;

	// Protecting the shell of being interrupted while waiting for the foreground process:
	signal(SIGINT, SIG_IGN);

	// Creating a mask to block SIGUSR1:
	sigset_t newmask, oldmask;
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	// Creating a child process that will control the requested programs:
	pid_t pid = fork();

	// If it is the child:
	if (pid == 0)
	{
		int success = execv("./bin/controller", args);
		if (success == -1) goto proc_error;
	}
	else if (pid > 0)
	{
		// Waiting for the termination of the foreground process:
		sigsuspend(&oldmask);
		// Restoring old signal mask and handlers:
		sigprocmask(SIG_SETMASK, &oldmask, NULL);
		signal(SIGINT, SIGINT_Handler);
	}
	else goto proc_error;
	
	return 1;

	proc_error:
	fprintf(stderr, "OOPS :O... Looks like there was an error while trying to execute the given commands.\n");
	exit(1);
}
