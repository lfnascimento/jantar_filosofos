#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define NUMERO_FILOSOFOS 5
//Nao eh necessario essa variavel estatica ATRASO
#define ATRASO 2
//Variavel estatica que limita o loop(comida na mesa) dos filosfos
#define COMIDA 25

//Declaracao das funcoes
void *filosofo (void *id);
void pegaGarfo (int, int, char *);
void devolveGarfo (int, int, char *);

void pensar(int);
void comer(int);

int comidaNaMesa ();

int numero_filosofos;

pthread_mutex_t *garfo;
pthread_t *filo;

pthread_mutex_t comidaBloqueada;

int sleep_seconds = 0;

int tempoComendo;
int tempoPensando;

int filosofoAleatorio;

int main (int argn, char **argv)
{
    int i;

    printf("Informe o numero de filosofos:\n");
    scanf("%d", &numero_filosofos);

    //Escolhe um filosofo aleatorio
    filosofoAleatorio = rand() % numero_filosofos;

    garfo = (pthread_mutex_t *) malloc(numero_filosofos*sizeof(pthread_mutex_t));
    filo = (pthread_t *) malloc(numero_filosofos*sizeof(pthread_t));

    printf("Informe o tempo gasto do filosofo pensando (ms):\n");
    scanf("%d", &tempoPensando);

    printf("Informe o tempo gasto do filosofo comendo (ms):\n");
    scanf("%d", &tempoComendo);

    printf("---------------------------------------------:\n");

    if (argn == 2)
        sleep_seconds = atoi (argv[1]);

    pthread_mutex_init (&comidaBloqueada, NULL);
    for (i = 0; i < numero_filosofos; i++)
        pthread_mutex_init (&garfo[i], NULL);
    for (i = 0; i < numero_filosofos; i++)
        pthread_create (&filo[i], NULL, filosofo, (void *)i);
    for (i = 0; i < numero_filosofos; i++)
        pthread_join (filo[i], NULL);
    return 0;
}
//Funcao que simula o filosofo
void *filosofo (void *num)
{
    int id;
    int i, garfoEsquerdo, garfoDireito, f;

    id = (int)num;

    garfoDireito = id;
    garfoEsquerdo = id + 1;

    pensar(id);

    if (garfoEsquerdo == numero_filosofos)
        garfoEsquerdo = 0;

    //Enquanto tiver comida na mesa
    while (f = comidaNaMesa()) {

        //Um filosofo aleatorio pega o garfo da esquerda
        if (filosofoAleatorio == id)
            pegaGarfo (id, garfoEsquerdo, "esquerda");
        else
            pegaGarfo (id, garfoDireito, "direita ");

        //Todos devolvem o garfo da direita
        devolveGarfo(id, garfoDireito,  "direita");

        //Todos pegam o garfo direita, dando a possibilidade do filosofo aleatorio pegar os 2 garfos,
        // ja que ele ja tem o esquerdo
        pegaGarfo (id, garfoDireito, "direita");


        //Se for um filosofo comum(nao eh aleatorio) pega o garfo da esquerda, se for o aleatorio,
        // entao ele come e devolve o garfos, em seguida PENSA
        if (filosofoAleatorio != id) {
            devolveGarfo(id, garfoDireito,  "direita");
            pegaGarfo(id, garfoEsquerdo, "esquerda");

        } else {
            comer(id);

            devolveGarfo(id, garfoDireito,  "direita");
            devolveGarfo(id, garfoEsquerdo,  "esquerda");
            //Escolha de um filosofo aleatorio para pegar o garfo no sentido oposto
            filosofoAleatorio = rand() % numero_filosofos;

            pensar(id);

        }
        //O filosofo  comum pega o garfo da direita
        pegaGarfo(id, garfoDireito, "direita");

        comer(id);
        //O filosofo comum devolve os garfos
        devolveGarfo(id, garfoDireito,  "direita");
        devolveGarfo(id, garfoEsquerdo,  "esquerda");

        pensar(id);

    }

    printf ("Filosofo %d acabou de comer.\n", id);
    return (NULL);
}
//Funcao que limita o loop dos filosofos
int
comidaNaMesa ()
{
    static int comida = COMIDA;
    int meuPrato;

    pthread_mutex_lock (&comidaBloqueada);
    if (comida > 0) {
        comida--;
    }
    meuPrato = comida;
    pthread_mutex_unlock (&comidaBloqueada);
    return comida;
}

//Funcao que o filosofo pega o garfo
void
pegaGarfo (int filo,
                int g,
                char *lado)
{
    pthread_mutex_lock (&garfo[g]);
    printf ("Filosfo %d FAMINTO: pega  o garfo %d da %s\n", filo, g, lado);
}
//Funcao em que o filosofo devolve o garfo
void
devolveGarfo (int filo, int g, char *lado)

{
    pthread_mutex_unlock(&garfo[g]);
    printf("Filosfo %d devolve o garfo %d da %s\n", filo, g, lado);
}
//Funcao que o filosofo fica pensando
void pensar(int id) {
    printf("Filosofo %d esta PENSANDO\n",id);
    usleep(tempoPensando*1000); //Gasta um tempo em microsegundos
}
//Funcao que o filosofo fica comendo
void comer(int id) {
    printf ("Filosfo %d: COMENDO.\n", id);
    usleep(tempoComendo*1000);//Gasta um tempo em microsegundos
}