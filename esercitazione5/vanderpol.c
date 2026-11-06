#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct
{
  double x0;
  double v0;
  double mu;
} Punto;

typedef struct
{
  double dt;
  int nsteps;
  double T;
} Sistema;

double Energia(Sistema *sist, Punto *p);
double zeri(Sistema *sist, Punto *p, double theta, double ptheta);
void stampa_periodi(Punto *p,double x_iniziale, double periodo);
void derivate_oscillatore(double t,Punto *p, Punto *y, Sistema *sist, Punto *deriv);
double rk4(Sistema *sist, Punto *p, void (*f)(double,Punto*, Punto*, Sistema*, Punto*));

int main(int argc, char *argv[])
{
  Punto p;
  Sistema sist;
  if(argc!=7)
    {
      fprintf(stderr,"inserire: dt nsteps stride x0 v0 mu\n");
      exit(1);
    }

  sist.dt=atof(argv[1]);
  sist.nsteps=atoi(argv[2]);
  int stride=atoi(argv[3]);
  p.x0=atof(argv[4]);
  p.v0=atof(argv[5]);
  p.mu=atof(argv[6]);
  sist.T=0;
  double px=0;
  double x=0;
  double t_zero=0;
  double t_prec=0;
  double periodo=0;
  int contatore_semiperiodi=0;
  int contatore_periodi=0;
  double x_iniziale=p.x0;

  double E = Energia(&sist, &p);
  printf("%lf %lf %lf %lf\n", sist.T, p.x0, p.v0,E);

  for(int i=0;i<sist.nsteps;i++)
    {
      x=rk4(&sist, &p, derivate_oscillatore);
      sist.T+=sist.dt;
      E = Energia(&sist, &p);
     
      if(i!=0 && contatore_semiperiodi%2==0)
        {
          t_zero=zeri(&sist,&p,x,px);
          if(t_zero!=0)
	    {
	      contatore_semiperiodi++;
	      if(t_prec!=0)
		{
		  periodo+=(t_zero-t_prec);
		  contatore_periodi++;
		}
	      t_prec=t_zero;
	    }
        }
      else if(i!=0 && contatore_semiperiodi%2!=0)
        {
          t_zero=zeri(&sist,&p,x,px);
          if(t_zero!=0)
	    {
	      contatore_semiperiodi++;
	    }
        }
      px=x;
      if(i%stride==0)
        {
          printf("%lf %lf %lf %lf\n", sist.T, p.x0, p.v0,E);
        }
    }
  periodo=periodo/contatore_periodi;
  stampa_periodi(&p,x_iniziale,periodo);
}

double Energia(Sistema *sist, Punto *p)
{
  double E = 0.5 * (pow(p->x0,2) + pow(p->v0,2));
  //printf("%lf %lf\n", sist->T, E);
  return E;

}

double zeri(Sistema *sist, Punto *p, double x, double px)
{
  double slope=0;
  if(x*px<0)
    {
      slope=(x-px)/sist->dt;
      return (sist->T-sist->dt)-(px)/slope;
    }
  else return 0;
}
    
void stampa_periodi(Punto *p,double x_iniziale, double periodo)
{
  double T0=0;
  if(periodo <= 0.0)
    {
      return;
    }
    if(p->mu < 1)
    {
      T0=2*M_PI;
    }
    else
    {
      T0=(3-2*log(2))*p->mu;
    }
  double T_grafico=periodo/T0;
  //printf("%lf %lf\n", x_iniziale, T_grafico); commentato il periodo per la stampa delle energie
}

void derivate_oscillatore(double t, Punto *p, Punto *y, Sistema *sist, Punto *deriv) {
  deriv->x0 = y->v0;
  deriv->v0 = -p->mu*(y->x0*y->x0 - 1.0)*y->v0 - y->x0;
}

double rk4(Sistema *sist, Punto *p, void (*f)(double,Punto*, Punto*, Sistema*, Punto*)) {
  Punto k1, k2, k3, k4;
  Punto y_supp;
  double dt = sist->dt;
  double t = sist->T;

  f(t,p, p, sist, &k1);

  y_supp.x0 = p->x0 + 0.5 * dt * k1.x0;
  y_supp.v0 = p->v0 + 0.5 * dt * k1.v0;
  f(t + 0.5 * dt,p, &y_supp, sist, &k2);

  y_supp.x0 = p->x0 + 0.5 * dt * k2.x0;
  y_supp.v0 = p->v0 + 0.5 * dt * k2.v0;
  f(t + 0.5 * dt, p, &y_supp, sist, &k3);

  y_supp.x0 = p->x0 + dt * k3.x0;
  y_supp.v0 = p->v0 + dt * k3.v0;
  f(t + dt,p, &y_supp, sist, &k4);

  p->x0 += (dt / 6.0) * (k1.x0 + 2.0 * k2.x0 + 2.0 * k3.x0 + k4.x0);
  p->v0 += (dt / 6.0) * (k1.v0 + 2.0 * k2.v0 + 2.0 * k3.v0 + k4.v0);
  
  return p->x0;
}
