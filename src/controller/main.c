/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * controller.c: this program will execute the commands to
 * create and execute the programs requested by the user.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * argv will contain all the command line tokens that where
 * cought by the shell, and will be used to execute the programs
*/
int main(int argc, char *argv[])
{
	#ifdef GSH_DEBUG
		printf("Entered in the process creator.\n");
	#endif

	printf("Trying to wake up father with SIGUSR1\n");

	if(kill(getppid(), SIGUSR1) == -1) printf("Failed to send SIGUSR1\n");

	sleep(10);

	printf("I can't wait anymore... i'm dead XP\n");

	return 0;
}
