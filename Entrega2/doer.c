#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

typedef struct nodo {
	int valor;
	struct nodo *next;

}Nodo;



typedef struct queue {
	Nodo* head;
	Nodo* tail;
	int cantidad_de_procesos;
	int quantum;
}Queue;

void append(Queue* q, int valor);
Queue* crear_queue();
Nodo* pop(Queue* q);
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

    fp = fopen("ej.txt", "r");

    while (1) {
      //sleep(2);sleep(1);

			read = getline(&line, &len, fp);
      if (read  != -1){
	      int pid=fork();
				printf("string %s",line);
	      char**ar=argv2(line,len);
				printf("comando: %s, parametro:%s, %s",ar[0],ar[1],ar[2]);
	      if(pid==0){
	       printf("\n");
	       execvp(ar[0],ar);
	      }



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


char* mi_argv(char* line, int len){

   char *arcv_string = strchr(line, ' ');


  return arcv_string;
}

/*
void append(Queue* q, int valor){
  Nodo*p;
  p= (Nodo *) calloc(1,sizeof(Nodo));
  p->valor=valor;
  p->next=NULL;
	q->cantidad_de_procesos++;
	if(q->head==NULL){
		q->head=p;
		q->tail=p;
	}
	else{
		q->tail->next=p;
		q->tail=p;
	}
}
Nodo* pop(Queue* q){
	if(q->head!=NULL){
		Nodo* sacado;
		sacado=q->head;
		q->cantidad_de_procesos--;
		if (sacado->next==NULL){
			q->tail=NULL;
		}
		q->head=sacado->next;
		sacado->next=NULL;
		return sacado;
	}
	else{
		return NULL;
	}
}

Queue* crear_queue(){
	Queue *q;
	q= (Queue *) calloc(1,sizeof(Queue));
	q->head=NULL;
	q->tail=NULL;
	q->cantidad_de_procesos=0;

	return q;
}



int argc2(char* line, int len){
  char*line2=line;
  char*line3=line;
  Queue *espacios;
	espacios= crear_queue();
  while(strchr(line2, ' ')){
    char *substring = strchr(line2, ' ');
    int espacio_en =substring-line;
    append(espacios,espacio_en);
    line2=substring;
  }
  cantidad_de_espacios

  Queue *comillas;
  comillas=crear_queue();
  while(strchr(line3, '"')){
    char *substring = strchr(line3, '"');
    int comillaen =substring-line;
    append(comillas,comillaen);
    line=substring;
  }
  int i;
  espacios_dentro_de_comillas=0;
  for (i=0;i<len;i++){

  }
}*/
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
  for(i=0;i<arc;i++){
      argumentos[i]=mi_argc(line,len);
      char *substring=strchr(line, ' ');
      int l= substring-line;
      len-=l;
      line=substring;
  }
	char *argumentofinal = (char*) malloc((len) * sizeof(char));

//   printf("reta: %i ",cc);
 int j;
	 for (j=0;j<(len);j++){
		 argumentofinal[j]=line[j];
	 }

  argumentos[i]=argumentofinal;


  return argumentos;
}
