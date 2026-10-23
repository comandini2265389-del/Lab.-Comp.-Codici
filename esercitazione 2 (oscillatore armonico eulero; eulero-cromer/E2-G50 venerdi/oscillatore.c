#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
} Sistema;

void euler(Sistema *sist, Punto *p,double T);
void euler_cromer(Sistema *sist, Punto *p, double T);

int main(int argc, char *argv[])
{
  Punto p;
  Sistema sist;
  if(argc!=8)
    {
      fprintf(stderr,"inserire: metodo dt nsteps stride v0 x0 omega0\n");
      exit(1);
    }
  p.x0=atof(argv[5]);
  p.v0=atof(argv[6]);
  sist.dt=atof(argv[2]);
  sist.omega0=atof(argv[7]);
  sist.nsteps=atoi(argv[3]);
  char *stringa=argv[1];
  char eulero[]="euler";
  char eulerocromer[]="euler-cromer";
  int risultato=strcmp(stringa, eulero);
  int stride=atoi(argv[4]);
  int i=0;
  double T=0;
  if(risultato==0)
    {
      for(i=0;i<sist.nsteps;i++)
	{
	  T+=sist.dt;
	  euler(&sist,&p,T);
	}
    }
  else if(risultato != 0)
    {
      risultato=strcmp(stringa, eulerocromer);
      if(risultato==0)
	{
	  for(i=0;i<sist.nsteps;i++)
	    {
	      T+=sist.dt;
		euler_cromer(&sist,&p,T);
	    }
	}
    }
  else if(risultato != 0)
    {
      fprintf(stderr,"inserire un metodo valido. \n");
      exit(1);
    }
}

void euler(Sistema *sist, Punto *p, double T)
{
  double x_prec=p->x0;
  double E=0;
  p->x0 += p->v0 * sist->dt;
  p->v0 += -sist->omega0 * sist->omega0 * x_prec * sist->dt;
  E = 0.5 * p->v0 * p->v0 + 0.5 * sist->omega0 * sist->omega0 * p->x0 * p->x0;
  printf("%lf %lf %lf %lf\n", T, p->x0, p->v0, E); 
}

void euler_cromer(Sistema *sist, Punto *p, double T)
{
  double E=0;
  p->v0 += -sist->omega0 * sist->omega0 * p->x0 * sist->dt;
  p->x0 += p->v0 * sist->dt;
  E = 0.5 * p->v0 * p->v0 + 0.5 * sist->omega0 * sist->omega0 * p->x0 * p->x0;
  printf("%lf %lf %lf %lf\n", T, p->x0, p->v0, E);
}
