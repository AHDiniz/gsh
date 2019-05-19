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

/**
 * Running a command with the given arguments:
 * 
 * Inputs: the command arguments and a flag that tells if it should
 * run in the background or in the foreground.
 * 
 * Output: bool like int that tells if the program was executed
 * successfully
*/
static int Controller_RunCmd(char *args[], int fg);

void Controller_Execute(int argc, char *args[])
{
	int commands = 1; // Amount of commands in the input
	
	// Command arguments' data:
	char *commandArgs[MAX_ARGS + 2];
	// Initializing the command args buffer:
	for (int j = 0; j <= MAX_ARGS; j++)
		commandArgs[j] = NULL;
	int commandInit = 0, commandEnd = 0;
	for (int i = 0; i < argc; i++)
	{
		// Checking if the command ended:
		if (strncmp(args[i], "->", 2) == 0)
		{
			// Checking if there are too many commands:
			commands++;
			if (commands > MAX_COMMANDS) goto command_error;

			Controller_RunCmd(commandArgs, 1);

			// Cleaning the command args buffer:
			for (int j = 0; j <= MAX_ARGS; j++)
				commandArgs[j] = NULL;
			
			// Resetting the command limits:
			commandInit = commandEnd = i;

			// Getting the next token:
			i++;
			if(i >= argc) goto operator_error;
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
	Controller_RunCmd(commandArgs, 1);

	return;

	operator_error:
	fprintf(stderr, "OOPS :O... missing second argument of operator ->.\n");

	return;

	command_error:
	fprintf(stderr, "OOPS :O... Max number of arguments reached.\n");
}

static int Controller_RunCmd(char *args[], int fg)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		int success = execv(args[0], args);
		if (!success) goto proc_error;
	}
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		kill(getppid(), SIGUSR1);
	}
	else goto proc_error;

	return 1;

	proc_error:
	fprintf(stderr, "OOPS :O... Looks like there was an error while trying to execute %s.\n", args[0]);
	return 0;
}
