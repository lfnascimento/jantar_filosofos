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
                        char *);
int food_on_table ();

int numero_filosofos;

pthread_mutex_t *garfo;
pthread_t *filo;

pthread_mutex_t food_lock;
int sleep_seconds = 0;

int tempoComendo;
int tempoPensando;

int filosofoAleatorio;

int
main (int argn,
      char **argv)
{
    int i;

    printf("Informe o numero de filosofos:\n");
    scanf("%d", &numero_filosofos);

    filosofoAleatorio = rand() % numero_filosofos;

    garfo = (pthread_mutex_t *) malloc(numero_filosofos*sizeof(pthread_mutex_t));
    filo = (pthread_t *) malloc(numero_filosofos*sizeof(pthread_t));

    printf("Informe o tempo gasto do filosfo pensando:\n");
    scanf("%d", &tempoPensando);

    printf("Informe o tempo gasto do filosfo comendo:\n");
    scanf("%d", &tempoComendo);

    if (argn == 2)
        sleep_seconds = atoi (argv[1]);

    pthread_mutex_init (&food_lock, NULL);
    for (i = 0; i < numero_filosofos; i++)
        pthread_mutex_init (&garfo[i], NULL);
    for (i = 0; i < numero_filosofos; i++)
        pthread_create (&filo[i], NULL, filosofo, (void *)i);
    for (i = 0; i < numero_filosofos; i++)
        pthread_join (filo[i], NULL);
    return 0;
}

void *
filosofo (void *num)
{
    int id;
    int i, garfoEsquerdo, garfoDireito, f;

    id = (int)num;
    printf ("Filosofo %d esta pensando.\n", id);
    sleep(tempoPensando);
    garfoDireito = id;
    garfoEsquerdo = id + 1;

    /* Wrap around the chopsticks. */
    if (garfoEsquerdo == numero_filosofos)
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

        devolveGarfo(id, garfoDireito,  "direita");
        pegaGarfo (id, garfoDireito, "direita");

        if (filosofoAleatorio != id) {
            devolveGarfo(id, garfoDireito,  "direita");
            pegaGarfo(id, garfoEsquerdo, "esquerda");

        } else {
            printf("Filosofo %d: COMENDO.\n", id);
            sleep(2);
            devolveGarfo(id, garfoDireito,  "direita");
            devolveGarfo(id, garfoEsquerdo,  "esquerda");
            filosofoAleatorio = rand() % numero_filosofos;
            printf("Filoso %d esta PENSANDO\n",id);
            sleep(tempoPensando);
        }

        pegaGarfo(id, garfoDireito, "direita");
        printf ("Filosfo %d: COMENDO.\n", id);
        sleep(2);

        devolveGarfo(id, garfoDireito,  "direita");
        devolveGarfo(id, garfoEsquerdo,  "esquerda");
        printf("Filosofo %d esta PENSANDO\n",id);
        sleep(tempoPensando);

        usleep (DELAY * (FOOD - f + 1));
    }

    printf ("Filosofo %d acabou de comer.\n", id);
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
pegaGarfo (int filo,
                int g,
                char *lado)
{
    pthread_mutex_lock (&garfo[g]);
    printf ("Filosfo %d FAMINTO: pega  o garfo %d da %s\n", filo, g, lado);
}

void
devolveGarfo (int filo, int g, char *lado)

{
    pthread_mutex_unlock(&garfo[g]);
    printf("Filosfo %d devolve o garfo %d da %s\n", filo, g, lado);
}
