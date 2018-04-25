#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int argc2(char* line, int len);
char** argv2(char* line, int len);
char* mi_argc(char* line, int len);
char* mi_argv(char* line, int len);
char* mi_argc_confin(char* line, int len);
char* mi_argc_con_comillas(char* line, int len);

int main(int argc, char *argv[]){

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    //int i;
		int n = atoi(argv[2]);
    fp = fopen(argv[1], "r");
		int procesos_corriendo=0;
		int cantidad_lineas=0;
		while ( getline(&line, &len, fp)!=-1) {
			//sleep(2);
			cantidad_lineas++;


		}
		fclose(fp);
		printf("cantidad:%i",cantidad_lineas);
		char***lineas_guardadas= (char***) calloc((cantidad_lineas) , sizeof(char**));

		int *cantidad_parametros;
		cantidad_parametros=(int*) calloc((cantidad_lineas) , sizeof(int));

  	fp = fopen(argv[1], "r");
		int l=-1;
    while (1) {
      //sleep(2);


			read = getline(&line, &len, fp);
      if (read  != -1){
				l++;


				//printf("string %s",line);
				//printf("\n");
	      char**ar=argv2(line,len);
				lineas_guardadas[l]=ar;

        free(line);
				//printf("comando: %s, parametro:%s , %s",ar[0],ar[1],ar[2]);


			}
			else{break;}
    }

    fclose(fp);

		int h=0;
		for (h=0;h<cantidad_lineas;h++){
			sleep(1);
			char**ar=lineas_guardadas[h];
			//printf("comando: %s, parametro:%s , %s",ar[0],ar[1],ar[2]);

			if(procesos_corriendo==n){
				wait(NULL);
				procesos_corriendo--;
			}
			procesos_corriendo++;
			int pid=fork();
			if(pid==0){
			 printf("\n");

		 // ar[1]="-al";
		 printf("funcion: %s",ar[1]);
			execvp(ar[0],ar);
		}

		}

		for (h=0;h<cantidad_lineas;h++){
			int j=0;
			for(j=1;j<3;j++){
				free(lineas_guardadas[h][j]);
			}
			free(lineas_guardadas[h]);


		}
		free(lineas_guardadas);
    free(cantidad_parametros);




























sleep(3);

    return 0;///fin main
}
char* mi_argc(char* line, int len){
  char *argc = (char*) calloc((len) , sizeof(char));
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
  char *argc = (char*) calloc((len) ,sizeof(char));
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

char* mi_argc_confin(char* line, int len){
  char *argc = (char*) calloc((len) , sizeof(char));
  char *ret;
   ret = strchr(line, '\n');
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
//char* lll=line;
	int arc=argc2(line,len);


//  printf("%i",arc);
  char**argumentos=(char**) calloc((arc+2) , sizeof(char*));
	//argumentos[0]="time";
	int i;
	int a=0;
	 /* for(i=0;i<arc+1;i++){
			argumentos[i]=NULL;
		}*/

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

						/*printf("par: i%si",  argumentos[a]);
						printf("\n");
						printf("string restante: %s", line);
						printf("\n");*/
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

						/*printf("par: i%si",  argumentos[a]);
						printf("\n");
						printf("string restante: %s", line);
						printf("\n");*/
						a++;
				}


	      char *substring=strchr(line, '"');
	      int l= substring-line;
	      len-=l;
	      line=substring+1;
			}

			}


char* s=mi_argc_confin(line,len);
//char* s="";
	 printf("iiiiii: %i\n", a);
	 if (strcmp(s, "")!=0){
 			argumentos[a]=s;
		}
		else{
			argumentos[a]=NULL;
		}
	int m=0;
	printf("________________");
	printf("\n");
	for (m=0;m<a+1;m++){
		printf("argumento: %s",argumentos[m]);
		printf("\n");
		printf("________________");
		printf("\n");
	}
//free(lll);
printf("aaaa:%i ",a);

  return argumentos;
}
