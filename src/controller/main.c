/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * controller.c: this program will execute the commands to
 * create and execute the programs requested by the user.
*/

#include "controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * argv will contain all the command line tokens that where
 * cought by the shell, and will be used to execute the programs
*/
int main(int argc, char *argv[])
{
	// #ifdef GSH_DEBUG
	// 	printf("Entered in the process creator.\n");
	// #endif

	Controller_Execute(argc, argv);

	return 0;
}
