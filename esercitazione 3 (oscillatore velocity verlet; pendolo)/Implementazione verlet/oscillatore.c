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
  void err(Sistema *sist, Punto *p, double xiniz, double viniz);

  int main(int argc, char *argv[])
  {
    Punto p;
    Sistema sist;
    if(argc!=6)
      {
        fprintf(stderr,"inserire: metodo (1/2/3) nsteps x0 v0 omega0\n");
        exit(1);
      }

    int scelta=atoi(argv[1]);
    sist.nsteps=atoi(argv[2]);
    p.x0=atof(argv[3]);
    p.v0=atof(argv[4]);
    double xiniz=p.x0;
    double viniz=p.v0;
    sist.omega0=atof(argv[5]);
    sist.dt=0.001;
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
    for(int j=0;j<7;j++)
    {
      sist.T=0;
      p.x0=xiniz;
      p.v0=viniz;
      int npassi=sist.nsteps;
    for(int i=0;i<npassi;i++)
    {
      sist.T+=sist.dt;
      integrazione(&sist, &p);
    }
    npassi = npassi/pow(2,j);
    err(&sist, &p, xiniz, viniz);
    sist.dt*=2;
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
    //printf("%lf %lf %lf %lf\n", sist->T, p->x0, p->v0, E); 
  }

  void euler_cromer(Sistema *sist, Punto *p)
  {
    p->v0 += -sist->omega0 * sist->omega0 * p->x0 * sist->dt;
    p->x0 += p->v0 * sist->dt;
    double E = Energia(sist, p);
    //printf("%lf %lf %lf %lf\n", sist->T, p->x0, p->v0, E);
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
    //printf("%lf %lf %lf %lf\n", sist->T, p->x0, p->v0, E);
  }

  void err(Sistema *sist, Punto *p, double xiniz, double viniz){
    double e_g=0;
    double x_e = xiniz * cos(sist->omega0*sist->T) + (viniz/sist->omega0) * sin(sist->omega0*sist->T);
    double A = sqrt((xiniz*xiniz)+((viniz*viniz)/(sist->omega0*sist->omega0)));
    e_g = (fabs(p->x0-x_e))/A;
    printf("%lf %lf\n", sist->dt,e_g);
  }
