/*
Meno: Matus Brandys
Datum: 4.12.2016

Simulujte nasledujucu situaciu.
Vo vyvhodnej Europe, v malej dedinke prepukla epidemia a nakazilo sa 10 ludi. Sedem z nich maju len mensiu nemoc na traja ludia maju vazne problemi. Aby sa nakazilo este viac ludi, tak sa vlada rozhodla tychto 10 ludi prestahovat do vedlasej dedinky, kde uz nikto dlhsie nebyval. Tito ludia musia kazdy den chodit na zdravotne stredisko v pravidelnych intervaloch (lahko chori v simulacii 3s a takzko chori 2.5s), kde ordinuje jeden doktor. Osetrenie jedneho pacienta trva urcity cas (lahko choreho v simulacii 0.5s a tazko choreho 1) a potom moze ist domov. Cela simulacia trva 20s.  

1. Doplnte do programu pocitadlo pocitajuce, kolko krat boli vysetreni pacienti s velkymi ocherenim a tiez pocitadlo ratajucem kolko krat boli vysetreni pacienti len sa malou nemocou.

2. Zabezpecte, aby do miestnosti k lekarovi mohli vojst maximalne dvaja lahko postihnuti pacienti, a ak v cakarni caka pacient z vaznym postihnutim, tak lekar dovysetruje pritimnich pacientov v miestnosti a pacient z vaznym postinutim ma prednst pred ostatnymi. Pocet pacientov v miestnosti s vaznym ochorenim mozu byt aj 3 ale vysetruje sa iba jeden.

3. Osetrite v programe spravne ukoncenie simulacie po uplynuti stanoveneho casu, tak ze ti pacienti co su este v cakarni, tyc doktor dovysetruje. 

Poznamky:
- na synchronizaciu je mozne pouzit mutexy, podmienene premenne alebo semafory
- nespoliehajte sa na uvedene casy, simulacia by mala fungovat aj s inymi casmi
- build (console): gcc epidemia.c -o epidemia -lpthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// signal na zastavenie simulacie
pthread_mutex_t mutex_stoj = PTHREAD_MUTEX_INITIALIZER;
int stoj = 0;

//pthread_cond_t chram_z = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_tazko_chori = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_lahko_chori = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_vysetrenie = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_t;
pthread_cond_t cond_l;
int count_tazko_chori = 0;
int count_lahko_chori = 0;
int count_vsetci = 0;
int pritomni_t = 0;
int pritomni_l = 0;

// lahko chori ide domov
void volno_lahko_chory(int i) {
    printf("lahko chory pacient %d je doma\n", i);
    sleep(3);
}

void vysetrenie_lahko_choreho(int i) {

    //lahko chori moznu do miestnosti vstupit aj dvaja ak tam nie je tazko chori
    pthread_mutex_lock(&mutex_vysetrenie);
    printf("%d lahko chory sa dostavil pred miestnost\n", i);
    while(pritomni_t != 0 || pritomni_l >= 2)
        pthread_cond_wait(&cond_l, &mutex_vysetrenie);
    pritomni_l++;
    printf("%d lahko chory bude vstupovat do miestnostnosti\n", i);
    pthread_mutex_unlock(&mutex_vysetrenie);

    //kriticka oblast == miestnost == lahko chori vysetrovat iba po 1 
    pthread_mutex_lock(&mutex_lahko_chori);
    printf("vysetrenie %d lahko choreho\n", i);
    sleep(0.5);           //samotne vysetrenie v miestnosti u dkotora
    count_lahko_chori++;
    count_vsetci++;
    pthread_mutex_unlock(&mutex_lahko_chori);    

    pthread_mutex_lock(&mutex_vysetrenie);
    pritomni_l--;       //opusta miestnost dokora
    printf("lahko chory %d opustuje miestnostnost\n", i);
    pthread_cond_broadcast(&cond_t);
    pthread_cond_broadcast(&cond_l);  //potrebne na ukoncenie na konci !!!!!
    pthread_mutex_unlock(&mutex_vysetrenie);
}

void *lahka_choroba( void *ptr ) {

    int i = (int*)ptr;
    // pokial nie je zastaveny
    pthread_mutex_lock(&mutex_stoj);
    while(!stoj) {
        pthread_mutex_unlock(&mutex_stoj);

        vysetrenie_lahko_choreho(i);
        volno_lahko_chory(i);

        pthread_mutex_lock(&mutex_stoj);
    }
    pthread_mutex_unlock(&mutex_stoj);
    return NULL;
}

// tazko chori ide domov
void volno_tazko_chory(int i) {
    printf("tazko chory pacient %d je doma\n", i);
    sleep(2.5);
}

void vysetrenie_tazko_choreho(int i) {

    //tazko chori vstupuje do miestnosti iba jeden a ma prednost pred lahko chorimi, ale musi pockat kym sa lahko chori dovysetruje
    pthread_mutex_lock(&mutex_vysetrenie);
    pritomni_t++;
    printf("%d tazko chory sa dostavil pred miestnost\n", i);
    while(pritomni_l != 0) 
        pthread_cond_wait(&cond_t, &mutex_vysetrenie);
    printf("%d tazko chory bude vstupovat do miestnostnosti\n", i);
    pthread_mutex_unlock(&mutex_vysetrenie);

    //kriticka oblast == miestnost == tazko chori vysetrovat iba po 1 
    pthread_mutex_lock(&mutex_tazko_chori);
    printf("vysetrenie %d tazko choreho\n", i);
    sleep(1);           //samotne vysetrenie v miestnosti u dkotora
    count_tazko_chori++;
    count_vsetci++;
    pthread_mutex_unlock(&mutex_tazko_chori);

    pthread_mutex_lock(&mutex_vysetrenie);
    pritomni_t--;       //opusta miestnost dokora
    printf("tazko chory %d opustuje miestnostnost\n", i);
    pthread_cond_broadcast(&cond_l);
    pthread_mutex_unlock(&mutex_vysetrenie);
}

void *vazna_choroba( void *ptr ) {

    int i = (int*)ptr;

    // pokial nie je zastaveny
    pthread_mutex_lock(&mutex_stoj);
    while(!stoj) {
        pthread_mutex_unlock(&mutex_stoj);

        vysetrenie_tazko_choreho(i);

        volno_tazko_chory(i);

        pthread_mutex_lock(&mutex_stoj);
    }
    pthread_mutex_unlock(&mutex_stoj);
    return NULL;
}

int main(void) {
    int i;

    pthread_t vazne_chori[3];
    pthread_t lahko_chori[7];

    for (i=0;i<3;i++) pthread_create( &vazne_chori[i], NULL, &vazna_choroba,(void*)i);
    for (i=0;i<7;i++) pthread_create( &lahko_chori[i], NULL, &lahka_choroba, (void*)i);

    sleep(20);
    pthread_mutex_lock(&mutex_stoj);
    stoj = 1;
    pthread_cond_broadcast(&cond_l);
    pthread_cond_broadcast(&cond_t);
    pthread_mutex_unlock(&mutex_stoj);


    for (i=0;i<3;i++) pthread_join( vazne_chori[i], NULL);
    for (i=0;i<7;i++) pthread_join( lahko_chori[i], NULL);

    printf("pocet vysetreni tazko chorych: %d, lahko_chorich: %d, vsetkych: %d\n ", count_tazko_chori, count_lahko_chori, count_vsetci);
    exit(EXIT_SUCCESS);
}
