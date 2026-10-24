  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  #include <string.h>

  #define M 1
  #define G 9.81

  typedef struct
  {
    double theta0;
    double omega0;
    double length;
  } Punto;

  typedef struct
  {
    double dt;
    int nsteps;
    double T;
  } Sistema;

  void v_verlet(Sistema *sist, Punto *p);
  double Energia(Sistema *sist, Punto *p);

  int main(int argc, char *argv[])
  {
    Punto p;
    Sistema sist;
    if(argc!=7)
      {
        fprintf(stderr,"inserire: dt nsteps stride theta0 omega0 length\n");
        exit(1);
      }

    sist.dt=atof(argv[1]);
    sist.nsteps=atoi(argv[2]);
    int stride=atoi(argv[3]);
    p.theta0=atof(argv[4]);
    p.omega0=atof(argv[5]);
    p.length=atof(argv[6]);
    sist.T=0;

    for(int i=0;i<sist.nsteps;i++)
    {
      v_verlet(&sist, &p);
      sist.T+=sist.dt;
      double E = Energia(&sist, &p);
      if(i%stride==0)
      {
        printf("%lf %lf %lf %lf\n", sist.T, p.theta0, p.omega0,E);
      }
    }
  }

  double Energia(Sistema *sist, Punto *p)
  {
  return 0.5 * M * pow(p->length,2) * pow(p->omega0,2) + M*G*p->length*(1-cos(p->theta0));
  }

  void v_verlet(Sistema *sist, Punto *p)
  {
    double acc=0;
    p->omega0 += 0.5 * (-G/p->length) * sin(p->theta0) * sist->dt;
    p->theta0 += p->omega0 * sist->dt;
    acc = -(G/p->length * sin(p->theta0));
    p->omega0 += 0.5 * acc * sist->dt;
  }
   