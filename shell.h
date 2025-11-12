#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;

/* Function prototypes */
void display_prompt(void);
char *read_command(void);
char **parse_command(char *command);
int execute_command(char **args);
int check_builtin(char **args);
char *find_path(char *command);
char *build_path(char *path, char *command);

#endif /* SHELL_H */
