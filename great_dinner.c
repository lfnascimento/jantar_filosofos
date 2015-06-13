#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
  
#define N 5
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2
#define ESQUERDA (ph_num+4)%N
#define DIREITA (ph_num+1)%N
  
sem_t mutex;
sem_t S[N];
  
void * filosofo(void *num);
void take_fork(int);
void put_fork(int);
void tentaComer(int);
  
int state[N];
int phil_num[N]={0,1,2,3,4};
  
int main()
{
    int i;
    pthread_t thread_id[N];
    sem_init(&mutex,0,1);
    for(i=0;i<N;i++)
        sem_init(&S[i],0,0);
    for(i=0;i<N;i++)
    {
        pthread_create(&thread_id[i],NULL,filosofo,&phil_num[i]);

    }
    for(i=0;i<N;i++)
        pthread_join(thread_id[i],NULL);
}
  
void *filosofo(void *num)
{
    while(1)
    {
        int *i = num;
        printf("Filosofo %d esta pensando\n",*i+1);
        take_fork(*i);
        //sleep(0);
        put_fork(*i);
    }
}
  
void take_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = FAMINTO;
    printf("Filosofo %d esta faminto\n",ph_num+1);
    tentaComer(ph_num);
    sem_post(&mutex);
    sem_wait(&S[ph_num]);
    sleep(1);
}
  
void tentaComer(int ph_num)
{
    if (state[ph_num] == FAMINTO && state[ESQUERDA] != COMENDO && state[DIREITA] != COMENDO)
    {
        state[ph_num] = COMENDO;
        sleep(2);
        printf("Filosofo %d pega garfo %d e %d\n",ph_num+1,ESQUERDA+1,ph_num+1);
        printf("Filosofo %d esta comendo\n",ph_num+1);
        sem_post(&S[ph_num]);
    }
}
  
void put_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = PENSANDO;
    printf("Filosofo %d coloca o garfo %d and %d na mesa\n",ph_num+1,ESQUERDA+1,ph_num+1);
    printf("Filosfo %d esta pensando\n",ph_num+1);
    tentaComer(ESQUERDA);
    tentaComer(DIREITA);
    sem_post(&mutex);
}
