#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
int main() {
  int link[2];
  pid_t pid1,pid2;

  char foo[4096];

  if (pipe(link)==-1)
    die("pipe");

  if ((pid1 = fork()) == -1)
    die("fork");

  if(pid1 == 0) {

    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    char* ar[1];
    ar[0]="ls";
    execvp("ls", ar);
    die("execl");

  }

int link2[2];
char foo2[4096];
if (pipe(link2)==-1)
  die("pipe");

if ((pid2 = fork()) == -1)
  die("fork");
if(pid2 == 0) {

  dup2 (link2[1], STDOUT_FILENO);
  close(link2[0]);
  close(link2[1]);
  char* ar2[2];
  ar2[0]="ls";
  ar2[1]="-al";
  execvp("ls", ar2);
  die("execl");

}
if (pid2!=0) {
close(link2[1]);
int nbytes2 = read(link2[0], foo2, sizeof(foo2));
printf("Output:\n (%.*s)\n", nbytes2, foo2);


}
if(pid1!=0) {
close(link[1]);
int nbytes = read(link[0], foo, sizeof(foo));
printf("Output:\n (%.*s)\n", nbytes, foo);


}
wait(NULL);
wait(NULL);
return 0;
}
