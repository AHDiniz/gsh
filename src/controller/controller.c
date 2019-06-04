/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * controller.c: this program will execute the commands to
 * create and execute the programs requested by the user.
*/

// #include "controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_COMMANDS 5
#define MAX_ARGS     3

pid_t fore; // foreground program pid
pid_t back = 0; // background group pid

// Defining signal handlers:

// Used when called exit to terminate all children:
void SIGTERM_Handler()
{
	if(!back) kill(-back,SIGTERM);
	exit(0);
}

// Ctrl + z handler:
void SIGTSTP_Handler()
{
	// Redirecting this signal to the background processes: (Foreground children are in the same group of gsh and altomatically receive it)
	if(!back) kill(-back,SIGTSTP);
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

/**
 * argv will contain all the command line tokens that where
 * cought by the shell, and will be used to execute the programs
*/
int main(int argc, char *args[])
{
	signal(SIGTERM, SIGTERM_Handler);
	signal(SIGTSTP, SIGTSTP_Handler);

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

	// Waiting for all children to end it's execution:
	for (int i = 0; i < (back==0 ? 1 : 2); i++)
	{
		pid_t pid;
		pid = waitpid(-1, NULL, 0);
		if(pid == -1) goto wait_error;
		
		// If it's the foreground process that ended:
		if (pid == fore) kill(getppid(), SIGUSR1); // Wake up the shell
		else // If it's one of the background program's that ended:
		{
			// Kill every other process in the background group:
			kill(-back, SIGKILL);
			// Cleaning zombies:
			for (int j = 0; j < ccmd - 2; j++)
				waitpid(-1, NULL, WNOHANG);
		}
	}

	return 0;

	command_error:
	fprintf(stderr, "OOPS :O... max number of commands reached.\n");

	return 1;

	wait_error:
	fprintf(stderr, "OOPS :O... Looks like there was a error while waiting for children to terminate.\n");

	return 1;
}

static int Controller_RunCmd(char *args[], int fg)
{
	pid_t pid = fork(); // Creating a new process
	if (pid == 0) // If it's the child process...
	{
		/*
			Setting the program to be in the background
			if it's not the first command:
		*/
		if(fg > 0)
		{
			setpgid(0,back);
			int fd = open("/dev/null",O_RDWR);
    		if(fd == -1) goto proc_error;
    		dup2(fd,0);
    		dup2(fd,1);
    		dup2(fd,2);
    		if (fd > 2)
    		    close(fd);
		}

		char *cmd = malloc(6 + strlen(args[0]));

		if(strncmp(args[0], ".", 1) != 0)
		{
			strcpy(cmd,"/bin/");
			strcat(cmd,args[0]);
		}
		else
		{
			strcpy(cmd,args[0]);
		}

		// Trying to execute command:
		int success = execv(cmd, args);
		free(cmd);
		if (success == -1) goto proc_error;
	}
	else if (pid > 0) // Id it's the father process...
	{
		if(!fg) // Foreground program
			fore = pid;
		else if(fg == 1) // First background program
			back = pid;
	}
	else goto proc_error; // If there was an error, treat it

	return 1;

	proc_error:
	fprintf(stderr, "OOPS :O... Looks like there was an error while trying to execute %s.\n", args[0]);
	exit(1);
}
