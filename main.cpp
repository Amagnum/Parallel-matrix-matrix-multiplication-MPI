/*
*  
* Adarsh Baghel
* 
*/

#include <mpi.h>
#include <bits/stdc++.h>
using namespace std;

double *createMatrix(int n,int m){
    double *matrix;
    matrix = new double[n*m];

    for (int h=0; h<n*m; h++) {
        matrix[h] = h+1;
    }

    return matrix;
}

double *takeMatrix(int n,int m){
    double *matrix;
    matrix = new double[n*m];

    for (int h=0; h<n*m; h++) {
        cin >> matrix[h];
    }

    return matrix;
}


int main(int argc, char *argv[])
{
  int pid, nProc, ierr;
  double mytime, maxtime, mintime, avgtime;

  int rows,offset,i,j,k;
  MPI_Status status;

  ierr = MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  int N;
  if(pid==0){
    cin>>N;
  }

  assert(N%nProc==0);

  MPI_Bcast(&N, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

  double a[N][N],b[N][N],c[N][N]; // AxB=C

  if (pid == 0) {
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
        a[i][j]= 2.0;
        b[i][j]= 3.0;
      }
    }
  }

  MPI_Bcast(&b, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  rows = N/nProc;
  MPI_Bcast(&rows, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  mytime = MPI_Wtime(); /*get time just before work section */
  //#####################################


  if(pid==0){
    offset = rows;
    /* send matrix data to the worker tasks */  
    for (int dest=1; dest<nProc; dest++)
    {
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
      offset = offset + rows;
    }

    /* Matrix multiplication */
    for (k=0; k<N; k++)
      for (i=0; i<rows; i++) {
        c[i][k] = 0.0;
        for (j=0; j<N; j++)
          c[i][k] = c[i][k] + a[i][j] * b[j][k];
      }

    /* wait for results from all worker tasks */
    for (i=1; i<nProc; i++)
    {
      MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&c[offset][0], rows*N, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &status);
    }
  } else {
    MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&a, rows*N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);

    /* Matrix multiplication */
    for (k=0; k<N; k++)
      for (i=0; i<rows; i++) {
        c[i][k] = 0.0;
        for (j=0; j<N; j++)
          c[i][k] = c[i][k] + a[i][j] * b[j][k];
      }

    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  //#####################################
  mytime = MPI_Wtime() - mytime; /*get time just after work section*/


  if(pid==0){
    printf("Here is the result matrix:\n");
      for (i=0; i<N; i++) {
        for (j=0; j<N; j++)
          printf("%6.2f   ", c[i][j]);
        printf ("\n");
      }
  }

  MPI_Reduce(&mytime, &maxtime, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&mytime, &mintime, 1, MPI_DOUBLE, MPI_MIN, 0,MPI_COMM_WORLD);
  MPI_Reduce(&mytime, &avgtime, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (pid == 0) 
    {
      avgtime /= nProc;
    printf("Min: %lf Max: %lf Avg: %lf\n", mintime, maxtime, avgtime);
    }

  MPI_Finalize();
}