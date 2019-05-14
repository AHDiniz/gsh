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

	// Input testing:
	char **tokens = GSH_SplitLine(GSH_ReadLine()); // Getting the command tokens
	int i = 0;
	char *token = tokens[i];

	while (token != NULL)
	{
		printf("%s\n", token);
		token = tokens[++i];
	}

	shell.isRunning = 0;
}

void GSH_Finish()
{
	// Use this to free memory if necessary
}
