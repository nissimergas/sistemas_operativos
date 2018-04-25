#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct proceso {
	int argc;
  char** argv;
	int status;
  int intentos;
  int exit_code;
  int pid;

}Proceso;

int argc2(char* line, int len);
char** argv2(char* line, int len);
char* mi_argc(char* line, int len);
char* mi_argv(char* line, int len);
char* mi_argc_confin(char* line, int len);
char* mi_argc_con_comillas(char* line, int len);
Proceso* crear_proceso(int ac, char** av);
void destruir_proceso(Proceso*p);

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
		Proceso**procesos= (Proceso**) calloc((cantidad_lineas) , sizeof(Proceso*));

  	fp = fopen(argv[1], "r");
		int l=-1;
    while (1) {
      //sleep(2);


			read = getline(&line, &len, fp);
      if (read  != -1){
				l++;


				//printf("string %s",line);
				//printf("\n");
	      int cp=argc2(line,len);



      //  printf("cp: %i ",cp);
        //printf("\n");
        char**ar=argv2(line,len);

        procesos[l]=crear_proceso(cp, ar);
				printf("\n");


        //free(line);
			}
			else{break;}
    }
    free(line);

    fclose(fp);
    int status;
		int h=0;
		for (h=0;h<cantidad_lineas;h++){ //CORRER POR PRIMERA VEZ EL COMANDO
			//sleep(1);
			char**ar=procesos[h]->argv;
      procesos[h]->intentos++;
			//printf("comando: %s, parametro:%s , %s",ar[0],ar[1],ar[2]);

			if(procesos_corriendo==n){
				int p_esperado=wait(&status);
        int hi;
        for (hi=0;hi<cantidad_lineas;hi++){
          Proceso* proceso_ed=proceso_ed=procesos[hi];
          if(proceso_ed->pid==p_esperado){
            proceso_ed->status=WEXITSTATUS(status);
          }
        }
				procesos_corriendo--;
			}
			procesos_corriendo++;
			procesos[h]-> pid=fork();
			if(procesos[h]-> pid==0){
			 printf("\n");

		 // ar[1]="-al";
		 printf("funcion: %s",ar[1]);
			execvp(ar[0],ar);
		}

		}
    procesos_corriendo=0;
    for (h=0;h<cantidad_lineas;h++){//ESPERAR POR LOS PROCESOS;
       Proceso* p=procesos[h];
       if(p->status!=0){
          waitpid(p->pid,&status,0);
          p->status=WEXITSTATUS(status);
        }
    }


    for (h=0;h<cantidad_lineas;h++){ //CORRER POR segunda VEZ EL COMANDO
      //sleep(1);
      char**ar=procesos[h]->argv;
      if(procesos[h]->status!=0){
        procesos[h]->intentos++;
        //printf("comando: %s, parametro:%s , %s",ar[0],ar[1],ar[2]);

        if(procesos_corriendo==n){
          int p_esperado=wait(&status);
          int hi;
          for (hi=0;hi<cantidad_lineas;hi++){
            Proceso* proceso_ed=proceso_ed=procesos[hi];
            if(proceso_ed->pid==p_esperado){
              proceso_ed->status=WEXITSTATUS(status);
            }
          }
          procesos_corriendo--;
        }
        procesos_corriendo++;
        procesos[h]-> pid=fork();
        if(procesos[h]-> pid==0){
         printf("\n");

         // ar[1]="-al";
         printf("funcion: %s",ar[1]);
          execvp(ar[0],ar);
        }
      }

    }
		for (h=0;h<cantidad_lineas;h++){//ESPERAR POR LOS PROCESOS 2vez;
			 Proceso* p=procesos[h];
			 if(p->status!=0){
					waitpid(p->pid,&status,0);
					p->status=WEXITSTATUS(status);
				}
		}

    //int h=0;
		for (h=0;h<cantidad_lineas;h++){
      destruir_proceso(procesos[h]);
    }
    free(procesos);




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

  int a=0;

  while(1){
    int posicion_comilla=10000000;
    int posicion_espacio=10000000;
    int posicion_fin=10000000;
    char *ret;
    ret = strchr(line, ' ');
     if(ret !=NULL){
         posicion_espacio=ret-line;
       }
       ret = strchr(line, '"') ;
     if(ret != NULL ){
         posicion_comilla=ret-line;
       }
       ret = strchr(line, '\n') ;
     if(ret != NULL ){
         posicion_fin=ret-line;
       }

      if(posicion_espacio<posicion_comilla){
        char* s=mi_argc(line,len);
        if (strcmp(s, "")!=0){
            a++;
        }
        free(s);
        char *substring=strchr(line, ' ');
        int l= substring-line;
        len-=l;
        line=substring+1;
      }
      else if(posicion_espacio>posicion_comilla){
        line=strchr(line, '"') +1;
        char* s=mi_argc_con_comillas(line,len);
        if (strcmp(s, "")!=0){
            a++;
        }
        free(s);
        char *substring=strchr(line, '"');
        int l= substring-line;
        len-=l;
        line=substring+1;
      }
      else{
        char* s=mi_argc_confin(line,len);
        if (strcmp(s, "")!=0){
            a++;
        }
        free(s);
        return a;
      }

      }


  return a;

}
































char **argv2(char* line, int len){
//char* lll=line;
	int arc=argc2(line,len);


//  printf("%i",arc);
  char**argumentos=(char**) calloc((arc) , sizeof(char*));
	//argumentos[0]="time";

	int a=0;
	 /* for(i=0;i<arc+1;i++){
			argumentos[i]=NULL;
		}*/

  //for(i=0;i<arc;i++){
  while(1){
    if(len<=0){
      break;
    }

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
            //printf("argumento: %s",s);
        		//printf("\n");
						a++;
				}
        else{
          free(s);
        }
	      char *substring=strchr(line, ' ');
	      int l= substring-line;
	      len-=l;
	      line=substring+1;
			}
			else if(posicion_espacio>posicion_comilla){
				line=strchr(line, '"') +1;
				char* s=mi_argc_con_comillas(line,len);
			  if (strcmp(s, "")!=0){
					  argumentos[a]=s;
            //printf("argumento2: %s",s);
        		//printf("\n");
						a++;
				}
        else{
          free(s);
        }
	      char *substring=strchr(line, '"');
	      int l= substring-line;
	      len-=l;
	      line=substring+1;
			}
      else{
			  char* s=mi_argc_confin(line,len);
			  if (strcmp(s, "")!=0  ){
          //printf("argumento: %s",s);
          //printf("\n");
					  argumentos[a]=s;
						a++;
            break;
				}
        else{
          free(s);
          break;
        }
	      char *substring=strchr(line, '\n');
	      int l= substring-line;
	      len-=l;
	      line=substring+1;
			}


			}
      //free(line);
/*int m;
	for (m=0;m<arc;m++){
		printf("argumento: %s",argumentos[m]);
		printf("\n");
		printf("________________");
		printf("\n");
	}*/
//free(lll);

  return argumentos;
}

Proceso* crear_proceso(int ac, char** av){
  Proceso *p=(Proceso*) calloc(1 , sizeof(Proceso));
  p->argc=ac;
  p->argv=av;
  p->status=1;
  p->intentos=0;
  p->pid=0;
  p->exit_code=0;
  return p;
}

void destruir_proceso(Proceso*p){
   printf("_________________________________\n");
    printf("intentos,: %i \n",p->intentos);
    char** ar=p->argv;
    int cp=p->argc;
    int b;
    for(b=0;b<cp;b++){
      free(ar[b]);
    }
    free(ar);
    free(p);

}
