/* @ MC.c program generates a large sequence of 2D argon configurations. 
   Each configuration is accepted or rejected according to a certain
   probability condition (the so called Metropolis criterion) that 
   depend mainly on the temperature T and potential energy difference 
   between configurations. This procedure is repeated until the simulation
   reaches equilibrium. The accepted configurations are
   stored in the position.xyz at regular intervals.
   
   @author Naveen kumar Kaliannan
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <string.h>

double       Pr       =  0;                       // Pressure  
const double Pi       = 3.1415926536;             // PI value
const double R        = <set gas constant here>;  // Gas constant (in KJ per mol per K)
double       sigma    = <set sigma here>;        // Lennard Jones potential parameter - Sigma   (in Angstrom)
double       epsilon  = <set epsilon here>;      // Lennard Jones potential parameter - Epsilon (in Kelvin). 
                                                 // Use boltzmann constant and avagardo number to convert the KJ per mol to Kelvin.


// generates a random  integer between [min,max)
int Rand_INT(int min,int max) //[min,max)
{
  return rand()%(max-min)+min;
}

// generates the random number between 0 and 1
double Rand_DOUBLE() //(0,1)
{
  return (double) rand() / (double) RAND_MAX;
}

// finds the minimum (a,b)
double min(double a,double b)
{
  if(a>b)
  {
    return b;
  }
  else
  {
    return a;
  }
} 

// Computes the vector norm
double norm(double x,double y)
{
  return sqrt(x*x+y*y);
}


// Computes the dot product of the two vector [x,y,z] and [xx yy zz]
double dp(double x,double y,double z,double xx,double yy,double zz)
{
  return x*xx+y*yy+z*zz;
}

void Save(int dim,int N,double **r,double **r_new)
{
  for(int i = 0;i < N;i++)
  {
    for(int j = 0; j < (dim+1); j++)
    {
      r_new[j][i] = r[j][i]; 
    }
  }
}


double LenJones(int N,double** r,double L)
{
  double u = 0;
  double rij  = 0, rij_ = 0;
  double dx,dy;  L = 0;

  for(int i = 0;i < N-1;i++)
  {
    for(int j = i+1;j < N;j++)
    {
      dx = r[0][i]-r[0][j];
      dy = r[1][i]-r[1][j];
 
      rij = norm(dx,dy);

      rij_ = pow(sigma/rij,6);

      if(rij <= 100)
      {
        u = u + 4*epsilon*(rij_*rij_ - rij_);
      }
    }
  }
  return u; 
}


int main(int argc, char *argv[])
{
  double T1 = 0;
  double T2 = 0;
  int iter = 0;
  int N = 0;

  char temp[12];
  FILE *fp1;
  fp1 = fopen("in.inp", "r+");
  fscanf(fp1, "%s",temp);
  fscanf(fp1, "%s",temp);
  N = atof (temp);
  fscanf(fp1, "%s",temp);
  fscanf(fp1, "%s",temp);
  iter = atof (temp);
  fscanf(fp1, "%s",temp);
  fscanf(fp1, "%s",temp); 
  T1 = 1.00 * atof (temp);
  fscanf(fp1, "%s",temp);
  fscanf(fp1, "%s",temp);
  T2 = 1.00 * atof (temp);
  fclose(fp1);

 
  int dim = 3;
  double L = 6;
  double V = L*L*L;
  double V_new =  V;
  double** r = (double **)malloc((dim)*sizeof(double));
  for(int i = 0;i < (dim); i++)
  {
    r[i] = (double *)malloc(N*sizeof(double));
  }
  double** r_new = (double **)malloc((dim)*sizeof(double));
  for(int i = 0;i < (dim); i++)
  {
    r_new[i] = (double *)malloc(N*sizeof(double));
  }
   
  for(int i = 0;i < N;i++)
  {
    r[0][i] = i;
    r[1][i] = 0;
    r[2][i] = 0;
    r_new[0][i] = r[0][i];
    r_new[1][i] = 0;
    r_new[2][i] = 0;
  }

  double u = 0, u_new = 0, weight = 0;
  u = LenJones(N,r,L);
  FILE *fp;
  FILE *fp_RDF;
  FILE *fenergy;
  fp = fopen("movie_Starting_and_Final_Configuration.xyz","w+");
  fp_RDF = fopen("Compute_RDF_using_me.xyz","w+");
  fenergy = fopen("Energy.dat","w+");
 // printf("\nMC-steps \t Potenital energy (eV) \n"); 
  for(int k = 0 ;k < iter;k++)
  {

    // Storing the data
    if(k%100 == 0 && k > 0)
      {
        fprintf(fenergy," %i %lf \n", k , u);
      }   
    if(k%100 == 0)
      {
        //printf("%i \t \t %lf \n",k, u); 
        fprintf(fp," 7 \n \n");
        for(int ii = 0; ii < N ;++ii)
          {
            fprintf(fp," Ar %lf %lf %lf \n", r[0][ii], r[1][ii], r[2][ii]);
          }
      }
    if(k >= (iter/2) && k%100 == 0)
      {
        //printf("%i \t \t %lf \n",k, u); 
        fprintf(fp_RDF," 7 \n \n");
        for(int ii = 0; ii < N ;++ii)
          {
            fprintf(fp_RDF," Ar %lf %lf %lf \n", r[0][ii], r[1][ii], r[2][ii]);
          }
      }
     
    int P = Rand_INT(0,N);
    r_new[0][P] += 1*(2*Rand_DOUBLE()-1); 
    r_new[1][P] += 1*(2*Rand_DOUBLE()-1);

    u_new = LenJones(N,r_new,L);

   if(k < iter/2)
    {
      weight = exp(-(1/(R*T1))*((u_new-u)));
    }
    else if(k >= iter/2)
    {
      weight = exp(-(1/(R*T2))*((u_new-u)));
    }

    if(min(1,weight) > Rand_DOUBLE())
    {
      Save(dim-1,N,r_new,r);
      u = u_new;
    }
    else
    {
      Save(dim-1,N,r,r_new);
      u_new = u;
    }



  }
  return 0;
}
