#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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

static char *trim(char *s)
{
char *end;
while (*s == ' ' || *s == '\t')
s++;
end = s + strlen(s);
while (end > s && (end[-1] == ' ' || end[-1] == '\t'))
end--;
*end = '\0';
return (s);
}

static int has_space(const char *s)
{
while (*s)
{
if (*s == ' ' || *s == '\t')
return (1);
s++;
}
return (0);
}

int main(void)
{
char *line = NULL, *cmd;
size_t cap = 0;
ssize_t nread;
pid_t pid;
int status;
char *argv[2];

while (1)
{
print_prompt();
nread = getline(&line, &cap, stdin);
if (nread == -1)
{
write(STDOUT_FILENO, "\n", 1);
break;
}
strip_newline(line);
cmd = trim(line);
if (*cmd == '\0')
continue;
if (has_space(cmd))
{
dprintf(STDERR_FILENO, "%s: not found\n", cmd);
continue;
}
pid = fork();
if (pid == -1)
{
perror("fork");
continue;
}
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
return (0);
}
