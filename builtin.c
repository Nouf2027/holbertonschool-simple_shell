#include "shell.h"

/**
 * handle_exit - Handles the exit built-in command
 * @line: The input line to free
 * @args: The argument array to free
 * @status: The last exit status to return
 *
 * Return: void (exits the program)
 */
void handle_exit(char *line, char **args, int status)
{
	free(line);
	free_array(args);
	exit(status);
}

/**
 * handle_env - Handles the env built-in command
 *
 * Return: 0 on success
 */
int handle_env(void)
{
	char **env = environ;

	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
	return (0);
}

/**
 * is_builtin - Checks if command is a built-in command
 * @args: Array of command arguments
 * @line: The input line (for memory cleanup on exit)
 * @status: The last exit status
 *
 * Return: 1 if built-in executed, 0 otherwise
 */
int is_builtin(char **args, char *line, int status)
{
	if (args == NULL || args[0] == NULL)
		return (0);

	if (strcmp(args[0], "exit") == 0)
	{
		handle_exit(line, args, status);
		return (1);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		handle_env();
		return (1);
	}

	return (0);
}
