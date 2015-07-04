#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2
 
int i,estado[1000], NumeroDeFilosofos, tempoComendo, tempoPensando;
 
//semáforos
sem_t mutex;
sem_t sem_fil[1000];


void main(){

	printf("Informe o Número de filósofos: ");
	scanf("%d",&NumeroDeFilosofos);
 	printf("Informe o tempo médio que cada filósofo gastará comendo, em milissegundos: ");
	scanf("%d",&tempoPensando);
 	printf("Informe o tempo médio que cada filósofo gastará pensando, em milissegundos: ");
	scanf("%d",&tempoComendo);

	//Inicializa o estado
	for(i = 0; i < NumeroDeFilosofos; i++){
    	estado[i]=0;
	}
	mostrar();
	pthread_t thread[1000];
	void *thread_result;
 
	//inicia os semáforos
	sem_init(&mutex,0,1);
	 
	for(i = 0; i < NumeroDeFilosofos; i++){
    	sem_init(&sem_fil[i],0,0);
	}
 
	//cria as threads(filosofos)
	for(i = 0; i < NumeroDeFilosofos; i++){
   	pthread_create(&thread[i],NULL,filosofo,&i);
	}
 
	//faz um join nas threads
	for(i = 0; i < NumeroDeFilosofos; i++){
    	pthread_join(thread[i],NULL);
    
	}

}


//Calcula a posição do filosofo a esquerda
int calcularEsquerda(int meio, int total){
	int esquerda;
	esquerda = (meio+total-1)%total;
	return esquerda;
}
//Calcula a posicao do filosofo a direita
int calcularDireita (int meio, int total){
	int direita;
	direita = (meio+1)%total;
	return direita;
}
//Gerar tempo médio
int calcularTempoMedio(int entrada){
	srand(time(NULL));
	return 1 + rand() %((entrada*1000) +1);
}
//Funcao que mostra o estado dos filosofos
void mostrar(){
   for(i=1; i<=NumeroDeFilosofos; i++){
   	if(estado[i] == PENSANDO){
      	printf("O filósofo %d esta pensando...\n", i);
   	}
   	if(estado[i] == FAMINTO){
      	printf("O filósofo %d esta com fome...\n", i);
   	}
   	if(estado[i] == COMENDO){
      	printf("O filósofo %d esta comendo!\n", i);
   	}
   }
   printf("\n");
}
//acao do filósofo
void *filosofo(void *j){
	int i= *(int*) j;
	while(1){  
    	pensar(i);
    	pegarGarfo(i);
    	comer(i);
    	devolverGarfo(i);
	}
}
 
void pegarGarfo(int i){
	sem_wait(&mutex); //Pega o semáforo pra si
	estado[i]=FAMINTO; //Seta que está com fome
	mostrar(); //Mostra o estado dos outros filosofos
	intencao(i); //Tenta pegar os garfos pra comer
	sem_post(&mutex); //Libera o simáforo
	sem_wait(&sem_fil[i]); //Atualiza seu próprio semáforo
}
 
void devolverGarfo(int i){
	int esquerda, direita;
	sem_wait(&mutex);
	estado[i]=PENSANDO;
	mostrar();
	esquerda = calcularEsquerda(i, NumeroDeFilosofos);
	direita = calcularDireita(i, NumeroDeFilosofos);
	intencao(esquerda);
	intencao(direita);
	sem_post(&mutex);
}
 
//funcao que testa se o filósofo pode comer
void intencao(int i){
   int esquerda, direita;
   esquerda = calcularEsquerda(i, NumeroDeFilosofos);
   direita = calcularDireita(i, NumeroDeFilosofos);
   if(estado[i] == FAMINTO && estado[esquerda] != COMENDO && estado[direita] != COMENDO){
   	estado[i]=COMENDO;
   	mostrar();
   	sem_post(&sem_fil[i]);
   }
}
 
//a thread(filósofo) espera um tempo aleatoria pensando ou comendo
void pensar(){
	usleep(calcularTempoMedio(tempoPensando));
}
 
void comer(){
	usleep(calcularTempoMedio(tempoComendo));
}
