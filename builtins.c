#include "shell.h"

/**
 * bi_exit - builtin that signals the shell to exit
 * @argv: argument vector (unused)
 * Return: -1 to tell the main loop to exit
 */
int bi_exit(char **argv)
{
	(void)argv;
	return (-1);
}

/**
 * handle_builtin - checks and executes builtins
 * @argv: tokens (argv[0] is command name)
 * Return: -1 if exit, 1 if handled other builtin, 0 if not a builtin
 */
int handle_builtin(char **argv)
{
	if (!argv || !argv[0])
		return (0);
	if (strcmp(argv[0], "exit") == 0)
		return (bi_exit(argv));
	return (0);
}
