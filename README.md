*******************************************
*Author:
*asrivas3 Abhishek Kumar Srivastava
*CSC 548 Assignment #2 Problem #1
*Individual Problem
*******************************************

PROBLEM STATEMENT: Create an api simulating MPI using sockets.

INTRODUCTION: In this project we have implemented MPI using socket programming in C language. We have implemented following functions:
1. MPI_Init
2. MPI_Send
3. MPI_Recv
4. MPI_Comm_rank
5. MPI_Comm_size
6. MPI_Barrier
7. MPI_Finalize

PREQUISTIES: A compiler capable of compiling C program and a multiple nodes to run the program.

IMPLEMENTATION:
my_prun:
1. ssh to the nodes the command to execute the program.
2. Pass the rank, number of nodes to the program for processing.

my_mpi:
1. In MPI_Init create a socket and store the port number in a file and all nodes will proceed until all the nodes are done.
2. In MPI_Send, using this port number connect to a particular node to send the data.
3. In MPI_Recv, the Receiving port is listening and will spawn a new socket when connection request arrives.
4. On this new port the receiver will receive the message.
5. In MPI_Barrier, all the nodes send there status to node 0 which in turns reverts when it receives messages from each node.
6. IN MPI_Finalize, the server socket is closed for each node.

PROCEDURE:
1. Create a server socket and listen for connection requests.
2. Wait until each node has started listening.
3. For Sending Data Start a client socket and send the Data.
4. For Receiving the Data accept the connection using Server port then spawn new port for message transfers.
5. For Barrier ech node send some data to Node 0 when every node receives reply from node 0 then they can proceed.
6. For Finalize close the server socket and free all the variables allocated on heap.


RESULTS:
Following Results were obtained by executing the above experiment:
Size	  MeanPair1		StandardDeviationPair1	  MeanPair2		StandardDeviationPair2	  MeanPair3		StandardDeviationPair3	  MeanPair4		StandardDeviationPair4
32 		2.334000e-04 		2.461788e-05 		2.282000e-04 		2.659248e-05 		1.957000e-04 		1.852593e-05 		2.361000e-04 		1.274716e-05
64 		2.251000e-04 		2.317089e-05 		2.278000e-04 		2.399083e-05 		1.996000e-04 		3.693237e-06 		2.478000e-04 		2.009378e-05
128 	2.337000e-04 		4.721663e-05 		1.943000e-04 		8.626123e-06 		2.204000e-04 		1.994593e-05 		2.477000e-04 		1.940129e-05
256 	2.522000e-04 		2.313785e-05 		2.257000e-04 		3.377292e-05 		2.329000e-04 		3.434953e-05 		1.870000e-04 		3.254535e-05
512 	2.370000e-04 		1.551129e-05 		2.229000e-04 		3.545264e-05 		2.690000e-04 		1.127830e-05 		2.088000e-04 		2.762897e-05
1024 	2.749000e-04 		2.201568e-05 		2.331000e-04 		2.783685e-05 		2.984000e-04 		2.290502e-05 		2.223000e-04 		3.543177e-05
2048 	2.527000e-04 		3.434545e-05 		2.859000e-04 		5.243367e-05 		2.665000e-04 		2.249111e-05 		2.347000e-04 		2.151767e-05
4096 	4.251000e-04 		7.486581e-05 		4.299000e-04 		4.592265e-05 		4.161000e-04 		8.193833e-05 		3.707000e-04 		6.536979e-05
8192 	5.917000e-04 		7.689610e-05 		5.429000e-04 		5.395081e-05 		5.376000e-04 		5.445218e-05 		5.154000e-04 		9.005243e-05
16384 	6.320000e-04 		3.557527e-05 		6.133000e-04 		4.639192e-05 		6.270000e-04 		3.051229e-05 		5.962000e-04 		3.079221e-05
32768 	1.055900e-03 		3.073581e-05 		1.015900e-03 		2.299761e-05 		1.058000e-03 		6.249960e-05 		1.046000e-03 		6.844122e-05
65536 	1.774400e-03 		1.432761e-04 		1.719500e-03 		8.599331e-05 		1.760600e-03 		7.855724e-05 		1.724700e-03 		8.005879e-05
131072 	2.622000e-03 		7.683619e-05 		2.612200e-03 		6.909964e-05 		2.644900e-03 		9.821960e-05 		2.612800e-03 		7.125419e-05
262144 	4.795700e-03 		2.216777e-05 		4.789800e-03 		3.184902e-05 		4.792200e-03 		2.506711e-05 		4.783300e-03 		3.195951e-05
524288 	9.383100e-03 		2.440676e-05 		9.381900e-03 		2.928976e-05 		9.387500e-03 		2.734319e-05 		9.391300e-03 		2.525094e-05
1048576 1.825170e-02 		3.782341e-05 		1.824530e-02 		3.759269e-05 		1.821400e-02 		2.849561e-05 		1.823840e-02 		3.585024e-05
2097152 3.594680e-02 		4.997559e-05 		3.600230e-02 		1.087557e-04 		3.599430e-02 		7.038331e-05 		3.598660e-02 		6.790906e-05


The Following inferences are drawn from the results of my_rtt and HW1 Problem #1:

INFERENCE 1:
The custom implementation is taking more time the standard MPI which could be due to file read/write overheads and a few network overheads.

NOTE: 1.Between 2 subsquent executions of the program some time needs to be provided so that the sockets can be freed from previous execution.
----- 2.The program may take long time to execute based on network congestion.

USAGE:	./my_prun -n 8 ./my_rtt
------
--------------------------------------------------------------------------End Of Document----------------------------------------------------------------------------


