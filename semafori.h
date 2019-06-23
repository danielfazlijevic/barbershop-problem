#include <semaphore.h>

// cekaonica Llimitira broj musterija koji može sesti na stolice
sem_t cekaonica;   

// stolica je semafor za mušteriju koja sedi kod berberina
sem_t stolica;

// spava_berberin je semafor koji dozvoljava berberinu da spava kod ne naiđe nova musterija
sem_t spava_berberin;

// cekanje je semafor koji musterije primorava da čekaju dok se ne završi šišanje prethodne musterije
sem_t cekanje;