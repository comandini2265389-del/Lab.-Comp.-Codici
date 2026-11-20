#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M 1
#define G 9.81
#define PI 3.14159265358979323846

typedef struct
{
  double theta0;
  double omega0;
} Punto;

typedef struct
{
  int ND;
  int nsteps;
  int stride;
  double q;
  double f;
  double Omega;
  double dtau;
} Sistema;

typedef Punto (*Derivata)(double tau, Punto *p, Sistema *sist);
typedef void (*Integratore)(double tau, Punto *p, Sistema *sist, Derivata f);

Punto pendolo(double tau, Punto *p, Sistema *sist);
void rk4(double tau, Punto *p, Sistema *sist, Derivata f);
double energia(Punto *p, Sistema *sist);

int main(int argc, char *argv[])
{
  Punto p;
  Sistema sist;
  Derivata f;
  Integratore passo;
  f=pendolo;
  passo=rk4;
  if(argc!=9)//1
    {
      fprintf(stderr, "inserire: %s ND nsteps stride theta0 omega0 q f Omega\n",argv[0]);
      exit(1);
    }
  sist.ND=atoi(argv[1]);
  sist.nsteps=atoi(argv[2]);
  sist.stride=atoi(argv[3]);
  p.theta0=atof(argv[4]);
  p.omega0=atof(argv[5]);
  sist.q=atof(argv[6]);
  sist.f=atof(argv[7]);
  sist.Omega=atof(argv[8]);
  sist.dtau=(2*PI)/(sist.Omega*sist.ND);
      
  double E0=energia(&p,&sist);
  double tau=0;
  printf("%lf %lf  %lf %lf %lf\n", tau, p.theta0, p.omega0, E0, atan2(sin(p.theta0),cos(p.theta0)));
  for(int i=0;i<sist.nsteps;i++)
    {
      passo(tau,&p,&sist,f);
      double E=energia(&p,&sist);
      tau+=sist.dtau;
      if(i%sist.stride == 0)
	{
	  printf("%lf %lf  %lf %lf %lf\n", tau, p.theta0, p.omega0, E, atan2(sin(p.theta0),cos(p.theta0)));
	}
    }
}

double energia(Punto *p, Sistema *sist)
{
  return 0.5*pow(p->omega0,2)+1-cos(p->theta0);
}

Punto pendolo(double tau,Punto *p, Sistema *sist)
{
  Punto d;
  d.theta0=p->omega0;
  d.omega0=-sin(p->theta0)-(sist->q)*(p->omega0)+(sist->f)*(cos((sist->Omega)*tau));
  return d;
}

void rk4(double tau, Punto *p, Sistema *sist, Derivata f)
{
  Punto k1,k2,k3,k4;
  Punto tmp1,tmp2,tmp3;

  k1=f(tau,p,sist);
  tmp1.theta0=p->theta0+0.5*sist->dtau*k1.theta0;
  tmp1.omega0=p->omega0+0.5*sist->dtau*k1.omega0;

  k2=f(tau+0.5*sist->dtau,&tmp1,sist);
  tmp2.theta0=p->theta0+0.5*sist->dtau*k2.theta0;
  tmp2.omega0=p->omega0+0.5*sist->dtau*k2.omega0;

  k3=f(tau+0.5*sist->dtau,&tmp2,sist);
  tmp3.theta0=p->theta0+sist->dtau*k3.theta0;
  tmp3.omega0=p->omega0+sist->dtau*k3.omega0;

  k4=f(tau+sist->dtau,&tmp3,sist);

  p->theta0=p->theta0+(sist->dtau/6)*(k1.theta0+2*k2.theta0+2*k3.theta0+k4.theta0);
  p->omega0=p->omega0+(sist->dtau/6)*(k1.omega0+2*k2.omega0+2*k3.omega0+k4.omega0);
}
