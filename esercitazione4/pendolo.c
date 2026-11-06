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

    double v_verlet(Sistema *sist, Punto *p);
    double Energia(Sistema *sist, Punto *p);
    double zeri(Sistema *sist, Punto *p, double theta, double ptheta);
    void stampa_periodi(Punto *p,double theta_iniziale, double periodo);

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
      double ptheta=0;
      double theta=0;
      double t_zero=0;
      double t_uno=0;
      double t_prec=0;
      double periodo=0;
      int contatore_semiperiodi=0;
      int contatore_periodi=0;
      double theta_iniziale=p.theta0;

      for(int i=0;i<sist.nsteps;i++)
      {
        theta=v_verlet(&sist, &p);
        sist.T+=sist.dt;
        double E = Energia(&sist, &p);
        if(i%stride==0)
        {
          //printf("%lf %lf %lf %lf\n", sist.T, p.theta0, p.omega0,E);
        }
        if(i!=0 && contatore_semiperiodi%2==0)
        {
          t_zero=zeri(&sist,&p,theta,ptheta);
          if(t_zero!=0)
          {
            contatore_semiperiodi++;
            if(t_prec!=0)
            {
              periodo+=(t_zero-t_prec);
              contatore_periodi++;
              //printf("-------------\n Periodo=%lf\n----------\n", periodo);
            }
            t_prec=t_zero;
          }
        }
        else if(i!=0 && contatore_semiperiodi%2!=0)
        {
          t_uno=zeri(&sist,&p,theta,ptheta);
          if(t_uno!=0)
          {
            contatore_semiperiodi++;
          }
        }
        ptheta=theta;
      }
      if(contatore_periodi>0)
      {
      periodo=periodo/contatore_periodi;
      stampa_periodi(&p,theta_iniziale,periodo);
      }
    }


    double Energia(Sistema *sist, Punto *p)
    {
    return 0.5 * M * pow(p->length,2) * pow(p->omega0,2) + M*G*p->length*(1-cos(p->theta0));

    }

    double v_verlet(Sistema *sist, Punto *p)
    {
      double acc=0;
      p->omega0 += 0.5 * (-G/p->length) * sin(p->theta0) * sist->dt;
      p->theta0 += p->omega0 * sist->dt;
      acc = -(G/p->length * sin(p->theta0));
      p->omega0 += 0.5 * acc * sist->dt;
      return p->theta0;
    }

    double zeri(Sistema *sist, Punto *p, double theta, double ptheta)
    {
      double slope=0;
      double t0=0;
      if(theta*ptheta<0 || theta*ptheta==0)
      {
        slope=(theta-ptheta)/sist->dt;
        return t0=(sist->T-sist->dt)-(ptheta)/slope;
      }
      else return 0;
    }
    
    void stampa_periodi(Punto *p,double theta_iniziale, double periodo)
    {
      if(periodo <= 0.0)
      {
        return;
      }
      double T0=2*M_PI*sqrt(p->length/G);
      double T_grafico=periodo/T0;
      printf("%lf %lf\n", theta_iniziale, T_grafico);
    }