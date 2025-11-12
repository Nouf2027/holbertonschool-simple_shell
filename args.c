#include "shell.h"
#include <string.h>

/**
 * make_argv - split line into tokens separated by space/tab/newline
 * @line: input buffer (modified in place)
 * @argv: output array of pointers (size = cap)
 * @cap: capacity of argv
 *
 * Return: number of tokens; 0 if no tokens
 */
int make_argv(char *line, char **argv, int cap)
{
	int i = 0;
	char *tok;

	if (!line || !argv || cap < 2)
		return (0);

	tok = strtok(line, " \t\n");
	while (tok && i < cap - 1)
	{
		argv[i++] = tok;
		tok = strtok(NULL, " \t\n");
	}
	argv[i] = NULL;
	return (i);
}
