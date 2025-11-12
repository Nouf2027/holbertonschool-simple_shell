#include "shell.h"

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
	write(STDOUT_FILENO, "$ ", 2);
}

/**
 * read_line - Reads a line from stdin
 *
 * Return: Pointer to the line read
 */
char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return (NULL);
	}

	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	return (line);
}

/**
 * parse_line - Parses a line into arguments
 * @line: The line to parse
 *
 * Return: Array of arguments
 */
char **parse_line(char *line)
{
	char **args = malloc(64 * sizeof(char *));
	char *token;
	int i = 0;

	if (!args)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " ");
	while (token != NULL)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return (args);
}

/**
 * free_array - Frees an array of strings
 * @array: The array to free
 */
void free_array(char **array)
{
	if (array)
		free(array);
}

/**
 * main - Entry point for the simple shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: Exit status
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	char **args = NULL;
	int status = 0;

	(void)argc;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();

		line = read_line();
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (line[0] == '\0')
		{
			free(line);
			continue;
		}

		args = parse_line(line);
		if (args == NULL || args[0] == NULL)
		{
			free(line);
			free_array(args);
			continue;
		}

		status = execute_command(args, argv[0], line, status);
		free(line);
		free_array(args);
	}
	return (status);
}
