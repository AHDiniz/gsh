/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * internal.c: shell's internal operations implementation
*/

#include "internal.h"

#include <stdio.h>
#include <wait.h>
#include <sys/types.h>

int GSH_Exit()
{
	return 1;
}

void GSH_MyWait()
{
	pid_t pid;
	do {
		pid = waitpid(-1, NULL, WNOHANG);
		if(pid == -1) goto wait_error;
	} while(pid > 0);

	return;

	wait_error:
	fprintf(stderr, "OOPS :O... Looks like there was an error while trying to free zombies (obviously).\n");
}
