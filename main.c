#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
extern char **environ;

static void print_prompt(void)
{
write(STDOUT_FILENO, "$ ", 2);
}

static void strip_newline(char *s)
{
size_t n;
if (!s)
return;
n = strlen(s);
if (n && s[n - 1] == '\n')
s[n - 1] = '\0';
}

static char *trim_spaces(char *s)
{
size_t i, j, n;
if (!s)
return s;
n = strlen(s);
i = 0;
while (i < n && (s[i] == ' ' || s[i] == '\t'))
i++;
j = n;
while (j > i && (s[j - 1] == ' ' || s[j - 1] == '\t'))
j--;
s[j] = '\0';
return s + i;
}

static int has_space(const char *s)
{
size_t i;
if (!s)
return 0;
for (i = 0; s[i]; i++)
if (s[i] == ' ' || s[i] == '\t')
return 1;
return 0;
}

int main(void)
{
char *line = NULL, *cmd, *argv[2];
size_t cap = 0;
ssize_t nread;
int interactive, status;
pid_t pid;
interactive = isatty(STDIN_FILENO);
while (1)
{
if (interactive)
print_prompt();
nread = getline(&line, &cap, stdin);
if (nread == -1)
{
if (interactive)
write(STDOUT_FILENO, "\n", 1);
break;
}
strip_newline(line);
cmd = trim_spaces(line);
if (!*cmd)
continue;
if (has_space(cmd))
{
dprintf(STDERR_FILENO, "%s: not found\n", cmd);
continue;
}
pid = fork();
if (pid == -1)
continue;
if (pid == 0)
{
argv[0] = cmd;
argv[1] = NULL;
execve(cmd, argv, environ);
perror(cmd);
_exit(127);
}
waitpid(pid, &status, 0);
}
free(line);
return 0;
}
