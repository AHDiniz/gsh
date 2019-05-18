/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz, Rafael Belmock Pedruzzi & Israel Santos
 * 
 * parser.h: line parser interface
*/

#ifndef PARSER_H_

#define PARSER_H_

/**
 * Getting a line in the shell:
 * 
 * Output: the line that the user wrote in the shell
*/
char *GSH_ReadLine(void);

/**
 * Separating the input line:
 * 
 * Input: the line that was read
 * 
 * Output: the tokens that might be shell commands
*/
char **GSH_SplitLine(char *line);

#endif
