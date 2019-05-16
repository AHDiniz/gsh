/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz & Rafael Belmock Pedruzzi
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
	int zombies = 0;
	pid_t pid;
	sigset_t sigmask;
};

struct gsh shell;

/**
 * Defining function to execute a given command:
 * 
 * Inputs: all the program arguments: the executable and it's args (max. 3)
 * 
 * Output: bool like int that tells if the operation was a success
*/
static int GSH_Execute(char *args[]);

int GSH_Init()
{
	// Starting gsh as lider of a session:
	pid_t p = fork();
	if(p) exit(0);
	shell.pid = setsid();

	// 

	// Welcome message:
	printf("Welcome to gsh: the Linux Group SHell :)\n");
	printf("Written by Alan Diniz and Rafael Belmock.\n\n");
	
	shell.isRunning = 1; // The shell is now running
	
	return 1;

	init_error:
	fprintf(stderr, "OOPS :O... Looks like there was an initialization error.\n");
	return 0;
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
	int commandInit = 0, commandEnd = 0; // Positions of the command's beggining and ending
	char *args[MAX_ARGS + 2];
	args[MAX_ARGS + 1] = NULL;
	int internal = 0; // Tells if current command is an internal operation
	while (tokens[i] != NULL)
	{
		// Checking if is the same command:
		if (strncmp(tokens[i], "->", 2) == 0)
		{
			commands++; // There's a new command
			
			// Checking if the command limit wasn't broken:
			if (commands >= MAX_COMMANDS + 1) break;

			GSH_Execute(args); // Executing the current command
			for (int j = 0; j <= MAX_ARGS; j++)
				args[j] = " ";
			
			// Getting the next token:
			i++;
			commandInit = commandEnd = i; // Resetting the command limits
			continue;
		}
		// If the current command is still the same:
		
		// Checking if the command is an internal operation:
		if (strncmp(tokens[i], "exit", 4) == 0 && GSH_Exit())
		{
			shell.isRunning = 0;
			internal = 1;
			break;
		}
		else if (strncmp(tokens[i], "mywait", 6) == 0)
		{
			GSH_MyWait();
			internal = 1;
		}
		else
		{
			// Checking if the position of the token in the command is valid:
			int relativePos = commandEnd - commandInit;
			if (relativePos <= MAX_ARGS)
			{
				args[relativePos] = tokens[i]; // Adding the argument to current command
			}
			commandEnd++;
		}

		// Going to the next token:
		i++;
	}
	// Executing the last command:
	if (!internal) GSH_Execute(args);

	free(tokens);
	free(line);
}

void GSH_Finish()
{
	// Use this to free memory if necessary
}

static int GSH_Execute(char *args[])
{
	pid_t pid = fork();
	if (pid == 0) // Child process
	{
		// Executing the program:
		int success = execv(args[0], args);
		if (!success) goto proc_error;
	}
	else if (pid > 0) // Parent
	{
		waitpid(pid, NULL, 0);
	}
	else goto proc_error;
	
	return 1;

	proc_error:
	fprintf(stderr, "OOPS :O... Looks like there was an error while trying to execute %s.\n", args[0]);
	return 0;
}
