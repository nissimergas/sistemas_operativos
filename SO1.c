#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct process {
	int pid;
	char *name;
	int state;
	struct process *next;
	int tiempo_inicio;
	int *rafagas;
	int N;
	int turnos_en_cpu;
	int bloqueos;
	int tiempo_en_cpu;
	int rafaga_actual;
}Process;

typedef struct queue {
	Process* head;
	Process* tail;
	int cantidad_de_procesos;
}Queue;

typedef struct array_list {
	Process* *lista;
	int ultimo_elemento;
	int largo_lista;
}Array_list;

int divicion_entera(int a, int b);
Queue* crear_queue();
Process* crear_proceso(int id, char nombre[256],int estado,int N); //estado=0,1,2
Array_list* crear_array_list();
void append_array_list(Array_list* array,Process* p);
void up_heap(Array_list* array);
void down_heap(Array_list* array, int k);
Process*  max_heap(Array_list* array);

void agregar_rafaga(Process* p,int i,int rafaga);
void append(Queue* q, Process* p);
Process* pop(Queue* q);
void imprimir_proceso(Process* p);
void destruir_proceso(Process* p);
void aaa();
//////////////////////////////////////////////////////main//////////////////////////////////////////////////////////////////
Array_list* array;
int main(int argc, char *argv[]){
	/*Process* pro;
	pro=crear_proceso(1, "Nissim", 0,2);
	imprimir_proceso(pro);
	free(pro);*/

char * version = argv[1];
int quantum = atoi(argv[3]);
int quantum2 = atoi(argv[3]);
int n_queues = atoi(argv[4]);
printf("%s \n", version);
printf("%d \n", quantum);
printf("%d \n", n_queues);
FILE* archivo_procesos = fopen(argv[2], "r");
if(!archivo_procesos)
{
	perror(argv[2]);
	return 2;
}
//cosas que sacare del archivo
char nombre[256];
//nombre="hola";
int N;
int tiempo;
//int c;
//Array_list* array= crear_array_list();
array=crear_array_list();
int id = 1;

while((fscanf(archivo_procesos, "%s %d %d", nombre, &tiempo,&N))==3){
	printf("Nombre: %s\n",nombre);

	Process* pro;
	pro=crear_proceso(id, nombre, tiempo,N);
	id ++;
	int i=1;

	for( i=0;i<N;i++){
		int rafaga;
		fscanf(archivo_procesos, "%d",&rafaga);
		agregar_rafaga( pro, i, rafaga);
	}
	append_array_list(array, pro);
}
// ultima cola es la cola finished
Queue* *colas = (Queue**) malloc(n_queues * sizeof(Queue*));
int i;
for( i = 0; i< (n_queues + 1); i++) {
	colas[i] = crear_queue();
}

int time = 0;
Process* proceso_en_cpu=NULL;
int prioridad_proceso;
int cantidad_de_procesos_creados=array->ultimo_elemento;
printf("creados: %i \n",cantidad_de_procesos_creados);
while(1){

	//revisar si llegan procesos nuevos
	if(array->ultimo_elemento>0){
		while(1){
				if(array->ultimo_elemento>0){
					if(array->lista[1]->tiempo_inicio==time){
						Process* p;
						p=max_heap(array);
						p->state=1;//Ready
						append(colas[n_queues],p);
					}
					else{
						break;
					}
			}
			else{
				break;
			}
		}
	}
	//meter proceso a la cpu
	if(proceso_en_cpu==NULL){
		int prioridad=n_queues;
		while(prioridad>0 ){
			if(colas[prioridad]->head!=NULL){
				proceso_en_cpu=pop(colas[prioridad]);
				proceso_en_cpu->turnos_en_cpu++;
				proceso_en_cpu->state=2; //Running
				prioridad_proceso=prioridad;
				break;
			}
			prioridad--;
		}
		if(prioridad==0){
			proceso_en_cpu=NULL;
		}
	}//fin meter proceso a la cpu
	/*int turnos_en_cpu; si
	int bloqueos;
	int tiempo_en_cpu;si
	int rafaga_actual;si*/
	else{
		quantum--;
		proceso_en_cpu->rafagas[proceso_en_cpu->rafaga_actual]--;
		proceso_en_cpu->tiempo_en_cpu++;
		//SI SE COMPLETA RAFAGA DE TIEMPO
		if(proceso_en_cpu->rafagas[proceso_en_cpu->rafaga_actual]==0){
			proceso_en_cpu->rafaga_actual++;
			//si es que no quedan mas rafagas
			if(proceso_en_cpu->rafaga_actual==proceso_en_cpu->N){
				append(colas[0],proceso_en_cpu);
				proceso_en_cpu->state=3;//finished
				//colas[0]->cantidad_de_procesos=colas[0]->cantidad_de_procesos+1;
				printf("terminados: %i \n",colas[0]->cantidad_de_procesos);
				//sleep(1);
			}
			//si es que quedan mas rafagas.
			else{
				append(colas[prioridad_proceso],proceso_en_cpu);
			}
			proceso_en_cpu->state=1;//readdy
			proceso_en_cpu=NULL;
			quantum=quantum2;
		}
		//SI SE COMPLETA QUANTUM
		if(quantum==0 && proceso_en_cpu!=NULL){
			//baja la prioridad del proceso;
			if(prioridad_proceso>1){
				prioridad_proceso--;
			}
			proceso_en_cpu->bloqueos++;
			append(colas[prioridad_proceso],proceso_en_cpu);
			proceso_en_cpu->state=1;
			proceso_en_cpu=NULL;
			quantum=quantum2;
		}

	}
	time ++;
	/*if(tiempo  == 100){
		break;
	}*/
	//condicion de termino
	//printf("terminados: %i \n",colas[0]->cantidad_de_procesos);
	if(colas[0]->cantidad_de_procesos==cantidad_de_procesos_creados){
		break;
	}
}
int m;
Process* p=colas[0]->head;
//imprimir_proceso(p);
for(m=1;m<cantidad_de_procesos_creados;m++){
	if(p!=NULL){
		Process* aux=p->next;
		imprimir_proceso(p);
		destruir_proceso(p);
		p=aux;
	}
	//imprimir_proceso(array->lista[i]);
	//destruir_proceso(array->lista[i]);

}
//aaa();
/*
int i;
int h=array->ultimo_elemento;
for(i=0;i<h;i++){
  Process* p= max_heap(array);
	imprimir_proceso(p);
	destruir_proceso(p);
	//imprimir_proceso(array->lista[i]);
	//destruir_proceso(array->lista[i]);

}
free(array->lista);
free(array);*/


fclose(archivo_procesos);
	return 0;
}
//////////////////////////////////////////////////////end_main///////////////////////////////////////////////////////////////
int divicion_entera(int a, int b){
	int resto;
	resto=a%b;
	return (a-resto)/b;
}

Queue* crear_queue(){
	Queue *q;
	q= (Queue *) calloc(1,sizeof(Queue));
	q->head=NULL;
	q->tail=NULL;
	q->cantidad_de_procesos=0;

	return q;
}

Array_list* crear_array_list(){
	Array_list *array;
	array= (Array_list *) calloc(1,sizeof(Array_list));
	array->ultimo_elemento=0;
	array->largo_lista=3;
	array->lista=(Process* *) malloc((array->largo_lista) * sizeof(Process*));
	int i;
	for( i=0;i<array->largo_lista; i++){
		array->lista[i]=NULL;
	}
	return array;
}

void append_array_list(Array_list* array,Process* p){
	if(array->ultimo_elemento< array->largo_lista-1){
		array->ultimo_elemento++;
		array->lista[array->ultimo_elemento]=p;
	}
	else{
		int largo2=array->largo_lista*2;
		Process** lista2=(Process**  ) malloc((largo2) * sizeof(Process*));
		int i;
		for(i=0;i<array->largo_lista;i++){
			lista2[i]=array->lista[i];
		}
		free(array->lista);
		array->largo_lista=largo2;
		array->lista=lista2;
		array->ultimo_elemento++;
		array->lista[array->ultimo_elemento]=p;
	}
	up_heap(array);
}

Process* crear_proceso(int id, char* nombre,int tiempo, int N){ //estado=0,1,2
	Process*nuevo;
	nuevo=(Process *) calloc(1,sizeof(Process));
	nuevo->rafagas=(int *) malloc(N*sizeof(int));;
	nuevo->pid=id;
	//int largo=strlen(nombre);    FALTA RESTRICCION DE LARGO
	//nuevo->name=(char *)malloc(256*sizeof(char));
	nuevo->name=strdup(nombre);
	nuevo->state=0;
	nuevo->tiempo_inicio=tiempo;
	nuevo->next=NULL;
	nuevo->N=N;
	nuevo-> turnos_en_cpu=0;
	nuevo-> bloqueos=0;
	nuevo-> tiempo_en_cpu=0;
	nuevo->rafaga_actual=0;
	return nuevo;
}
void agregar_rafaga(Process* p,int i,int rafaga){
	p->rafagas[i]=rafaga;
}

void imprimir_proceso(Process* p){

	printf("Nombre: %s\n",p->name);
	printf("inicio: %i\n",p->tiempo_inicio);
	printf("id: %i\n",p->pid);
	if (p->state==1){
		printf("readdy: %i\n",p->state);
	}
	if (p->state==2){
		printf("running: %i\n",p->state);
	}
	if (p->state==3){
		printf("finished: %i\n",p->state);
	}

	printf("bloqueos: %i\n",p->bloqueos);
	printf("turnos en cpu: %i\n",p->turnos_en_cpu);
	printf("rafagas:");
	int i;
	for( i=0;i<p->N;i++){
		printf("%i ",p->rafagas[i]);
	}
	printf("\n");
	printf("\n");
}

void up_heap(Array_list* array){
	int k=array->ultimo_elemento;
  int j=divicion_entera(k, 2);
	while(k>1 && (array->lista[k]->tiempo_inicio)<(array->lista[j]->tiempo_inicio
	)){
			Process* aux;
			aux=array->lista[j];
			array->lista[j]=array->lista[k];
			array->lista[k]=aux;
			k=j;
	}
}
void down_heap(Array_list* array, int k){
	while(2*k<=array->ultimo_elemento){
		int j=2*k;

			if (j < array->ultimo_elemento){
				if( (array->lista[j]->tiempo_inicio)>(array->lista[j+1]->tiempo_inicio) ){
					j++;
				}
			}
			if(array->lista[k]->tiempo_inicio<=array->lista[j]->tiempo_inicio){
				break;
			}
			else{
				Process* aux;
				aux=array->lista[j];
				array->lista[j]=array->lista[k];
				array->lista[k]=aux;
				k=j;
			}


	}
}
Process*  max_heap(Array_list* array){
	Process* max=array->lista[1];
	array->lista[1]=array->lista[array->ultimo_elemento];
	array->lista[array->ultimo_elemento]=max;
	array->ultimo_elemento--;
	down_heap(array,1);
	return max;
}

void append(Queue* q, Process* p){
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
Process* pop(Queue* q){
	if(q->head!=NULL){
		Process* sacado;
		sacado=q->head;
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
void destruir_proceso(Process* p){
	free(p->rafagas);
	free(p->name);
	free(p);
}
void aaa(){
	int i;
	int h=array->ultimo_elemento;
	for(i=0;i<h;i++){
	  Process* p= max_heap(array);
		imprimir_proceso(p);
		destruir_proceso(p);
		//imprimir_proceso(array->lista[i]);
		//destruir_proceso(array->lista[i]);

	}
	free(array->lista);
	free(array);
}
