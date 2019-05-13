/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz & Rafael Belmock Pedruzzi
 * 
 * main.c: program's entry point
*/

#include <stdio.h>
#include <stdlib.h>
#include "gsh.h" // ? Makefile allows not using the path to header file to include it

int main()
{
	// Initializing the shell and checking for errors:
	if (!GSH_Init())
	{
		printf("Could not initialize gsh... :(\n");
		return EXIT_FAILURE;
	}

	while (GSH_IsRunning()) // While an exit command is not given or an error doesn't happen
	{
		GSH_ReadAndExecute(); // Read the commands and executing them
	}

	GSH_Finish();

	return EXIT_SUCCESS;
}
