#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
extern char **environ;
static void print_prompt(void)
{
write(STDOUT_FILENO,"$ ",2);
}
static void strip_newline(char *s)
{
size_t n;
if(!s)return;
n=strlen(s);
if(n&&s[n-1]=='\n')s[n-1]='\0';
}
static char *ltrim_rtrim(char *s)
{
size_t i=0,j,n;
if(!s)return s;
n=strlen(s);
while(i<n&&(s[i]==' '||s[i]=='\t'))i++;
j=n;
while(j>i&&(s[j-1]==' '||s[j-1]=='\t'))j--;
s[j]='\0';
return s+i;
}
static int make_argv(char *line,char **argv,int cap)
{
int c=0;
char *tok=strtok(line," \t");
while(tok&&c<cap-1){argv[c++]=tok;tok=strtok(NULL," \t");}
argv[c]=NULL;
return c;
}
static char *find_in_path(const char *cmd)
{
char *path,*dir,*full;
if(strchr(cmd,'/'))return strdup(cmd);
path=getenv("PATH");
if(!path)return NULL;
path=strdup(path);
dir=strtok(path,":");
while(dir)
{
full=malloc(strlen(dir)+strlen(cmd)+2);
sprintf(full,"%s/%s",dir,cmd);
if(access(full,X_OK)==0){free(path);return full;}
free(full);
dir=strtok(NULL,":");
}
free(path);
return NULL;
}
int main(void)
{
char *line=NULL,*work,*argv[64],*cmdpath;
size_t cap=0;
ssize_t n;
int interactive,status,argc;
pid_t pid;
interactive=isatty(STDIN_FILENO);
while(1)
{
if(interactive)print_prompt();
n=getline(&line,&cap,stdin);
if(n==-1)
{
if(interactive)write(STDOUT_FILENO,"\n",1);
break;
}
strip_newline(line);
work=ltrim_rtrim(line);
if(!*work)continue;
argc=make_argv(work,argv,64);
if(argc==0)continue;
cmdpath=find_in_path(argv[0]);
if(!cmdpath){write(STDERR_FILENO,argv[0],strlen(argv[0]));write(STDERR_FILENO,": not found\n",12);continue;}
pid=fork();
if(pid==-1){free(cmdpath);continue;}
if(pid==0)
{
execve(cmdpath,argv,environ);
perror(argv[0]);
_exit(127);
}
free(cmdpath);
waitpid(pid,&status,0);
}
free(line);
return 0;
}
