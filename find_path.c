#include "shell.h"

/**
 * get_path_env - Gets the PATH environment variable
 *
 * Return: Pointer to PATH value, or NULL if not found or empty
 */
char *get_path_env(void)
{
	int i = 0;
	char *path_value;

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path_value = environ[i] + 5;
			if (*path_value == '\0')
				return (NULL);
			return (path_value);
		}
		i++;
	}
	return (NULL);
}

/**
 * check_absolute_path - Checks if command is absolute or relative path
 * @command: The command to check
 *
 * Return: Duplicated command path if valid, NULL otherwise
 */
char *check_absolute_path(char *command)
{
	struct stat st;

	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
	}
	return (NULL);
}

/**
 * search_in_path - Searches for command in PATH directories
 * @command: The command to find
 * @path_env: The PATH environment variable value
 *
 * Return: Full path if found, NULL otherwise
 */
char *search_in_path(char *command, char *path_env)
{
	char *path_copy, *dir, *full_path;
	struct stat st;

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		full_path = malloc(strlen(dir) + strlen(command) + 2);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full_path, "%s/%s", dir, command);
		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * find_path - Finds the full path of a command
 * @command: The command name to search for
 *
 * Return: Full path to command if found, NULL otherwise
 */
char *find_path(char *command)
{
	char *path_env, *result;

	result = check_absolute_path(command);
	if (result)
		return (result);

	path_env = get_path_env();
	if (!path_env)
		return (NULL);

	return (search_in_path(command, path_env));
}
