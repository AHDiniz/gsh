/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz & Rafael Belmock Pedruzzi
 * 
 * gsh.c: implementing the shell's interface functions
*/

#include "gsh.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum amount of commands that will be executed
#define MAX_COMMANDS 5

struct gsh
{
	int isRunning;
	char **tokens;
};

struct gsh shell;

int GSH_Init()
{
	// Welcome message:
	printf("Welcome to gsh: the Linux Group SHell :)\n");
	printf("Written by Alan Diniz and Rafael Belmock.\n\n");
	
	shell.isRunning = 1; // The shell is now running
	
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
	shell.tokens = GSH_SplitLine(GSH_ReadLine());

	// Iterating through the tokens:
	int i = 0, commands = 1;
	int commandInit = 0, commandEnd = 0; // Positions of the command's beggining and ending
	while (shell.tokens[i] != NULL)
	{
		// Checking if is the same command:
		if (strncmp(shell.tokens[i], "->", 2) == 0)
		{
			commands++; // There's a new command
			
			// Checking if the command limit wasn't broken:
			if (commands >= MAX_COMMANDS + 1) break;
			
			// Getting the next token:
			i++;
			commandInit = commandEnd = i; // Resetting the command limits
			continue;
		}
		// If the current command is still the same:
		
		// Checking if the command is an internal operation:
		if (strncmp(shell.tokens[i], "exit", 4) == 0)
		{
			shell.isRunning = 0;
			break;
		}
		else if (strncmp(shell.tokens[i], "mywait", 6) == 0)
		{
			// Treat mywait operation
		}
	}
}

void GSH_Finish()
{
	// Use this to free memory if necessary
	free(shell.tokens);
}
