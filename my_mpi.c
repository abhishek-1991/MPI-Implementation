/********************************************
 * Author:
 * asrivas3 Abhishek Kumar Srivastava
 * CSC 548 Assignment#2 Problem#1
 * *****************************************/

#include"my_mpi.h"

char* info[8],*iplist[8];
int port;
char ip[8][15];
int rank;
int number_of_nodes;
int servsocket,clisocket,acceptsocket;
int portlist[8]={25000,25001,25002,25003,25004,25005,25006,25007};
int clientfd[8]={-1,-1,-1,-1,-1,-1,-1,-1};
int acceptfd[8]={-1,-1,-1,-1,-1,-1,-1,-1};

int host2ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
         
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        herror("gethostbyname");
        return 1;
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
     
    return 1;
}

void MPI_Init(int *argc,char*** argv)
{
	//Input data:
	//1. Filename
	//2. Port not required now
	//3. my rank
	char* filename = argv[0][1],ip[20];
	port = 0;
	rank = atoi(argv[0][3]);
	char* line=NULL;
	int i,read;
	size_t len=0;
	char temp[15], portfile[20];
	struct sockaddr_in serv_addr,serv_info;

	//Open the file to read the hosts and determine their IP and store them for use.
	FILE *file;
	file = fopen(filename,"r");

	int count=0;

	for(i=0;i<8;i++)
	{
		read = getline(&line,&len,file);
		info[i] = (char*)malloc(read);
		iplist[i] = (char*)malloc(20);
		line[strcspn(line, "\n")] = '\0';
		strcpy(info[i],line);
		host2ip(line,ip);
		strcpy(iplist[i],ip);
		number_of_nodes++;
	}
	fclose(file);
	
	//Start Server Socket
	servsocket = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	bind(servsocket, (struct sockaddr *) &serv_addr,sizeof(serv_addr));
	i = sizeof(serv_info);
	getsockname(servsocket,(struct sockaddr *)&serv_info,&i);
	listen(servsocket,100);
	port=ntohs(serv_info.sin_port);

	//Create file for each node containing their trespective port numbers
	sprintf(portfile,"rank%d",rank);
	file = fopen(portfile,"w");
	fprintf(file,"%d",port);
	fclose(file);

	//Wait until every node is done upto here
	while(1)
	{
		int count = 0;
		if(access("rank0",F_OK) != -1)
			count++;
		if(access("rank1",F_OK) != -1)
			count++;
		if(access("rank2",F_OK) != -1)
			count++;
		if(access("rank3",F_OK) != -1)
			count++;
		if(access("rank4",F_OK) != -1)
			count++;
		if(access("rank5",F_OK) != -1)
			count++;
		if(access("rank6",F_OK) != -1)
			count++;
		if(access("rank7",F_OK) == -1)
			count++;

		if(count == 7)
			break;
	}
}

//Function to return rank of current node.
void MPI_Comm_rank(int flag,int* rnk)
{
	*rnk = rank;
}

//Function to return total number of nodes.
void MPI_Comm_size(int flag,int* number)
{
	*number = number_of_nodes;
}

//Function to send Data.
void MPI_Send(void* msg,int count,char* datatype,int dest,int tag,int comm)
{
	
	struct sockaddr_in client_addr;
	struct hostent *serv;
	int dsize=0,val;
	char filename[20];
	FILE *file;
	int port;

	//Check the type of input.
	if(strcmp(datatype,"int")==0)
		dsize=sizeof(int);
	else if(strcmp(datatype,"double")==0)
		dsize=sizeof(double);
	else
		dsize=sizeof(char);
	
	//If connection decsriptor of socket is not known then create the descriptor and store it for future use.
	if(clientfd[dest]==-1)
	{
		sprintf(filename,"rank%d",dest);
		file = fopen(filename,"r");
		fscanf(file,"%d",&port);
		fclose(file);
		clisocket = socket(AF_INET,SOCK_STREAM,0);
		serv = gethostbyname(info[dest]);
		bzero((char*)&client_addr,sizeof(client_addr));
		client_addr.sin_family = AF_INET;
		bcopy((char*)serv->h_addr,(char*)&client_addr.sin_addr.s_addr,serv->h_length);
		client_addr.sin_port = htons(port);
		val=connect(clisocket,(struct sockaddr *) &client_addr,sizeof(client_addr));
		clientfd[dest]=clisocket;
	}
	//If descriptor is known then use it.
	else
	{
		clisocket = clientfd[dest];
	}
	write(clisocket,msg,count*dsize);
}

void MPI_Recv(void* msg,int count,char* datatype,int source,int tag,int comm,MPI_Status * status)
{
	struct sockaddr_in client_addr;
	int dsize,i;
	char response;
	int idx,sourcerank=0;
	char source_ip[20];

	//Check the type of input.
	if(strcmp(datatype,"int")==0)
		dsize=sizeof(int);
	else if(strcmp(datatype,"double")==0)
		dsize=sizeof(double);
	else
		dsize=sizeof(char);

	//If connection descriptor is not known then create it when connection request is made and store it for future use.
	if(clientfd[source]==-1)
	{
		socklen_t clilen = sizeof(client_addr);
		acceptsocket = accept(servsocket,(struct sockaddr*)&client_addr,&clilen);
		strcpy(source_ip,inet_ntoa(client_addr.sin_addr));
		clientfd[source]=acceptsocket;
	}
	//If descriptor is known then use it.
	else
	{
		acceptsocket = clientfd[source];
	}
	recv(acceptsocket,msg,count*dsize,MSG_WAITALL);
}

//Function to make sure if every node is at same point.
void MPI_Barrier()
{
	fflush(stdout);
	int i = 1;
	MPI_Status status;
	if(rank != 0)
	{
		MPI_Send(&i,1,MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Recv(&i,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
	}
	else
	{
		MPI_Recv(&i,1,MPI_INT,1,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&i,1,MPI_INT,2,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&i,1,MPI_INT,3,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&i,1,MPI_INT,4,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&i,1,MPI_INT,5,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&i,1,MPI_INT,6,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&i,1,MPI_INT,7,0,MPI_COMM_WORLD,&status);
		MPI_Send(&i,1,MPI_INT,1,0,MPI_COMM_WORLD);
		MPI_Send(&i,1,MPI_INT,2,0,MPI_COMM_WORLD);
		MPI_Send(&i,1,MPI_INT,3,0,MPI_COMM_WORLD);
		MPI_Send(&i,1,MPI_INT,4,0,MPI_COMM_WORLD);
		MPI_Send(&i,1,MPI_INT,5,0,MPI_COMM_WORLD);
		MPI_Send(&i,1,MPI_INT,6,0,MPI_COMM_WORLD);
		MPI_Send(&i,1,MPI_INT,7,0,MPI_COMM_WORLD);
	}
}

//Close the server socket and free the allocated memory.
void MPI_Finalize()
{
	int i;
	MPI_Barrier();
	close(servsocket);
	for(i=0;i<8;i++)
	{
		free(info[i]);
		free(iplist[i]);
	}
}
