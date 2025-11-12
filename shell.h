#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int _strlen(const char *s);
extern char **environ;

char *resolve_cmd(const char *name);
void print_prompt(void);
void strip_newline(char *s);
char *ltrim_rtrim(char *s);
int make_argv(char *line, char **argv, int cap);

int handle_builtin(char **argv);
int bi_exit(char **argv);

int run_shell(void);
int exec_external(char **argv, int *status);

#endif /* SHELL_H */
