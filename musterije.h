#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>
#include "nasumicno_cekanje.h" // fajl sa funkcijom nasumicnog cekanja
#include "semafori.h" // fajl sa funkcijom semafora

// Funkcija mušterije
void *musterija(void *broj) {

    int br = *(int *)broj;

    // Odlazi iz salona, kako bi se ponovo vratila kada bude bilo dovoljno mesta
    printf("Mušterija %d odlazi na neko vreme iz salona.\n", br);
    nasumicno_cekanje(5);
    printf("Mušterija %d dolazi u salon.\n", br);

    // Čekanje dok se ne osobodi mesto u čekaonici
    sem_wait(&cekaonica);
    printf("Mušterija %d ulazi u čekaonicu.\n", br);

    // Čekanje dok stolica za šišanje ne bude slobodna
    sem_wait(&stolica);

    // Osobođena je stolica za šišanje. Mušterija oslobađa mesto u čekaonici
    sem_post(&cekaonica);

    // Buđenje berberina ukoliko spava
    printf("Mušterija %d budi berberina.\n", br);
    sem_post(&spava_berberin);

    // Čekanje da berberin završi njegovo šišanje 
    sem_wait(&cekanje);

    // Ustajanje sa stolice
    sem_post(&stolica);
    printf("Mušterija %d odlazi iz salona.\n", br);
}