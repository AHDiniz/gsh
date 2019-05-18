/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * internal.h: shell's internal operations definition
*/

#ifndef INTERNAL_H_

#define INTERNAL_H_

/**
 * Exit command: if there are no child processes, the shell exits
 * 
 * Output: bool like int that tells if the shell can exit
*/
int GSH_Exit();

/**
 * MyWait command: frees all zombie child processes
*/
void GSH_MyWait();

#endif
