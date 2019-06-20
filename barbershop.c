#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

// Maksimalni broj niti za musterije
#define MAX_CUSTOMERS 25

// Deklaracija funkcija
void *customer(void *num);
void *barber(void *);

void randwait(int secs);

// Definisanje semafora

// waitingRoom Llimitira broj musterija koji moze sesti na stolice
sem_t waitingRoom;   

// barberChair je semafor za musteriju koja sedi kod frizera
sem_t barberChair;

// barberPillow je semafor koji dozvoljava frizeru da spava kod ne naidje nova musterija
sem_t barberPillow;

// seatBelt ije semafor koji musterije primorava da cekaju dok se ne zavrsi sisanje prethodne musterije
sem_t seatBelt;

// FOznaka koja omogucava frizeru da zaustavi rad, kada nema vise musterija
int allDone = 0;

int main() {
    pthread_t btid;
    pthread_t tid[MAX_CUSTOMERS];
    long RandSeed;
    int i, numCustomers, numChairs;
    int Number[MAX_CUSTOMERS];
	
	printf("Unesite broj musterija: ");
	scanf("%d", &numCustomers);
	
	printf("Unesite broj stolica u cekaonici: ");
	scanf("%d", &numChairs);


    // Get the command line arguments and convert them
    // into integers.
    RandSeed = 1212;

    // Make sure the number of threads is less than the number of
    // customers we can support.
    if (numCustomers > MAX_CUSTOMERS) {
    printf("The maximum number of Customers is %d.\n", MAX_CUSTOMERS);
    exit(-1);
    }

    printf("\nSleepBarber.c\n\n");
    printf("A solution to the sleeping barber problem using semaphores.\n");

    // Initialize the random number generator with a new seed.
    srand48(RandSeed);

    // Initialize the numbers array.
    for (i=0; i<MAX_CUSTOMERS; i++) {
    Number[i] = i;
    }

    // Initialize the semaphores with initial values...
    sem_init(&waitingRoom, 0, numChairs);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);

    // Create the barber.
    pthread_create(&btid, NULL, barber, NULL);

    // Create the customers.
    for (i=0; i<numCustomers; i++) {
    pthread_create(&tid[i], NULL, customer, (void *)&Number[i]);
    }

    // Join each of the threads to wait for them to finish.
    for (i=0; i<numCustomers; i++) {
    pthread_join(tid[i],NULL);
    }

    // When all of the customers are finished, kill the
    // barber thread.
    allDone = 1;
    sem_post(&barberPillow);  // Wake the barber so he will exit.
    pthread_join(btid,NULL);    
}

void *customer(void *number) {
    int num = *(int *)number;

    // Leave for the shop and take some random amount of
    // time to arrive.
    printf("Customer %d leaving for barber shop.\n", num);
    randwait(5);
    printf("Customer %d arrived at barber shop.\n", num);

    // Wait for space to open up in the waiting room...
    sem_wait(&waitingRoom);
    printf("Customer %d entering waiting room.\n", num);

    // Wait for the barber chair to become free.
    sem_wait(&barberChair);

    // The chair is free so give up your spot in the
    // waiting room.
    sem_post(&waitingRoom);

    // Wake up the barber...
    printf("Customer %d waking the barber.\n", num);
    sem_post(&barberPillow);

    // Wait for the barber to finish cutting your hair.
    sem_wait(&seatBelt);

    // Give up the chair.
    sem_post(&barberChair);
    printf("Customer %d leaving barber shop.\n", num);
}

void *barber(void *junk) {
    // While there are still customers to be serviced...
    // Our barber is omnicient and can tell if there are 
    // customers still on the way to his shop.
    while (!allDone) {

    // Sleep until someone arrives and wakes you..
    printf("The barber is sleeping\n");
    sem_wait(&barberPillow);

    // Skip this stuff at the end...
    if (!allDone) {

        // Take a random amount of time to cut the
        // customer's hair.
        printf("The barber is cutting hair\n");
        randwait(3);
        printf("The barber has finished cutting hair.\n");

        // Release the customer when done cutting...
        sem_post(&seatBelt);
    }
    else {
        printf("The barber is going home for the day.\n");
    }
    }
}

void randwait(int secs) {
    int len;

    // Generate a random number...
    len = (int) ((drand48() * secs) + 1);
    sleep(len);
}
