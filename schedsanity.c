#include "types.h"
#include "user.h"
#define NUM_OF_CHILDS 60
#define MEDIUM_SIZE_LOOP 50
#define LARGE_SIZE_LOOP 500


int calculation(){
	int a = 1+2+3+4;
	return a;
}

void print(){
	printf(1, "hello world!\n");
}


int main(int argc, char *argv[])
{
	//SchedSanity
	int sum_wtime[4];
	int sum_rtime[4];
	int sum_iotime[4];
	int pids[100];
	int wtime;
	int rtime;
	int iotime;
	int i,j;

//Calculation only - These processes will perform asimple calculation within a medium sized loop
	sum_wtime[0] = 0;
	sum_rtime[0] = 0;
	sum_iotime[0] = 0;
	for(i=0; i< NUM_OF_CHILDS; i++){
		int pid;
		pid = fork();
		if(pid == 0){
			for(j=0;j<100;++j){
				calculation();
				}
		exit();	
		}
		else 
			pids[i] = pid;
	
	}

	for(i=0; i< NUM_OF_CHILDS; i++){
			wait2(pids[i],&wtime,&rtime,&iotime);
			sum_wtime[0] += wtime;
			sum_rtime[0] += rtime;
			sum_iotime[0] += iotime;
	   
	}


//Calculation only – These processes will perform simple calculation within a very large loop
	sum_wtime[1] = 0;
	sum_rtime[1] = 0;
	sum_iotime[1] = 0;
	for(i=0; i< NUM_OF_CHILDS; i++){
		int pid;
		pid = fork();
		if(pid == 0){
			for(j=0;j<100000;++j){
				calculation();
				}
		exit();
		}	
		else 
			pids[i] = pid;
	
	}

	for(i=0; i< NUM_OF_CHILDS; i++){
			wait2(pids[i],&wtime,&rtime,&iotime);
                        printf(2 ,"runtime is  %d \n", rtime);
			sum_wtime[1] += wtime;
			sum_rtime[1] += rtime;
			sum_iotime[1] += iotime;
	   
	}

// Calculation + IO – These processes will perform printing to screen within a medium sized loop
	sum_wtime[2] = 0;
	sum_rtime[2] = 0;
	sum_iotime[2] = 0;
	for(i=0; i< NUM_OF_CHILDS; i++){
		int pid;
		pid = fork();
		if(pid == 0){
			for(j=0;j<MEDIUM_SIZE_LOOP;++j){
					print();
				}
		exit();	
		}
		else 
			pids[i] = pid;
	
	}

	for(i=0; i< NUM_OF_CHILDS; i++){
			wait2(pids[i],&wtime,&rtime,&iotime);
			sum_wtime[2] += wtime;
			sum_rtime[2] += rtime;
			sum_iotime[2] += iotime;
	   
	}

// Calculation + IO – These processes will perform printing to screen within a very large loop
	sum_wtime[3] = 0;
	sum_rtime[3] = 0;
	sum_iotime[3] = 0;
	for(i=0; i< NUM_OF_CHILDS; i++){
		int pid;
		pid = fork();
		if(pid == 0){
			for(j=0;j<LARGE_SIZE_LOOP;++j){
					print();
				}
		exit();	
		}
		else 
			pids[i] = pid;
	
	}

	for(i=0; i< NUM_OF_CHILDS; i++){
			wait2(pids[i],&wtime,&rtime,&iotime);
			sum_wtime[3] += wtime;
			sum_rtime[3] += rtime;
			sum_iotime[3] += iotime;
	   
	}
	

	printf(1,"Calculation Medium -  Wait time: %d,  Run time: %d, IO Time: %d\n\n",sum_wtime[0]/NUM_OF_CHILDS,sum_rtime[0]/NUM_OF_CHILDS,sum_iotime[0]/NUM_OF_CHILDS);
	printf(1,"Calculation Large -  Wait time: %d,  Run time: %d, IO Time: %d\n\n",sum_wtime[1]/NUM_OF_CHILDS,sum_rtime[1]/NUM_OF_CHILDS,sum_iotime[1]/NUM_OF_CHILDS);
	printf(1,"Calculation + IO Medium -  Wait time: %d,  Run time: %d, IO Time: %d\n\n",sum_wtime[2]/NUM_OF_CHILDS,sum_rtime[2]/NUM_OF_CHILDS,sum_iotime[2]/NUM_OF_CHILDS);
	printf(1,"Calculation + IO Large -  Wait time: %d,  Run time: %d, IO Time: %d\n\n",sum_wtime[3]/NUM_OF_CHILDS,sum_rtime[3]/NUM_OF_CHILDS,sum_iotime[3]/NUM_OF_CHILDS);

	exit();
}