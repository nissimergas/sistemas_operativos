#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int argc2(char* line, int len);
char** argv2(char* line, int len);
char* mi_argc(char* line, int len);
char* mi_argv(char* line, int len);

int main(int argc, char *argv[]){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    //int i;
		int n = atoi(argv[2]);
    fp = fopen(argv[1], "r");

    while (1) {
      //sleep(2);sleep(1);

			read = getline(&line, &len, fp);
      if (read  != -1){


				printf("string %s",line);
				printf("\n");
	      char**ar=argv2(line,len);
				//printf("comando: %s, parametro:%s , %s",ar[0],ar[1],ar[2]);
				int pid=fork();
				if(pid==0){
	       printf("\n");

 			 // ar[1]="-al";


 			  execvp(ar[0],ar);
			}
				sleep(7);





			}
			else{break;}

    }
    sleep(1);

    fclose(fp);


    return 0;
}
char* mi_argc(char* line, int len){
  char *argc = (char*) malloc((len) * sizeof(char));
  char *ret;
   ret = strchr(line, ' ');
   int i;
   int cc=ret-line;
//   printf("reta: %i ",cc);
   for (i=0;i<(cc);i++){
     argc[i]=line[i];
   }
  return argc;
}

char* mi_argc_con_comillas(char* line, int len){
  char *argc = (char*) malloc((len) * sizeof(char));
  char *ret;
   ret = strchr(line, '"');
   int i;
   int cc=ret-line;
//   printf("reta: %i ",cc);
   for (i=0;i<(cc);i++){
     argc[i]=line[i];
   }
  return argc;
}


char* mi_argv(char* line, int len){

   char *arcv_string = strchr(line, ' ');


  return arcv_string;
}


int argc2(char* line, int len){
  char*line2=line;
  int cantidad_espacios=0;
  while(1){
		char *substring = strchr(line2, ' ');

		if (substring!=NULL){


  //  printf("%s",substring);
    line2=substring+1;
    cantidad_espacios++;
	}
	else{break;}
  }
  return cantidad_espacios;

}
char **argv2(char* line, int len){
  int arc=argc2(line,len);


//  printf("%i",arc);
  char**argumentos=(char* *) malloc((arc+1) * sizeof(char*));
	int i;
	int a=0;
	  for(i=0;i<arc;i++){
			argumentos[i]=NULL;
		}

  for(i=0;i<arc;i++){
		int posicion_comilla=10000000;
		int posicion_espacio=10000000;
		char *ret;
		ret = strchr(line, ' ');
	   if(ret !=NULL){
				 posicion_espacio=ret-line;
			 }
			 ret = strchr(line, '"') ;
		 if(ret != NULL ){
			 	 posicion_comilla=ret-line;
			 }

      if(posicion_espacio<posicion_comilla){
			  char* s=mi_argc(line,len);
			  if (strcmp(s, "")!=0){
					  argumentos[a]=s;

						printf("par: i%si",  argumentos[a]);
						printf("\n");
						printf("string restante: %s", line);
						printf("\n");
						a++;
				}


	      char *substring=strchr(line, ' ');
	      int l= substring-line;
	      len-=l;
	      line=substring+1;
			}
			if(posicion_espacio>posicion_comilla){
				line=strchr(line, '"') +1;
				char* s=mi_argc_con_comillas(line,len);
			  if (strcmp(s, "")!=0){
					  argumentos[a]=s;

						printf("par: i%si",  argumentos[a]);
						printf("\n");
						printf("string restante: %s", line);
						printf("\n");
						a++;
				}


	      char *substring=strchr(line, '"');
	      int l= substring-line;
	      len-=l;
	      line=substring+1;
			}

			}




	char *argumentofinal = (char*) malloc((len) * sizeof(char));

//   printf("reta: %i ",cc);
 int j;
	 for (j=0;j<(len);j++){
		 if (line[j]!='\n'){
			 argumentofinal[j]=line[j];
		 }
		 else{
			 argumentofinal[j]=0;

		 }

		 //printf("caracter: %c",line[j]);
	 }
	 //printf("string restante: i%si", argumentofinal);
  argumentos[i]=argumentofinal;
	int m=0;
	printf("________________");
	printf("\n");
	for (m=0;m<a;m++){
		printf("argumento: %s",argumentos[m]);
		printf("\n");
		printf("________________");
		printf("\n");
	}


  return argumentos;
}
