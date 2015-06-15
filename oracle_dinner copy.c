#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define PHILOS 5
#define DELAY 2
#define FOOD 50

void *philosopher (void *id);
void grab_chopstick (int,
                     int,
                     char *);
void down_chopsticks (int,
                      int,
                      int, char *);
int food_on_table ();

pthread_mutex_t chopstick[PHILOS];
pthread_t philo[PHILOS];
pthread_mutex_t food_lock;
int sleep_seconds = 0;

int ph_num_aleatorio;

int
main (int argn,
      char **argv)
{
    int i;

    ph_num_aleatorio = rand() % 5;

    if (argn == 2)
        sleep_seconds = atoi (argv[1]);

    pthread_mutex_init (&food_lock, NULL);
    for (i = 0; i < PHILOS; i++)
        pthread_mutex_init (&chopstick[i], NULL);
    for (i = 0; i < PHILOS; i++)
        pthread_create (&philo[i], NULL, philosopher, (void *)i);
    for (i = 0; i < PHILOS; i++)
        pthread_join (philo[i], NULL);
    return 0;
}

void *
philosopher (void *num)
{
    int id;
    int i, left_chopstick, right_chopstick, f;

    id = (int)num;
    printf ("Philosopher %d is done thinking and now ready to eat.\n", id);
    right_chopstick = id;
    left_chopstick = id + 1;

    /* Wrap around the chopsticks. */
    if (left_chopstick == PHILOS)
        left_chopstick = 0;

    while (f = food_on_table ()) {

        /* Thanks to philosophers #1 who would like to take a nap
           * before picking up the chopsticks, the other philosophers
           * may be able to eat their dishes and not deadlock.
             */
        //if (id == 1)
          //  sleep (sleep_seconds);

        if (ph_num_aleatorio == id)
            grab_chopstick (id, left_chopstick, "left");
        else
            grab_chopstick (id, right_chopstick, "right ");

        down_chopsticks(id, right_chopstick, left_chopstick, "direita");
        grab_chopstick (id, right_chopstick, "right");

        if (ph_num_aleatorio != id) {
            down_chopsticks(id, right_chopstick, left_chopstick, "direita");
            grab_chopstick(id, left_chopstick, "left");
            //
        } else {
            printf("Philosopher %d: eating.\n", id);
            down_chopsticks(id, right_chopstick, left_chopstick, "direita");
            down_chopsticks(id, left_chopstick, left_chopstick, "esquerda");
            ph_num_aleatorio = rand() % 5;
        }

        grab_chopstick(id, right_chopstick, "right");
        printf ("Philosopher %d: eating.\n", id);
        down_chopsticks(id, right_chopstick, left_chopstick, "direita");
        down_chopsticks(id, left_chopstick, left_chopstick, "esquerda");

        //ph_num_aleatorio = rand() % 5;
        usleep (DELAY * (FOOD - f + 1));
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
grab_chopstick (int phil,
                int c,
                char *hand)
{
    pthread_mutex_lock (&chopstick[c]);
    printf ("Philosopher %d: got %s chopstick %d\n", phil, hand, c);
}

void
down_chopsticks (int phil, int c1, int c2, char *hand)

{
    //if (phil != ph_num_aleatorio) {
        printf("Filosfo %d devolve o garfo %d da %s\n", phil, c1, hand);
        pthread_mutex_unlock(&chopstick[c1]);
    //}
    //pthread_mutex_unlock (&chopstick[c2]);

}