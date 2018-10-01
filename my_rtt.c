/**********************************************
 *Author:
 *	asrivas3 Abhishek Kumar Srivastava
 *
 *CSC-548 Assignment #2 Problem #1
 *********************************************/

#include<stdio.h>
#include"my_mpi.h"
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>

double mean(double *);
double stddev(double*);

// Function to calculate mean of an array of size 10
double mean(double* arr)
{
	double sum = 0;
	int i;
	for (i = 0; i < 10; i++)
		sum += arr[i];
	return (sum / 10);
}

//Function to calculate standard deviation of an array of size 10
double stddev(double* arr)
{
	double u, std = 0.0;
	int i;

	u = mean(arr);

	//printf("%e\n", pow((arr[0] - u), 2));

	for (i = 0; i < 10; i++)
		std += pow(arr[i] - u, 2);

	return sqrt(std/10);
	//return std;
}

int main(int argc, char** argv)
{
	int rank;
	int p;
	long size = 8;
	int i,j;
	int* buffer;
	int tag = 0;
	double result[17][9];
	MPI_Status status;
	struct timeval tmStart, tmEnd, tmDiff;
	double temp;
	//double tmStart, tmEnd, tmDiff;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// If Processors are not 8 in number then abort 
	if (p != 8)
	{
		printf("Total Processors should be 8\n");
		return 0;
	}

	// Initializing buffer of size 1 for dummy transaction
	buffer = (int*)calloc(1, sizeof(int));
/*
	//Dummy Transaction
	if (rank < 4)
	{
		MPI_Send(buffer, 1, MPI_INT, rank + 4, tag, MPI_COMM_WORLD);
		MPI_Recv(buffer, 1, MPI_INT, rank + 4, tag, MPI_COMM_WORLD, &status);
	}
	else
	{
		MPI_Recv(buffer, 1, MPI_INT, rank - 4, tag, MPI_COMM_WORLD, &status);
		MPI_Send(buffer, 1, MPI_INT, rank - 4, tag, MPI_COMM_WORLD);
	}

	free(buffer);
*/
	// Loop for experiment of varying message sizes
	for (i = 1; i <= 17; i++)
	{
		double stat[10];
		//Initializing message for different sizes
		buffer = (int*)calloc(size, sizeof(int));

		// Loop for 10 experiments for each message size
		for(j=0;j<=10;j++)
		{
			//If the rank of the processor is between 0 to 3 inclusive then processor will initiate
			//transaction and receive reply and record the R.T.T.
			if (rank < 4 )
			{
				gettimeofday(&tmStart, NULL);
				//tmStart = MPI_Wtime();
				MPI_Send(buffer, size, MPI_INT, rank + 4, tag, MPI_COMM_WORLD);
				MPI_Recv(buffer, size, MPI_INT, rank + 4, tag, MPI_COMM_WORLD, &status);
				gettimeofday(&tmEnd, NULL);
				//tmEnd = MPI_Wtime();
				//tmDiff = tmEnd - tmStart;
				//printf("%lf\n", tmDiff);
				timersub(&tmEnd, &tmStart, &tmDiff);
				temp = tmDiff.tv_usec / 1000000.0;
				temp += tmDiff.tv_sec;
				if (j != 0)
					stat[j - 1] = temp;
					//stat[j - 1] = tmDiff;
				//stat[j] = tmDiff.tv_sec * 1000000 + tmDiff.tv_usec;
			}
			
			// If the processor rank is between 4 to 7 inclusive the they will receive the message
			// and reply back with same message.
			else
			{
				MPI_Recv(buffer, size, MPI_INT, rank - 4, tag, MPI_COMM_WORLD, &status);
				MPI_Send(buffer, size, MPI_INT, rank - 4, tag, MPI_COMM_WORLD);
			}

		}

		// If Processor rank is between 0 to 3 inclusive the the processor calculates the mean and standard
		// deviation of the experiments for each message sizes and each one of them except 0 will send their
		// results to processor #0 to colate.
		if (rank < 4)
		{
			double u[2];
			u[0] = mean(stat);
			u[1] = stddev(stat);
			//printf("%lf\n", u[1]);
			
			//Each Processor except Processor #0 sends its results to Processor #0
			if (rank != 0)
			{
				MPI_Send(&u, 2, MPI_DOUBLE, 0, size, MPI_COMM_WORLD);
			}
			else
			{
				//Processor #0 receives results from each processor and colates in a 2D array.
				double mean1[2], mean2[2], mean3[2];
				MPI_Status status1, status2, status3;
				MPI_Recv(&mean1, 2, MPI_DOUBLE, 1, size, MPI_COMM_WORLD, &status1);
				MPI_Recv(&mean2, 2, MPI_DOUBLE, 2, size, MPI_COMM_WORLD, &status2);
				MPI_Recv(&mean3, 2, MPI_DOUBLE, 3, size, MPI_COMM_WORLD, &status3);
				result[i - 1][0] = size*sizeof(int);
				result[i - 1][1] = u[0];
				result[i - 1][2] = u[1];
				result[i - 1][3] = mean1[0];
				result[i - 1][4] = mean1[1];
				result[i - 1][5] = mean2[0];
				result[i - 1][6] = mean2[1];
				result[i - 1][7] = mean3[0];
				result[i - 1][8] = mean3[1];
			}
		}
		size = size * 2;
		free(buffer);
		if(rank == 0)
		{
			printf("%d %e %e %e %e %e %e %e %e\n", (int)result[i-1][0], result[i-1][1], result[i-1][2], result[i-1][3], result[i-1][4], result[i-1][5], result[i-1][6], result[i-1][7], result[i-1][8]);
			fflush(stdout);
		}
	}

	MPI_Finalize();
	return 0;
}
