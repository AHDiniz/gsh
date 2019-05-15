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

// Maximum amount of commands that will be executed
#define MAX_COMMANDS 5

struct gsh
{
	int isRunning;
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
	char *line = GSH_ReadLine();
	char **tokens = GSH_SplitLine(line);

	// Iterating through the tokens:
	int i = 0, commands = 1;
	int commandInit = 0, commandEnd = 0; // Positions of the command's beggining and ending
	while (tokens[i] != NULL)
	{
		// Checking if is the same command:
		if (strncmp(tokens[i], "->", 2) == 0)
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
		if (strncmp(tokens[i], "exit", 4) == 0 && GSH_Exit())
		{
			shell.isRunning = 0;
			break;
		}
		else if (strncmp(tokens[i], "mywait", 6) == 0)
		{
			GSH_MyWait();
		}
		else
		{
			#ifdef GSH_DEBUG
				printf("Something\n");
			#endif
		}

		// Going to the next token:
		i++;
	}

	free(tokens);
	free(line);
}

void GSH_Finish()
{
	// Use this to free memory if necessary
}
