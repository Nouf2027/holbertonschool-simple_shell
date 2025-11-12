#include "shell.h"

/**
 * main - Entry point for the simple shell
 * @argc: Argument count
 * @argv: Argument vector
 * @env: Environment variables
 *
 * Return: 0 on success
 */
int main(int argc, char **argv, char **env)
{
    char *command;
    char **args;
    int status = 1;
    (void)argc;
    (void)argv;
    (void)env;

    while (status)
    {
        if (isatty(STDIN_FILENO))
            display_prompt();

        command = read_command();
        if (command == NULL)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        args = parse_command(command);
        if (args[0] != NULL)
        {
            status = execute_command(args);
        }

        free(command);
        free(args);
    }

    return (0);
}

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
    write(STDOUT_FILENO, "($) ", 4);
}

/**
 * read_command - Reads a command from stdin
 *
 * Return: Pointer to the command string
 */
char *read_command(void)
{
    char *command = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&command, &len, stdin);
    if (nread == -1)
    {
        free(command);
        return (NULL);
    }

    if (command[nread - 1] == '\n')
        command[nread - 1] = '\0';

    return (command);
}

/**
 * parse_command - Parses a command string into arguments
 * @command: The command string to parse
 *
 * Return: Array of argument strings
 */
char **parse_command(char *command)
{
    char **args = malloc(64 * sizeof(char *));
    char *token;
    int i = 0;

    if (!args)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = strtok(command, " ");
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
 * execute_command - Executes a command
 * @args: Array of command arguments
 *
 * Return: 1 to continue, 0 to exit
 */
int execute_command(char **args)
{
    pid_t pid;
    int status;
    char *full_path;

    if (check_builtin(args))
        return (1);

    full_path = find_path(args[0]);
    if (!full_path)
    {
        fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
        return (1);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(full_path);
        return (1);
    }
    else if (pid == 0)
    {
        if (execve(full_path, args, environ) == -1)
        {
            fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
            free(full_path);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        wait(&status);
        free(full_path);
    }

    return (1);
}

/**
 * check_builtin - Checks if command is a builtin
 * @args: Array of command arguments
 *
 * Return: 1 if builtin was executed, 0 otherwise
 */
int check_builtin(char **args)
{
    if (args[0] == NULL)
        return (1);

    if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(args[0], "env") == 0)
    {
        char **env = environ;
        while (*env)
        {
            printf("%s\n", *env);
            env++;
        }
        return (1);
    }

    return (0);
}

/**
 * find_path - Finds the full path of a command
 * @command: The command to find
 *
 * Return: Full path of command, or NULL if not found
 */
char *find_path(char *command)
{
    char *path, *path_copy, *dir, *full_path;
    struct stat st;

    if (strchr(command, '/') != NULL)
    {
        if (stat(command, &st) == 0)
            return (strdup(command));
        return (NULL);
    }

    path = getenv("PATH");
    if (!path)
        return (NULL);

    path_copy = strdup(path);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir)
    {
        full_path = build_path(dir, command);
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
 * build_path - Builds a full path from directory and command
 * @path: Directory path
 * @command: Command name
 *
 * Return: Full path string
 */
char *build_path(char *path, char *command)
{
    char *full_path;
    size_t len;

    len = strlen(path) + strlen(command) + 2;
    full_path = malloc(len);
    if (!full_path)
        return (NULL);

    strcpy(full_path, path);
    strcat(full_path, "/");
    strcat(full_path, command);

    return (full_path);
}
