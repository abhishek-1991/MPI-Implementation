/*********************************************************
 * Author:
 * asrivas3 Abhishek Kumar Srivastava
 * CSC 548 Assignment #2 Problem #1
 * ******************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>

#define MPI_COMM_WORLD 0
#define MPI_INT "int"
#define MPI_DOUBLE "double"

typedef struct
{
	int status;
}MPI_Status;

typedef struct
{
	char* ip;
	char* host;
	int rank;
}MPI_Info;

void MPI_Init(int *,char***);
void MPI_Comm_rank(int,int*);
void MPI_Comm_size(int,int*);
void MPI_Send(void*,int,char*,int,int,int);
void MPI_Recv(void*,int,char*,int,int,int,MPI_Status *);
void MPI_Finalize();
