/*
 * Esercitazione 7 - Random walk unidimensionale.
 *
 * nwalks camminatori indipendenti partono tutti da x = 0 e avanzano INSIEME:
 * a ogni passo ciascun camminatore si sposta di +dx o di -dx con
 * probabilita' 1/2. Ogni stride passi si calcolano le medie sull'insieme.
 *
 * Dovete:
 *   1. scrivere la funzione passo;
 *   2. nel main, calcolare e stampare le medie sull'insieme dei camminatori,
 *      come avete fatto in langevin.c.
 * Cercate i TODO. Questo programma e' la base dell'esercitazione 8.
 *
 * Uso:          ./random_walk nsteps stride nwalks seed
 * Output:       t mean_x msd  (condizione iniziale e poi una riga ogni stride passi)
 *               con nwalks = 1 la colonna mean_x e' la posizione dell'unico camminatore
 * Compilazione: gcc -Wall -O2 random_walk.c -o random_walk -lm
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Lo stato di un camminatore: solo la posizione. */
typedef struct {
    double x;
} Punto;

/* Il passo reticolare dx e l'intervallo di tempo dt tra due passi.
 * Nel main valgono entrambi 1: il tempo coincide con il numero di passi. */
typedef struct {
    double dx;
    double dt;
} Sistema;

/* Un passo del random walk per il camminatore *p.
 * Usate UN SOLO numero casuale per passo: se drand48() < 0.5 il camminatore
 * si sposta a destra (+dx), altrimenti a sinistra (-dx). */
void passo(Punto *p, Sistema *sist) {
    // TODO
  double d=drand48();
  if(d < 0.5)
    {
      p->x += sist->dx;
    }
  else
    {
      p->x -= sist->dx;
    }
}

void medie(Punto *p, Sistema *sist, int nwalks, int n)
{
  double x=0;
  double xq=0;
  for(int i=0; i<nwalks; i++)
    {
      x += p[i].x;
      xq += p[i].x * p[i].x;
    }
  x = x/nwalks;
  xq = xq/nwalks;

  printf("%lf %.10e %.10e\n", n*sist->dt, x, xq);
}

int main(int argc, char *argv[]) {
    if(argc != 5) {
        fprintf(stderr, "Uso: %s nsteps stride nwalks seed\n", argv[0]);
        return 1;
    }
    int nsteps = atoi(argv[1]);
    int stride = atoi(argv[2]);
    int nwalks = atoi(argv[3]);
    int seed = atoi(argv[4]);
    if(nsteps < 0 || stride <= 0 || nwalks <= 0) {
        fprintf(stderr, "Errore: nsteps non negativo, stride e nwalks positivi\n");
        return 1;
    }

    Sistema sist = {1.0, 1.0}; /* dx = 1, dt = 1 */

    /* un elemento per ogni camminatore (allocazione dinamica della memoria),
     * tutti nell'origine */
    Punto *p = malloc(nwalks * sizeof(Punto));
    if(p == NULL) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        return 1;
    }
    for(int k = 0; k < nwalks; k++) {
        p[k].x = 0.0;
    }

    srand48(seed);
    for(int n = 0; n <= nsteps; n++) {
        /* TODO: se n e' un multiplo di stride (compreso n = 0, cioe' la
         * condizione iniziale), calcolate le medie <x> e <x^2> sui nwalks
         * camminatori e stampate una riga con tre colonne, t <x> <x^2>, con
         * t = n dt e il formato %.10e. E' lo stesso calcolo che avete scritto
         * in langevin.c: potete copiarlo. */

      if(n % stride == 0)
	{
	  medie(p,&sist,nwalks,n);
	}

        if(n < nsteps) {
            /* tutti i camminatori fanno un passo */
            for(int k = 0; k < nwalks; k++) {
                passo(&p[k], &sist);
            }
        }
    }
    free(p);
    return 0;
}
