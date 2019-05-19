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
	int childs, zombies;
};

struct gsh shell;

/**
 * Defining signal handlers:
 *
*/

void Child_Handler()
{
	// shell.childs -= 1;
	// shell.zombies += 1;

	// while(1) {

	// }

	printf("I got a SIGCHLD! XD\n");
}

// void SIGUSR1_Handler()
// {
// 	printf("I got a SIGUSR1!\n");
// }

/**
 * Defining function to execute a given command:
 * 
 * Inputs: all the program arguments: the executable and it's args (max. 3)
 * 
 * Output: bool like int that tells if the operation was a success
*/
static int GSH_Execute(char *args[]);

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
	printf("Welcome to gsh: the Linux Group SHell :)\n");
	printf("Written by Alan Diniz and Rafael Belmock.\n\n");
	
	shell.childs = shell.zombies = 0;
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
	// int commandInit = 0, commandEnd = 0; // Positions of the command's beggining and ending
	// char *args[MAX_ARGS + 2];
	// args[MAX_ARGS + 1] = NULL;
	int internal = 0; // Tells if current command is an internal operation
	for (int i = 0; tokens[i] != NULL; i++)
	{
		if (strncmp(tokens[i], "->", 2) == 0)
			commands++; // There's a new command
	}
	while (tokens[i] != NULL && !internal)
	{
		// // Checking if is the same command:
		if (strncmp(tokens[i], "->", 2) == 0)
		{
			// commands++; // There's a new command
			
		// 	// Checking if the command limit wasn't broken:
		// 	if (commands >= MAX_COMMANDS + 1) break;

		// 	// GSH_Execute(args); // Executing the current command
		// 	for (int j = 0; j <= MAX_ARGS; j++)
		// 		args[j] = " ";
			
			// Getting the next token:
			i++;
		// 	commandInit = commandEnd = i; // Resetting the command limits
		// 	continue;
		}

		// If the current command is still the same:
		// Checking if the command is an internal operation:
		if (strncmp(tokens[i], "exit", 4) == 0) // Exit command
		{
			internal = 1;
			if (commands == 1 && GSH_Exit())
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
			if (commands == 1)
			{
				GSH_MyWait();
			}
			else
			{
				fprintf(stderr, "OOPS :O... mywait command wasn't used properlly.\n");
				fprintf(stderr, "Try executing it on it's own and it'll probably work. :D\n");
			}
			
		}
		// else
		// {
		// 	// Checking if the position of the token in the command is valid:
		// 	int relativePos = commandEnd - commandInit;
		// 	if (relativePos <= MAX_ARGS)
		// 	{
		// 		args[relativePos] = tokens[i]; // Adding the argument to current command
		// 	}
		// 	commandEnd++;
		// }

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
		sleep(10);
		kill(pid,SIGINT);
		waitpid(pid, NULL, 0);
	}
	else goto proc_error;
	
	return 1;

	proc_error:
	fprintf(stderr, "OOPS :O... Looks like there was an error while trying to execute %s.\n", args[0]);
	return 0;
}

static int GSH_Controller(char *args[])
{
	// signal(SIGUSR1, SIGUSR1_Handler);
	for (int j = 0; args[j] != NULL; j++)
	{
		printf("%s\n", args[j]);
	}

	// Creating a child process that will control the requested programs:
	pid_t pid = fork();

	// int count; // Amount of tokens in args
	// for (count = 0; args[count] != NULL; count++);
	// // Setting the arguments that will be passed on exec:
	// char **realArgs = malloc(sizeof(*realArgs) * count + 1);
	
	// If it is the child:
	if (pid == 0)
	{
		// realArgs[0] = "./bin/controller";
		// for (int i = 1; i <= count; i++)
		// 	realArgs[i] = args[i - 1];

		// Executing the program:
		// int success = execv(realArgs[0], realArgs);
		int success = execv("./bin/controller", args);
		if (!success) goto proc_error;
	}
	else if (pid > 0)
	{
		// waitpid(pid, NULL, 0);
		pause();
		// printf("I am awake again! It worked?\n");
	}
	else goto proc_error;
	
	// free(realArgs);
	return 1;

	proc_error:
	fprintf(stderr, "OOPS :O... Looks like there was an error while trying to execute the given commands.\n");
	return 0;
}
