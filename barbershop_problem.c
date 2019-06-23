#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

// Maksimalni broj niti za musterije
#define MAX_MUSTERIJA 25

// Deklaracija funkcija
void *musterija(void *br);
void *berberin(void *);

void nasumicno_cekanje(int sekunde);

Definisanje semafora

// cekaonica Llimitira broj musterija koji može sesti na stolice
sem_t cekaonica;   

// stolica je semafor za mušteriju koja sedi kod berberina
sem_t stolica;

// spava_berberin je semafor koji dozvoljava berberinu da spava kod ne naiđe nova musterija
sem_t spava_berberin;

// cekanje je semafor koji musterije primorava da čekaju dok se ne završi šišanje prethodne musterije
sem_t cekanje;

// Oznaka koja omogućava berberinu da zaustavi rad, kada nema više mušterija
int zavrseno = 0;

// glavni program
int main() {
    pthread_t btid;
    pthread_t tid[MAX_MUSTERIJA];
    int i, br_musterija, br_stolica;
    int Niz_musterija[MAX_MUSTERIJA];
	
	printf("Broj mušterija: ");
	scanf("%d", &br_musterija);
	
	printf("Broj stolica u čekaonici: ");
	scanf("%d", &br_stolica);

    // Provera da li je maksimalni broj mušterija veći od unetog broja mušterija
    if (br_musterija > MAX_MUSTERIJA) {
    printf("Maksimalni broj mušterija je %d. Unesite sledeći put manji broj od maksimalnog.\n", MAX_MUSTERIJA);
    exit(-1); // Završetak programa zbog greške
    }

    //Oznaka za početak programa
    printf("\nbarbershop_problem.c\n");
    printf("Rešenje \"Barbershop problem\"-a:\n\n");

    // Inicijalizacija nasumičnog generatora brojeva
    srand48(Int(time(nil)));

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

// Funkcija ze generisanje random dužine čekanja na berberina koji spava
void nasumicno_cekanje(int sekunde) {
    int duzina;

    // Generisanje nasumičnog broja
    duzina = (int) ((drand48() * sekunde) + 1);
    sleep(duzina);
}