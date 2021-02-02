

// Very simple problem : computation of temperatute
// distribution in 2-dimensional square domain with
// simple boundary conditions.
//
// Iterative Jacobi method, not the most efficient one one,
// (Gauss-Seidel with overrelaxation is much better). But this 
// is the simplest method for a parallel implementation, because
// there are no data dependencies.
//
// 2D array of M rows and N columns. Here are the boundary
// conditions:
//
//       n=0                                 n=N-1
//  m=0   * * * * * * * * * * * * * * * * * * * 
//  m=2   * * * * * * * * * * * * * * * * * * * 
//        * * * * * * * * * * * * * * * * * * * 
//        * * * * * * * * * * * * * * * * * * *
//        * * * * * * * * * * * * * * * * * * *
//        * * * * * * * * * * * * * * * * * * *
//  m=M-1 * * * * * * * * * * * * * * * * * * *
//
//  T = 1 on the front border (n=0)
//  T = 0 on the back border (n=N-1)
//  Linear decrease from 1 to 0 on the upper (m=0)
//  and lower (m=M-1) borders:
//
//  The stationary solution is, obviously, a linear
//  temperature gradient along the x direction where
//  each row has the same temperature dependence as
//  the upper and lower borders.
//
//  The initial condition is the exact solution plus a
//  random value at each grid point. The iterative
//  method must damp the initial random fluctiation and 
//  bring the temperature distribution to a linear gradient.
//
//  In this problem, two matrices U and V are needed. At
//  each iteration, V[m][n] are computed from the neighbor
//  values of U. Then the matrices are swapped (by swapping
//  pointers) so thet U is again the new value of the
//  approximate solution.
// ----------------------------------------------------
//
#include "cor/cor.hpp"

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include <CpuTimer.h>

#define PI     3.1415926535
#define IMUL   314159269
#define IADD   453806245
#define MASK   2147483647
#define SCALE  0.4656612873e-9

class Rand
   {
   private:
   ///////

   long seed;

   public:
   //////
   
   Rand() { seed = 999; }
   Rand(unsigned long s) { seed = (long)s; }
   double draw()
      {
      seed = (seed * IMUL + IADD) & MASK;
      return (seed * SCALE);
      }
   };



std::shared_ptr<cor::Operon_Client> operon;
std::atomic<double> thread_error_global=0;




extern "C"
{
    int Main(int argc, char *argv[]);
}

// ----------------------------------
// Declaration of auxiliary functions
// ----------------------------------
void InputData();
void InitJob(int MM, int NN);
double SetInitialValues();
void ExitJob();
void PrintResult(int n);
double **AllocMatrix(int MM, int NN);
void FreeMatrix(double **d);


// -----------------
// Global variables
// -----------------
double **U, **V;
const double EPS = 1.0e-5;
double initial_error, curr_error;
int   nIter;
bool  moreCycles;

// ------------------------------
// Data read from file "heat.dat"
// ------------------------------
int N, M;
int maxIts;
int nThreads;
int stepReport;

// ---------------------------------------------
// Auxiliary function to prepare next iteration,
// and decide if it must take place
// ---------------------------------------------
bool NextIteration(double error)
   {
   double **swap = U;   // swap pointers
   U = V;
   V = swap;
   nIter++;
   if(nIter%stepReport==0) std::cout << "\n Iteration " << nIter << std::endl;
   if( (error > EPS*initial_error) && (nIter <= maxIts) ) return true;
   else return false;
   }
   
void ExecuteTask()
{
   // The loop range of this thread
   auto [beg, end] = operon->ScheduleStatic(1, M-1);

   double error, thread_error;
   int m, n;
   int rank = operon->GetRank();
   std::cout << "rank: " << rank << std::endl;
   do
      {
      thread_error = 0.0;
      for(m=beg; m<end; m++)
         {
         for(n=1; n<(N-1); n++)
            {
            error = U[m+1][n] + U[m-1][n] + U[m][n-1] + U[m][n+1]
                     - 4 * U[m][n];
      thread_error += fabs(error);
            V[m][n] = U[m][n] + 0.25 * error;    
      }
   }

      //R.Accumulate(thread_error);
      thread_error_global.store(thread_error);
      operon->Wait();
      if(rank==1)
         { moreCycles = NextIteration(thread_error_global.load()); }
      operon->Wait();
      }while(moreCycles);
}

int Main(int argc, char *argv[])
   {
   auto domain = cor::GetDomain();
   int m, n, status;
   double error_norm, resid;
   double **swap;
   int nThreads = 1;
   if(argc==1) nThreads = atoi(argv[0]);
   std::cout << "nThreads: " << nThreads << std::endl; 

   InputData();
   InitJob(M, N);
   //CpuTimer TR;
   
   //TR.Start(); 
   std::uint64_t t = hpx::util::high_resolution_clock::now();

   operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp(),  "", nThreads);
   auto fut1 = operon->Dispatch(ExecuteTask);
   fut1.get();

   std::uint64_t elapsed = hpx::util::high_resolution_clock::now() - t;

   // -------------
   // Output result
   // -------------
   std::cout << "\n\n Data: N = " << N << "   M = " << M 
        << "  maxIts = " << maxIts  << std::endl;
   if(nIter >maxIts) std::cout << "\n Maximal iterations exceeded" << std::endl;
   else 
      {
      std::cout << "\n Initial error " << initial_error;
      std::cout << "\n Final error   " << curr_error;
      std::cout << "\n Iterations    " << nIter;
      std::cout << "\n Elapsed time   " << elapsed / 1e9 << std::endl;
      //TR.Report();
      }

   // PrintResult(10);
   ExitJob();
}

    




/* -------------------------------------------------
 * This function reads from file "heat.dat" the sizes
 * N, M of the problem and the maximal number of
 * iterations, maxIts.
 * Uses traditional C I/O. 
 * ------------------------------------------------*/
void InputData()
    {
    FILE *fp;
    char buffer[128];

    if( !(fp = fopen("/home/tfg/apps-sorces/placor-hpx/examples/heat.dat", "r") ))
	   {
	   std::cout << "\n Input error" << std::endl;
	   exit(0);
	   }
	
    fgets(buffer, 80, fp);
    sscanf(buffer, "%d", &N);
    fgets(buffer, 80, fp);
    sscanf(buffer, "%d", &M);
    fgets(buffer, 80, fp);
    sscanf(buffer, "%d", &maxIts);
    fgets(buffer, 80, fp);
    sscanf(buffer, "%d", &nThreads);
    fgets(buffer, 80, fp);
    sscanf(buffer, "%d", &stepReport);
    fclose(fp);
    }
   
void InitJob(int MM, int NN)
   {
   int n;	   
   U = AllocMatrix(MM, NN);
   V = AllocMatrix(MM, NN);
   initial_error = SetInitialValues();
   nIter = 0;
   }

void ExitJob()
   {
   FreeMatrix(U);
   FreeMatrix(V);
   }


/* ---------------------------------------------------
 * This function sets the boundary and initial values
 * of the U fiels, and returns the norm of the error of
 * the initial configuration.
 * ------------------------------------------------*/
double SetInitialValues()
   {
   int n, m;
   double a, retval, residue;
   Rand R(999);
   
   // ------------------------------------------------
   // Set the U, V initial values  as well as
   // the boundary values on the borders of the domain.
   // First, boundary values
   // ------------------------------------------------
   
   for(m=0; m<M; m++)
      {
      U[m][0] = 1.0;
      V[m][0] = 1.0;
      U[m][N-1] = 0.0;
      V[m][N-1] = 0.0;
      }
 
   a = -1.0/(N-1);
   for(n=0; n<N; n++)
      {
      U[0][n] = a*n+1;
      V[0][n] = a*n+1;
      U[M-1][n] = a*n+1;
      V[M-1][n] = a*n+1;
      }
   
   // ---------------------------------------
   // Interior values : exact result + random
   // ---------------------------------------
   for(m=1; m<(M-1); m++)
      for(n=1; n<(N-1); n++) 
      {
      U[m][n] = a*n+1 + 0.0001*R.draw();
      V[m][n] = a*n+1 + 0.0001*R.draw();
      }


   // -------------------------
   // Compute the initial error
   // -------------------------
   retval = 0;
   for(m=1; m<(M-1); m++)
      for(n=1; n<(N-1); n++)  
         {
         residue = U[m+1][n] + U[m-1][n] + U[m][n-1] + U[m][n+1]
		             - 4 * U[m][n];
         retval += fabs(residue);
         }

   return retval;
   }
    
/*------------------------------ 
 * Memory management functions 
 * Allocations with offset 0.
 *-----------------------------*/
double **AllocMatrix(int M, int N)
    {
    int n;
    double **d;
	
    d = (double **)malloc( M*sizeof(double *) );
    if(!d)
	{	
	std::cout << "\n malloc failed for d" << std::endl;
	exit(0);
	}
    d[0] = (double *)malloc ( (M*N)*sizeof(double) );
    if(!d[0]) 
	{
	std::cout << "\n malloc failed for d[0]" << std::endl;
	exit(0);
	}
    for(n=1; n<M; n++) d[n] = d[n-1] + N;
    return d;
    }

void FreeMatrix(double **d)
    {
    free(d[0]);
    free(d);
    }

void PrintResult(int step)	
   {
   int n;
   int counter = 0;
 
   std::cout << "\n Solution profile for m = M/2 : \n\n";
   for(n=1; n<=(N-1); n+=step)
       {
       std::cout << U[M/2][n] << "    " ;
       counter++;
       if(counter%4 == 0) 
          {
          std::cout << std::endl;
          counter = 0;
          }
       }
   }

    

