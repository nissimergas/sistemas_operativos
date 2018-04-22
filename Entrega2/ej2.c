#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

char* mi_argc(char* line, int len);
char* mi_argv(char* line, int len);
char* remove_from_string(char* string, int inicio, int fin, int len );//return string sin substring

char* sub_string(char* string, int inicio, int fin, int len );//retun substring
int main(int argc, char *argv[]){
  char *ar[3];
  ar[0]="ls";
  ar[1]=NULL;
  ar[2]=NULL;

  execvp("ls",ar);


    return 0;
}
char* remove_from_string(char* string, int inicio, int fin,int len ){//return string sin substring
  char *nuevo=(char*) malloc((len-(fin-inicio)) * sizeof(char));
  int i;
  int j=0;
  for(i=0;i<len;i++){
    if(!(i>=inicio && i<=fin)){
      nuevo[j]=string[i];
      j++;
    }
  }
  return nuevo;
}

char* sub_string(char* string, int inicio, int fin , int len){//retun substring
    char *nuevo=(char*) malloc((len-(fin-inicio)) * sizeof(char));
    int i;
    int j=0;
    for(i=0;i<len;i++){
      if((i>=inicio && i<=fin)){
        nuevo[j]=string[i];
        j++;
      }
    }
    return nuevo;
}

int argc2(char*string, int len){

}





char* mi_argc(char* line, int len){
  char *argc = (char*) malloc((len) * sizeof(char));
  char *ret;
   ret = strchr(line, ' ');
   int i;
   int cc=ret-line;
   printf("reta: %i ",cc);
   for (i=0;i<(cc);i++){
     argc[i]=line[i];
   }
  return argc;
}


char* mi_argv(char* line, int len){

   char *arcv_string = strchr(line, ' ');


  return arcv_string;
}
