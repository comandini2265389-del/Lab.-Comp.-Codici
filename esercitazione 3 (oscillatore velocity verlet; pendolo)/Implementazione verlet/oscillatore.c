#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define m 1

typedef struct
{
  double v0;
  double x0;
} Punto;

typedef struct
{
  double dt;
  double omega0;
  int nsteps;
  double T;
} Sistema;

void (*integrazione)(Sistema *sist, Punto *p);

void v_verlet(Sistema *sist, Punto *p);
void euler(Sistema *sist, Punto *p);
void euler_cromer(Sistema *sist, Punto *p);
double Energia(Sistema *sist, Punto *p);

int main(int argc, char *argv[])
{
  Punto p;
  Sistema sist;
  if(argc!=8)
    {
      fprintf(stderr,"inserire: metodo (1/2/3) dt nsteps stride x0 v0 omega0\n");
      exit(1);
    }

  int scelta=atoi(argv[1]);
  sist.dt=atof(argv[2]);
  sist.nsteps=atoi(argv[3]);
  int stride=atoi(argv[4]);
  p.x0=atof(argv[5]);
  p.v0=atof(argv[6]);
  sist.omega0=atof(argv[7]);
  if(scelta==1)
  {
    integrazione=euler;
  }
  else if(scelta==2)
  {
    integrazione=euler_cromer;
  }
  else if(scelta==3)
  {
    integrazione=v_verlet;
  }
  else
  {
    fprintf(stderr, "inserire un metodo di integrazione valido:\n euler=1; euler-cromer=2; v_verlet=3\n");
    exit(1);
  }
  for(int i=0;i<sist.nsteps;i++)
  {
    sist.T+=sist.dt;
    integrazione(&sist, &p);
  }
}

double Energia(Sistema *sist, Punto *p)
{
return 0.5 * p->v0 * p->v0 + 0.5 * sist->omega0 * sist->omega0 * p->x0 * p->x0;
}

void euler(Sistema *sist, Punto *p)
{
  double x_prec=p->x0;
  p->x0 += p->v0 * sist->dt;
  p->v0 += -sist->omega0 * sist->omega0 * x_prec * sist->dt;
  double E = Energia(sist, p);
  printf("%lf %lf %lf %lf\n", sist->T, p->x0, p->v0, E); 
}

void euler_cromer(Sistema *sist, Punto *p)
{
  p->v0 += -sist->omega0 * sist->omega0 * p->x0 * sist->dt;
  p->x0 += p->v0 * sist->dt;
  double E = Energia(sist, p);
  printf("%lf %lf %lf %lf\n", sist->T, p->x0, p->v0, E);
}

void v_verlet(Sistema *sist, Punto *p)
{
  double acc=0;
  p->v0 += 0.5 * -(sist->omega0 * sist->omega0) * sist->dt;
  p->x0 += p->v0 * sist->dt;
  //ipotizzando che la massa valga 1 la forza corrisponde (numericamente) all'accelerazione
  acc = (-(sist->omega0 * sist->omega0) * p-> x0)/m;
  p->v0 += 0.5 * acc * sist->dt;
  double E = Energia(sist, p);
  printf("%lf %lf %lf %lf\n", sist->T, p->x0, p->v0, E);
}
