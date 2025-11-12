#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

static void prompt(void){write(STDOUT_FILENO,"$ ",2);}

static void strip_nl(char *s){size_t n;if(!s)return;n=strlen(s);if(n&&s[n-1]=='\n')s[n-1]='\0';}

static char *trim(char *s){size_t i=0,j,n;if(!s)return s;n=strlen(s);while(i<n&&(s[i]==' '||s[i]=='\t'))i++;j=n;while(j>i&&(s[j-1]==' '||s[j-1]=='\t'))j--;s[j]='\0';return s+i;}

static int split(char *line,char **av,int cap){int c=0;char *t=strtok(line," \t");while(t&&c<cap-1){av[c++]=t;t=strtok(NULL," \t");}av[c]=NULL;return c;}

static char *get_path(const char *cmd)
{
char *p=NULL,*copy,*d,*full;int k;
for(k=0;environ[k];k++){if(strncmp(environ[k],"PATH=",5)==0){p=environ[k]+5;break;}}
if(!p)return NULL;
if(strchr(cmd,'/'))return strdup(cmd);
copy=strdup(p);
d=strtok(copy,":");
while(d){full=malloc(strlen(d)+strlen(cmd)+2);sprintf(full,"%s/%s",d,cmd);
if(access(full,X_OK)==0){free(copy);return full;}
free(full);d=strtok(NULL,":");}
free(copy);return NULL;
}

int main(void)
{
char *line=NULL,*w,*av[64],*cmd;size_t cap=0;ssize_t n;int inter,argc,status;pid_t pid;
inter=isatty(STDIN_FILENO);
for(;;)
{
if(inter)prompt();
n=getline(&line,&cap,stdin);
if(n==-1){if(inter)write(STDOUT_FILENO,"\n",1);break;}
strip_nl(line);w=trim(line);if(!*w)continue;
argc=split(w,av,64);if(argc==0)continue;
cmd=get_path(av[0]);
if(!cmd){write(STDERR_FILENO,av[0],strlen(av[0]));write(STDERR_FILENO,": not found\n",12);continue;}
pid=fork();if(pid==-1){free(cmd);continue;}
if(pid==0){execve(cmd,av,environ);perror(av[0]);_exit(127);}
free(cmd);waitpid(pid,&status,0);
}
free(line);return 0;
}
