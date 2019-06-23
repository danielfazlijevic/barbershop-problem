#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

#include "musterije.h" // fajl sa funkcijom mušterije
#include "berberin.h" // fajl sa funkcijom berberina
#include "nasumicno_cekanje.h" // fajl sa funkcijom nasumicnog cekanja
#include "semafori.h" // fajl sa funkcijom semafora

// Maksimalni broj niti za musterije
#define MAX_MUSTERIJA 25









// // Deklaracija funkcija
// void *musterija(void *br);
// void *berberin(void *);

// void nasumicno_cekanje(int sekunde);

// Definisanje semafora

// // cekaonica Llimitira broj musterija koji može sesti na stolice
// sem_t cekaonica;   

// // stolica je semafor za mušteriju koja sedi kod berberina
// sem_t stolica;

// // spava_berberin je semafor koji dozvoljava berberinu da spava kod ne naiđe nova musterija
// sem_t spava_berberin;

// // cekanje je semafor koji musterije primorava da čekaju dok se ne završi šišanje prethodne musterije
// sem_t cekanje;








// Oznaka koja omogućava berberinu da zaustavi rad, kada nema više mušterija
int zavrseno = 0;

// glavni program
int main() {
    pthread_t btid;
    pthread_t tid[MAX_MUSTERIJA];
    long RandSeed;
    int i, br_musterija, br_stolica;
    int Niz_musterija[MAX_MUSTERIJA];
	
	printf("Broj mušterija: ");
	scanf("%d", &br_musterija);
	
	printf("Broj stolica u čekaonici: ");
	scanf("%d", &br_stolica);

    // Generisanje nasumične brojke, kojom se određuje kojim redom će se niti izvršavati na osnovu broja mušterija
    RandSeed = 1212;

    // Provera da li je maksimalni broj mušterija veći od unetog broja mušterija
    if (br_musterija > MAX_MUSTERIJA) {
    printf("Maksimalni broj mušterija je %d. Unesite sledeći put manji broj od maksimalnog.\n", MAX_MUSTERIJA);
    exit(-1); // Završetak programa zbog greške
    }

    //Oznaka za početak programa
    printf("\nbarbershop_problem.c\n");
    printf("Rešenje \"Barbershop problem\"-a:\n\n");

    // Inicijalizacija nasumičnog generatora brojeva
    srand48(RandSeed);

    // Inicijalizacija niza mušterija
    for (i = 0; i < MAX_MUSTERIJA; i++) {
    Niz_musterija[i] = i + 1;
    }

    // Inicijalizacija semafora sa početnim vrednostima
    sem_init(&cekaonica, 0, br_stolica);
    sem_init(&stolica, 0, 1);
    sem_init(&spava_berberin, 0, 0);
    sem_init(&cekanje, 0, 0);

    // Kreiranje niti za berberina
    pthread_create(&btid, NULL, berberin, NULL);

    // Kreiranje niti za mušteriju
    for (i = 0; i < br_musterija; i++) {
    pthread_create(&tid[i], NULL, musterija, (void *)&Niz_musterija[i]);
    }

    // Neka svaka nit čeka da neka druga nit završi sa procesom
    for (i = 0; i < br_musterija; i++) {
    pthread_join(tid[i],NULL);
    }

    // Kada sve mušterije budu uslužene, briše nit berberina
    zavrseno = 1;

    // Buđenje berberina, kako bi mogao da započne šišanje
    sem_post(&spava_berberin);  
    pthread_join(btid,NULL);    
}