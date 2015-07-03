#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define Pensando 0
#define Faminto 1
#define Comendo 2
 
int i,estado[1000], n_filosofos, tempo_comendo, tempo_pensando;
 
//semáforos
sem_t mutex;
sem_t sem_fil[1000];

//Cabeçalhos das funções
int calcular_esquerda(int meio, int total);
int calcular_direita (int meio, int total);
int gerar_entrada (int entrada);
void mostrar();
void *acao_filosofo(void *j);
void pegar_garfo(int i);
void por_garfo(int i);
void test(int i);
void pensar();
void comer();
void entrada();
void core();
void debug_random();

//Calcula a posição do filosofo a esquerda
int calcular_esquerda(int meio, int total){
    int esquerda;
    esquerda = (meio+total-1)%total;
    return esquerda;
}
//Calcula a posicao do filosofo a direita
int calcular_direita (int meio, int total){
    int direita;
    direita = (meio+1)%total;
    return direita;
}
//Gerar tempo médio
int gerar_tempo_medio(int entrada){
    srand(time(NULL));
    return 1 + rand() %((entrada*1000) +1);
}
//Funcao que mostra o estado dos filosofos
void mostrar(){
   for(i=1; i<=n_filosofos; i++){
       if(estado[i-1] == Pensando){
          printf("O filósofo %d esta pensando!\n", i);
       }
       if(estado[i-1] == Faminto){
          printf("O filósofo %d esta com fome!\n", i);
       }
       if(estado[i-1] == Comendo){
          printf("O filósofo %d esta comendo!\n", i);
       }
   }
   printf("\n");
}
//acao do filósofo
void *acao_filosofo(void *j){
    int i= *(int*) j;
    while(1){  
        pensar(i);
        pegar_garfo(i);
        comer(i);
        por_garfo(i);
    }
}
 
void pegar_garfo(int i){
    sem_wait(&mutex); //Pega o semáforo pra si
    estado[i]=Faminto; //Seta que está com fome
    mostrar(); //Mostra o estado dos outros filosofos
    test(i); //Tenta pegar os garfos pra comer
    sem_post(&mutex); //Libera o simáforo
    sem_wait(&sem_fil[i]); //Atualiza seu próprio semáforo
}
 
void por_garfo(int i){
    int esquerda, direita;
    sem_wait(&mutex);
    estado[i]=Pensando;
    mostrar();
    esquerda = calcular_esquerda(i, n_filosofos);
    direita = calcular_direita(i, n_filosofos);
    test(esquerda);
    test(direita);
    sem_post(&mutex);
}
 
//funcao que testa se o filósofo pode comer
void test(int i){
   int esquerda, direita;
   esquerda = calcular_esquerda(i, n_filosofos);
   direita = calcular_direita(i, n_filosofos);
   if(estado[i] == Faminto && estado[esquerda] != Comendo && estado[direita] != Comendo){
       estado[i]=Comendo;
       mostrar();
       sem_post(&sem_fil[i]);
   } 
}
 
//a thread(filósofo) espera um tempo aleatoria pensando ou comendo
void pensar(){ 
    usleep(gerar_tempo_medio(tempo_pensando));
}
 
void comer(){
    usleep(gerar_tempo_medio(tempo_comendo));
}


void core(){
    //Inicializa o estado
    for(i = 0; i < n_filosofos; i++){
        estado[i]=0;
    }
    mostrar();
    pthread_t thread[1000];
    void *thread_result;
 
    //inicia os semáforos
    sem_init(&mutex,0,1);
     
    for(i = 0; i < n_filosofos; i++){
        sem_init(&sem_fil[i],0,0);
    }
 
    //cria as threads(filosofos)
    for(i = 0; i < n_filosofos; i++){
       pthread_create(&thread[i],NULL,acao_filosofo,&i);
    }
 
    //faz um join nas threads
    for(i = 0; i < n_filosofos; i++){
        pthread_join(thread[i],&thread_result);
    
    }
}

void main(){
	system("clear");
	printf("Informe o Número de filósofos: ");
        scanf("%d",&n_filosofos);
 	printf("Informe o tempo médio que cada filósofo gastará comendo (em milissegundos): ");
        scanf("%d",&tempo_pensando);
 	printf("Informe o tempo médio que cada filósofo gastará pensando (em milissegundos): ");
        scanf("%d",&tempo_comendo);
	system("clear");
	core();
	
}
