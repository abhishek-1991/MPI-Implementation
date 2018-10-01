#Author : 
#asrivas3 Abhishek Kumar Srivastava
#CSC548 - Assignment-2 - Question 1
all: my_mpi.c my_rtt.c
	gcc -o my_rtt my_mpi.c my_rtt.c -lm

clean:
	rm -f my_rtt
