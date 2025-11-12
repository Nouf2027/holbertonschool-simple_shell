#include "shell.h"

/**
 * fork_and_execute - Forks and executes a command
 * @command_path: Full path to the command
 * @args: Array of arguments
 * @argv0: Program name for error messages
 *
 * Return: Exit status of the command
 */
int fork_and_execute(char *command_path, char **args, char *argv0)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(command_path);
		return (1);
	}

	if (pid == 0)
	{
		if (execve(command_path, args, environ) == -1)
		{
			perror(argv0);
			free(command_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		free(command_path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}

	return (0);
}

/**
 * execute_command - Executes a command
 * @args: Array of arguments
 * @argv0: Program name for error messages
 * @line: The input line (for memory cleanup on exit)
 * @last_status: The last exit status
 *
 * Return: Exit status of command, or 127 if command not found
 */
int execute_command(char **args, char *argv0, char *line, int last_status)
{
	char *command_path;

	if (is_builtin(args, line, last_status))
		return (0);

	command_path = find_path(args[0]);
	if (!command_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", argv0, args[0]);
		return (127);
	}

	return (fork_and_execute(command_path, args, argv0));
}
