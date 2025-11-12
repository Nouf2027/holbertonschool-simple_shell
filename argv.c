#include "shell.h"

/**
 * make_argv - split a line into argv using spaces/tabs
 * @line: input line (will be modified in place)
 * @argv: pre-allocated array of char* (size = cap)
 * @cap: capacity of argv array
 *
 * Return: number of tokens; 0 if empty
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
