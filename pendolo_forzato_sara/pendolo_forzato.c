#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846 //definizione di pi greco

typedef struct {
  double theta;
  double omega;
} Punto;

typedef struct {
  double dt; //passo temporale
  int nsteps; //numero tot di passi
  int stride; //intervallo di stampa
  double q; //i diversi parametri 
  double f;
  double Omega;
} Sistema;

typedef Punto (*Accelerazione)(double t, Punto s, Sistema *sist); //definisco il puntatore a funzione che restituisce un Punto (come scrive lui i codici vedi esonero)
typedef void (*Integratore)(double t, Punto *s, Sistema *sist, Accelerazione f);


//funzioni messe all'inizio sempre per come scrive lui il codice
Punto derivata(double t, Punto s, Sistema *sist) {
  Punto d; //variabile locale che contiene le derivate
  d.theta = s.omega;
  d.omega = -sin(s.theta) - sist->q * s.omega + sist->f * cos(sist->Omega * t);
  return d;
}

double energia(Punto s, Sistema *sist) {
  (void)sist; //evita warning se -Wall perchè non uso parametri da sistema nell'energia
  return 0.5 * s.omega * s.omega + 1.0 - cos(s.theta);
}

void RK4(double t, Punto *s, Sistema *sist, Accelerazione f) {
  double dt = sist->dt;

  Punto k1 = f(t, *s, sist);

  Punto p1;
  p1.theta = s->theta + 0.5 * dt * k1.theta;
  p1.omega = s->omega + 0.5 * dt * k1.omega;
  Punto k2 = f(t + 0.5 * dt, p1, sist);

  Punto p2;
  p2.theta = s->theta + 0.5 * dt * k2.theta;
  p2.omega = s->omega + 0.5 * dt * k2.omega;
  Punto k3 = f(t + 0.5 * dt, p2, sist);

  Punto p3;
  p3.theta = s->theta + dt * k3.theta;
  p3.omega = s->omega + dt * k3.omega;
  Punto k4 = f(t + dt, p3, sist);

  s->theta += (dt / 6.0) * (k1.theta + 2.0 * k2.theta + 2.0 * k3.theta + k4.theta);
  s->omega += (dt / 6.0) * (k1.omega + 2.0 * k2.omega + 2.0 * k3.omega + k4.omega);
}

void stampa_stato(double t, Punto s, Sistema *sist) {
  double E = energia(s, sist);
  double theta_r = atan2(sin(s.theta), cos(s.theta));
  printf("%.16lf %.16lf %.16lf %.16lf %.16lf\n", t, s.theta, s.omega, E, theta_r);
}

int main(int argc, char *argv[]) {
  if (argc != 8) {
    fprintf(stderr, "Uso:%s  ND nsteps stride theta0 omega0 q f Omega\n", argv[0]);
    return 1;
  }

  int ND = atoi(argv[1]); //numero di passi per periodo della forzante
  Sistema sist;
  sist.nsteps = atoi(argv[2]);
  sist.stride = atoi(argv[3]);

  Punto s; //condizioni iniziali
  s.theta = atof(argv[4]);
  s.omega = atof(argv[5]);

  sist.q = atof(argv[6]);
  sist.f = atof(argv[7]);
  sist.Omega = atof(argv[8]);

  if (ND <= 0 || sist.nsteps < 0 || sist.stride <= 0 || sist.Omega == 0.0) {
    fprintf(stderr, "Errore: parametri non validi.\n");
    return 1;
  }
 
  sist.dt = (2.0 * PI) / (sist.Omega * ND); // Calcolo delta tau

  Integratore passo = RK4;

  double t = 0.0; // Condizione iniziale a tau = 0
  stampa_stato(t, s, &sist);

  for (int i = 0; i < sist.nsteps; i++) {
    t = i * sist.dt; // tempo all'inizio del passo
    passo(t, &s, &sist, derivata);
    double t_dopo = (i + 1) * sist.dt;

    if ((i + 1) % sist.stride == 0) {
      stampa_stato(t_dopo, s, &sist);
    }
  }

  return 0;
}
