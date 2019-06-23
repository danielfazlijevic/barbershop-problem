#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

// Funkcija ze generisanje random dužine čekanja na berberina koji spava
void nasumicno_cekanje(int sekunde) {
    int duzina;

    // Generisanje nasumičnog broja
    duzina = (int) ((drand48() * sekunde) + 1);
    sleep(duzina);
}