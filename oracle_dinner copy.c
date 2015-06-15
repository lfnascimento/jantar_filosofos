#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define NUMERO_FILOSOFOS 5
#define DELAY 2
#define FOOD 50

void *filosofo (void *id);
void pegaGarfo (int,
                     int,
                     char *);
void devolveGarfo (int,
                      int,
                      int, char *);
int food_on_table ();

pthread_mutex_t chopstick[NUMERO_FILOSOFOS];
pthread_t philo[NUMERO_FILOSOFOS];
pthread_mutex_t food_lock;
int sleep_seconds = 0;

int filosofoAleatorio;

int
main (int argn,
      char **argv)
{
    int i;

    filosofoAleatorio = rand() % NUMERO_FILOSOFOS;

    if (argn == 2)
        sleep_seconds = atoi (argv[1]);

    pthread_mutex_init (&food_lock, NULL);
    for (i = 0; i < NUMERO_FILOSOFOS; i++)
        pthread_mutex_init (&chopstick[i], NULL);
    for (i = 0; i < NUMERO_FILOSOFOS; i++)
        pthread_create (&philo[i], NULL, filosofo, (void *)i);
    for (i = 0; i < NUMERO_FILOSOFOS; i++)
        pthread_join (philo[i], NULL);
    return 0;
}

void *
filosofo (void *num)
{
    int id;
    int i, garfoEsquerdo, garfoDireito, f;

    id = (int)num;
    printf ("Philosopher %d is done thinking and now ready to eat.\n", id);
    garfoDireito = id;
    garfoEsquerdo = id + 1;

    /* Wrap around the chopsticks. */
    if (garfoEsquerdo == NUMERO_FILOSOFOS)
        garfoEsquerdo = 0;

    while (f = food_on_table ()) {
    //while (1) {
        /* Thanks to philosophers #1 who would like to take a nap
           * before picking up the chopsticks, the other philosophers
           * may be able to eat their dishes and not deadlock.
             */
        //if (id == 1)
          //  sleep (sleep_seconds);

        if (filosofoAleatorio == id)
            pegaGarfo (id, garfoEsquerdo, "esquerda");
        else
            pegaGarfo (id, garfoDireito, "direita ");

        devolveGarfo(id, garfoDireito, garfoEsquerdo, "direita");
        pegaGarfo (id, garfoDireito, "direita");

        if (filosofoAleatorio != id) {
            devolveGarfo(id, garfoDireito, garfoEsquerdo, "direita");
            pegaGarfo(id, garfoEsquerdo, "esquerda");

        } else {
            printf("Filosofo %d: COMENDO.\n", id);
            devolveGarfo(id, garfoDireito, garfoEsquerdo, "direita");
            devolveGarfo(id, garfoEsquerdo, garfoEsquerdo, "esquerda");
            filosofoAleatorio = rand() % NUMERO_FILOSOFOS;
            printf("Filoso %d esta PENSANDO\n",id);
        }

        pegaGarfo(id, garfoDireito, "direita");
        printf ("Filosfo %d: COMENDO.\n", id);
        devolveGarfo(id, garfoDireito, garfoEsquerdo, "direita");
        devolveGarfo(id, garfoEsquerdo, garfoEsquerdo, "esquerda");
        printf("Filosofo %d esta PENSANDO\n",id);

        //usleep (DELAY * (FOOD - f + 1));
    }

    printf ("Philosopher %d is done eating.\n", id);
    return (NULL);
}

int
food_on_table ()
{
    static int food = FOOD;
    int myfood;

    pthread_mutex_lock (&food_lock);
    if (food > 0) {
        food--;
    }
    myfood = food;
    pthread_mutex_unlock (&food_lock);
    return myfood;
}

void
pegaGarfo (int phil,
                int c,
                char *hand)
{
    pthread_mutex_lock (&chopstick[c]);
    printf ("Filosfo %d FAMINTO: pega  o garfo %d da %s\n", phil, c, hand);
}

void
devolveGarfo (int phil, int c1, int c2, char *hand)

{
    //if (phil != ph_num_aleatorio) {
        printf("Filosfo %d devolve o garfo %d da %s\n", phil, c1, hand);
        pthread_mutex_unlock(&chopstick[c1]);
    //printf("Filosofo %d esta PENSANDO\n",phil);
    //}
    //pthread_mutex_unlock (&chopstick[c2]);

}