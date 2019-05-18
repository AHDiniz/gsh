/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * gsh.h: shell's interface
*/

#ifndef GSH_H_

#define GSH_H_

/**
 * Function that starts the shell:
 * 
 * Output: bool like int that tells if the creation was successful or not
*/
int GSH_Init();

/**
 * Function that finishes the shell:
*/
void GSH_Finish();

/**
 * Function that tells if the shell is still running:
 * 
 * Output: bool like int that tells if the shell didn't received a exit command
*/
int GSH_IsRunning();

/**
 * Function that reads and executes the given commands:
*/
void GSH_ReadAndExecute();

#endif
