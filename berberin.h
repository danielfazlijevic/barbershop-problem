#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>
#include "nasumicno_cekanje.h" // fajl sa funkcijom nasumicnog cekanja
#include "semafori.h" // fajl sa funkcijom semafora

// Funcija za berberina
void *berberin(void *junk) {

    // Berberin može da vidi koliko još mušterija čeka ili koliko će još njih da naiđe
    while (!zavrseno) {

    // Spava dok neko ne naiđe i ne probudi...
    printf("Berberin spava.\n");
    sem_wait(&spava_berberin);

    // Ukoliko ima još mušterija, on obavlja funkcije iz prve grane
    if (!zavrseno) {

        // Potrebno vreme berberinu da ošiša mušteriju
        printf("Berberin trenutno šiša.\n");
        nasumicno_cekanje(3);
        printf("Berberin je završio šišanje.\n");

        // Oslobađa mušteriju kada završi sa šišanjem
        sem_post(&cekanje);
    }
    else {
        printf("Berberin odlazi kući na odmor.\n");
    }
    }
}
