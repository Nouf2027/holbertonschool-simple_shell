#include "shell.h"

static int shell_step(void);
static int g_status;

/**
 * exec_external - fork/exec and wait for an external command
 * @argv: argument vector (NULL-terminated)
 * @status: pointer to last status code (updated)
 * Return: 0 on parent path, -1 on fork error
 */
int exec_external(char **argv, int *status)
{
	pid_t pid = fork();
	int st = 0;

	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (execvp(argv[0], argv) == -1)
		{
			perror(argv[0]);
			_exit(127);
		}
		_exit(0);
	}
	if (wait(&st) == -1)
		perror("wait");

	if (WIFEXITED(st))
		*status = WEXITSTATUS(st);
	else if (WIFSIGNALED(st))
		*status = 128 + WTERMSIG(st);

	g_status = *status;
	return (0);
}

/**
 * shell_step - executes a single REPL iteration
 * Return: 1 to continue loop, 0 to stop
 */
static int shell_step(void)
{
	char *line = NULL;
	size_t len = 0;
	char **argv = NULL;
	int cap = 64, n, bret;
	ssize_t rd;

	if (isatty(STDIN_FILENO))
		print_prompt();

	rd = getline(&line, &len, stdin);
	if (rd == -1)
	{
		free(line);
		return (0);
	}

	strip_newline(line);

	argv = malloc(sizeof(char *) * cap);
	if (!argv)
	{
		free(line);
		return (1);
	}

	n = make_argv(line, argv, cap);
	if (n == 0)
	{
		free(argv);
		free(line);
		return (1);
	}

	bret = handle_builtin(argv);
	if (bret == -1)
	{
		free(argv);
		free(line);
		return (0);
	}
	if (bret == 1)
	{
		free(argv);
		free(line);
		return (1);
	}

	(void)exec_external(argv, &g_status);

	free(argv);
	free(line);
	return (1);
}

/**
 * run_shell - REPL loop of the simple shell
 * Return: last command status (used as process exit status)
 */
int run_shell(void)
{
	while (shell_step())
		;
	return (g_status);
}
