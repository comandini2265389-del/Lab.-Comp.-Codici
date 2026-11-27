/*
 * Esercitazione 7 - Equazione di Langevin per una particella libera in 1D,
 * con massa m = 1, integrata con il metodo di Eulero:
 *
 *     v_{n+1} = v_n - gamma v_n dt + sqrt(2 gamma kT dt) R_n
 *     x_{n+1} = x_n + v_n dt
 *
 * dove le R_n sono numeri gaussiani indipendenti con media 0 e varianza 1.
 * nwalks traiettorie indipendenti partono tutte da x = 0 con velocita' v0 e
 * avanzano INSIEME: a ogni passo si aggiornano tutte le particelle.
 *
 * La parte deterministica (attrito) e' gia' scritta. Dovete:
 *   1. scrivere la funzione gaussian;
 *   2. aggiungere il rumore nella funzione passo;
 *   3. nel main, calcolare e stampare le medie sull'insieme delle particelle.
 * Cercate i TODO.
 *
 * Uso:          ./langevin dt nsteps stride nwalks v0 gamma kT seed
 * Output:       t mean_x msd  (condizione iniziale e poi una riga ogni stride passi)
 * Compilazione: gcc -Wall -O2 langevin.c -o langevin -lm
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Lo stato di una particella: posizione e velocita'. */
typedef struct {
    double x;
    double v;
} Punto;

typedef struct {
    double dt;
    double gamma; /* coefficiente di attrito: con m = 1, tau_v = 1 / gamma */
    double kT;    /* k_B T */
} Sistema;

/* Restituisce un numero gaussiano con media 0 e varianza 1, generato con la
 * trasformazione di Box-Muller. TODO: utilizzate la versione vista a lezione, 
 * oppure quella che trovate sulle note del corso o sul libro */
double gaussian(void)
{
  /* Faccio come dicono gli appunti, produco solamente un numero casuale
     per integrazione*/

  double u1=drand48();
  double u2=drand48();

  return sqrt(-2*log(1-u1))*cos(2*M_PI*u2);
}

/* Un passo di Eulero per la particella *p. */
void passo(Punto *p, Sistema *sist) {
    double dt = sist->dt;

    /* TODO: il termine di rumore sqrt(2 gamma kT dt) R, con R estratto
     * chiamando gaussian() UNA SOLA VOLTA. Finche' vale 0 la particella
     * sente solo l'attrito. */
    double R=gaussian();
    double rumore = sqrt(2*sist->gamma*sist->kT*dt)*R;

    /* la posizione si aggiorna con la velocita' all'inizio del passo */
    double v_vecchia = p->v;
    p->v += -sist->gamma * p->v * dt + rumore;
    p->x += v_vecchia * dt;
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
    if(argc != 9) {
        fprintf(stderr, "Uso: %s dt nsteps stride nwalks v0 gamma kT seed\n", argv[0]);
        return 1;
    }
    Sistema sist;
    sist.dt = atof(argv[1]);
    int nsteps = atoi(argv[2]);
    int stride = atoi(argv[3]);
    int nwalks = atoi(argv[4]);
    double v0 = atof(argv[5]);
    sist.gamma = atof(argv[6]);
    sist.kT = atof(argv[7]);
    int seed = atoi(argv[8]);
    if(sist.dt <= 0.0 || nsteps < 0 || stride <= 0 || nwalks <= 0 || sist.gamma <= 0.0 || sist.kT < 0.0) {
        fprintf(stderr, "Errore: dt, stride, nwalks e gamma positivi, nsteps e kT non negativi\n");
        return 1;
    }

    /* un elemento per ogni particella, tutte in x = 0 con velocita' v0 */
    Punto *p = malloc(nwalks * sizeof(Punto));
    if(p == NULL) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        return 1;
    }
    for(int k = 0; k < nwalks; k++) {
        p[k].x = 0.0;
        p[k].v = v0;
    }

    srand48(seed);
    for(int n = 0; n <= nsteps; n++) {
        /* TODO: se n e' un multiplo di stride (compreso n = 0, cioe' la
         * condizione iniziale), calcolate le medie <x> e <x^2> sulle nwalks
         * particelle e stampate una riga con tre colonne, t <x> <x^2>, con
         * t = n dt e il formato %.10e. Conviene scrivere una funzione a parte. */
      if(n % stride == 0)
	{
	  medie(p,&sist,nwalks,n);
	}

        if(n < nsteps) {
            /* tutte le particelle fanno un passo */
            for(int k = 0; k < nwalks; k++) {
                passo(&p[k], &sist);
            }
        }
    }

    free(p);
    return 0;
}
