#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct {
  double theta; 
  double omega; 
} Punto;

typedef struct {
  double dt;
  int nsteps;
  int stride;
  double q;
  double f;
  double Omega;
} Sistema;

typedef Punto (*Accelerazione)(double t, Punto s, Sistema *sist);
typedef void (*Integratore)(double t, Punto *s, Sistema *sist, Accelerazione f);

Punto derivata(double t, Punto s, Sistema *sist) {
  Punto d;
  d.theta = s.omega;
  d.omega = -sin(s.theta) - sist->q * s.omega + sist->f * cos(sist->Omega * t);
  return d;
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

int main(void) {
  int ND = 100;
  int n_trans = 150;   // periodi di transiente da scartare
  int n_misure = 100;  // periodi da registrare dopo il transiente
  int n_tot_periodi = n_trans + n_misure;

  double theta0 = 0.2;
  double omega0 = 0.0;
  double q = 0.5;
  double Omega = 2.0 / 3.0;

  double f_min = 1.0;
  double f_max = 1.5;
  double df = 0.005;

  Sistema sist;
  sist.q = q;
  sist.Omega = Omega;
  sist.stride = ND;
  sist.nsteps = n_tot_periodi * ND;
  sist.dt = (2.0 * PI) / (sist.Omega * ND);

  Integratore passo = RK4;

  int n_f = (int)((f_max - f_min) / df + 0.5);

  for (int i = 0; i <= n_f; i++) {
    sist.f = f_min + i * df;

    Punto s;
    s.theta = theta0;
    s.omega = omega0;

    for (int j = 0; j < sist.nsteps; j++) {
      double t = j * sist.dt;
      passo(t, &s, &sist, derivata);

      int step_fatto = j + 1;
      if (step_fatto % sist.stride == 0) {
        int periodo_corrente = step_fatto / sist.stride;
        if (periodo_corrente > n_trans) {
          printf("%.16lf %.16lf\n", sist.f, s.omega);
        }
      }
    }
  }

  return 0;
}
