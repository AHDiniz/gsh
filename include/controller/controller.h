/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * controller.h: process controller functions' definitions
*/

#ifndef CONTROLLER_H_

#define CONTROLLER_H_

/**
 * Reads and runs the commands given by the user, if they are not internal operations.
 * 
 * Input: the array of tokens that where cought in the user's input ant the number of tokens in it.
*/
void Controller_Execute(int argc, char *args[]);

#endif
