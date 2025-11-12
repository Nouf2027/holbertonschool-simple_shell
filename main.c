#include "shell.h"

static char *path_value(void)
{
int i=0;
while(environ&&environ[i]){if(strncmp(environ[i],"PATH=",5)==0)return environ[i]+5;i++;}return NULL;
}

char *resolve_cmd(const char *name)
{
char *pv,*dup,*tok,*p;size_t sz;
if(!name||!*name)return NULL;
if(strchr(name,'/')){if(access(name,X_OK)==0)return strdup(name);return NULL;}
pv=path_value();if(!pv)return NULL;
dup=strdup(pv);if(!dup)return NULL;
tok=strtok(dup,":");
while(tok){sz=strlen(tok)+1+strlen(name)+1;p=malloc(sz);if(!p){tok=strtok(NULL,":");continue;}snprintf(p,sz,"%s/%s",tok,name);if(access(p,X_OK)==0){free(dup);return p;}free(p);tok=strtok(NULL,":");}
free(dup);return NULL;
}

void print_prompt(void){write(STDOUT_FILENO,"$ ",2);}

void strip_newline(char *s){size_t n;if(!s)return;n=strlen(s);if(n&&s[n-1]=='\n')s[n-1]='\0';}

char *ltrim_rtrim(char *s)
{
size_t i=0,j,n;if(!s)return s;n=strlen(s);while(i<n&&(s[i]==' '||s[i]=='\t'))i++;j=n;while(j>i&&(s[j-1]==' '||s[j-1]=='\t'))j--;s[j]='\0';return s+i;
}

int make_argv(char *line,char **argv,int cap)
{
int c=0;char *t=strtok(line," \t");while(t&&c<cap-1){argv[c++]=t;t=strtok(NULL," \t");}argv[c]=NULL;return c;
}

int main(void)
{
char *line=NULL,*work,*argv[64],*cmd=NULL;size_t cap=0;ssize_t n;int interactive,status,argc;pid_t pid;
interactive=isatty(STDIN_FILENO);
for(;;){
if(interactive)print_prompt();
n=getline(&line,&cap,stdin);
if(n==-1){if(interactive)write(STDOUT_FILENO,"\n",1);break;}
strip_newline(line);
work=ltrim_rtrim(line);
if(!*work)continue;
argc=make_argv(work,argv,64);
if(argc==0)continue;
cmd=resolve_cmd(argv[0]);
if(!cmd){write(STDERR_FILENO,argv[0],strlen(argv[0]));write(STDERR_FILENO,": not found\n",12);continue;}
pid=fork();
if(pid==-1){free(cmd);continue;}
if(pid==0){execve(cmd,argv,environ);perror(argv[0]);_exit(127);}
waitpid(pid,&status,0);
free(cmd);
}
free(line);
return 0;
}
