/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * controller.c: implementing the process controller functionalities
*/

#include "controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_COMMANDS 5
#define MAX_ARGS     3

pid_t fore;
pid_t back = 0;

// Defining signal handlers:

void SIGTERM_Handler()
{
	if(!back)
	{
		kill(-back,SIGTERM);
	}
	raise(SIGKILL);
}

void SIGTSTP_Handler()
{
	if(!back)
	{
		kill(-back,SIGTSTP);
	}
	raise(SIGSTOP);
}

/**
 * Running a command with the given arguments:
 * 
 * Inputs: the command arguments and a flag that tells if it should
 * run in the background or in the foreground (0 for foreground).
 * 
 * Output: bool like int that tells if the program was executed
 * successfully
*/
static int Controller_RunCmd(char *args[], int fg);

void Controller_Execute(int argc, char *args[])
{
	int commands = 1; // Amount of commands in the input
	int ccmd = 0; // Current command being executed
	
	// Command arguments' data:
	char *commandArgs[MAX_ARGS + 2];
	// Initializing the command args buffer:
	for (int j = 0; j < MAX_ARGS + 2; j++)
		commandArgs[j] = NULL;
	int commandInit = 0, commandEnd = 0;
	for (int i = 0; i < argc; i++)
	{
		// Checking if the command ended:
		if (strncmp(args[i], "->", 2) == 0)
		{
			Controller_RunCmd(commandArgs, ccmd++);

			// Checking if there are too many commands:
			commands++;
			if (commands > MAX_COMMANDS) goto command_error;

			// Cleaning the command args buffer:
			for (int j = 0; j < MAX_ARGS + 1; j++)
				commandArgs[j] = NULL;
			
			// Resetting the command limits:
			commandInit = commandEnd = i;
			// Getting the next token:
			continue;
		}

		// Checking if the position of the token in the command is valid:
		int relativePos = commandEnd - commandInit;
		if (relativePos <= MAX_ARGS)
		{
			commandArgs[relativePos] = args[i]; // Adding the argument to current command
		}
		commandEnd++;
	}
	if (commandArgs[0] != NULL) Controller_RunCmd(commandArgs, ccmd++);

	return;

	command_error:
	fprintf(stderr, "OOPS :O... max number of commands reached.\n");
}

static int Controller_RunCmd(char *args[], int fg)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		// const char *constArgs[MAX_ARGS + 2];
		// for (int i = 0; i < MAX_ARGS + 2; i++)
		// 	constArgs[i] = args[i];
		// int success = execv(constArgs[0], constArgs);
		if(fg > 0)
		{
			setpgid(0,back);
		}

		// Try to execute command:
		int success = execv(args[0], args);
		if (success == -1) goto proc_error;
	}
	else if (pid > 0)
	{
		if(!fg) // foreground program
		{
			fore = pid;
		}
		else if(fg == 1) // first background program
		{
			back = pid;
		}
		waitpid(pid, NULL, 0);
	}
	else goto proc_error;

	return 1;

	proc_error:
	fprintf(stderr, "OOPS :O... Looks like there was an error while trying to execute %s.\n", args[0]);
	exit(1);
}
