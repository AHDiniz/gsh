/**
 * gsh: simple Linux Group SHell
 * 
 * Alan Herculano Diniz & Rafael Belmock Pedruzzi
 * 
 * parser.c: implementing the line parsing
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GSH_BUFF_SIZE 1024
#define GSH_TOKS_SIZE 64
#define GSH_TOK_LIMIT " \t\r\n\a"

char *GSH_ReadLine(void)
{
	// Buffer size:
	int buffsize = GSH_BUFF_SIZE;
	
	// Current position in the buffer:
	int position = 0;
	
	// The string buffer that will get the line:
	char *buffer = malloc(sizeof(char) * buffsize);
	
	// The current character that is being read:
	char character;

	// Checking if the buffer was created successfully:
	if (!buffer) goto mem_error;

	while (1)
	{
		// Reading the character:
		character = getchar();

		// If the character is a new line or EOF:
		if (character == EOF || character == '\n')
		{
			buffer[position] = '\0'; // The buffer recieves a termination character
			return buffer; // And is returned
		}
		else buffer[position] = character;

		position++;

		// If position is bigger then the buffer size:
		if (position >= buffsize)
		{
			// The buffer will be realloc-ed
			buffsize += GSH_BUFF_SIZE;
			buffer = realloc(buffer, buffsize);
			// If there's not enough memory for the program:
			if (!buffer) goto mem_error;
		}
	}

	mem_error:
	fprintf(stderr, "OOPS :O... Looks like there was an allocation error.\n"); // Error message (not debug)
	exit(EXIT_FAILURE);
}

char **GSH_SplitLine(char *line)
{
	// Amount of tokens in the buffer:
	int buffsize = GSH_TOKS_SIZE; // Size of the token buffer
	char **tokens = malloc(sizeof(*tokens) * buffsize); // Tokens buffer
	char *token; // Current token
	int position = 0; // Current position in the buffer

	// Out of memory error checking:
	if (!tokens) goto mem_error;

	// Getting the first token:
	token = strtok(line, GSH_TOK_LIMIT);
	
	// While we can get a token:
	while (token != NULL)
	{
		// Putting the token in the buffer:
		tokens[position] = token;
		position++;

		// If position is bigger then the buffer size:
		if (position >= buffsize)
		{
			// Realocating the buffer with bigger size:
			buffsize += GSH_TOKS_SIZE;
			tokens = realloc(tokens, sizeof(*tokens) * buffsize);

			// Error checking:
			if (!tokens) goto mem_error;
		}

		// Getting the next token:
		token = strtok(NULL, GSH_TOK_LIMIT);
	}
	// Setting the last position to NULL to ease iterations
	tokens[position] = NULL;
	return tokens;

	mem_error:
	fprintf(stderr, "OOPs :O... Looks like there was a allocation error.\n"); // Error message (not debug)
	exit(EXIT_FAILURE);
}
