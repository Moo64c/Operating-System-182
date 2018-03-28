#include "types.h"
#include "user.h"
#define NUM_OF_CHILDS 30
#define MEDIUM_SIZE_LOOP 50
#define LARGE_SIZE_LOOP 500
#define VERY_LARGE_SIZE_LOOP 5000000
int number_of_prints = 0;
int number_of_calcs = 0;

int calculation(){
	number_of_calcs++;
	int a = 1+2+3+4;
	if (number_of_calcs % 2) {
		a = 5+a;
	}

	return a;
}

void print() {
	number_of_prints++;
	printf(1, "pid %d - this is print number %d\n", getpid(), number_of_prints);
}

int main(int argc, char *argv[]) {
	//SchedSanity
	int sum_wtime[4];
	int sum_rtime[4];
	int sum_iotime[4];
	int pids[100];
	int wtime;
	int rtime;
	int iotime;
	int i,j;
	double temp_for_calc = 4.2;
	double largiiii = 3.14;

	//Calculation only - These processes will perform asimple calculation within a medium sized loop
	sum_wtime[0] = 0;
	sum_rtime[0] = 0;
	sum_iotime[0] = 0;
	for (i=0; i< NUM_OF_CHILDS; i++) {
		int pid;
		pid = fork();
		if (pid == 0) {
			for(j=0;j<100;++j) {
				temp_for_calc = 1.23*(temp_for_calc + calculation());
			}
			exit();
		}
		else
		pids[i] = pid;

	}

	for (i=0; i< NUM_OF_CHILDS; i++) {
		wait2(pids[i],&wtime,&rtime,&iotime);
		sum_wtime[0] += wtime;
		sum_rtime[0] += rtime;
		sum_iotime[0] += iotime;

	}

	//Calculation only – These processes will perform simple calculation within a very large loop
	sum_wtime[1] = 0;
	sum_rtime[1] = 0;
	sum_iotime[1] = 0;
	temp_for_calc = 4.2;

	for(i=0; i< NUM_OF_CHILDS; i++) {
		int pid;
		pid = fork();
		if(pid == 0) {
			for(j=0; j < VERY_LARGE_SIZE_LOOP;++j) {
				temp_for_calc = (1.23 * (temp_for_calc + calculation())) + largiiii;
				largiiii = temp_for_calc / 5.9562;
			}
			exit();
		}
		else
		pids[i] = pid;

	}

	for (i=0; i< NUM_OF_CHILDS; i++) {
		wait2(pids[i], &wtime, &rtime, &iotime);
		sum_wtime[1] += wtime;
		sum_rtime[1] += rtime;
		sum_iotime[1] += iotime;

	}

	// Calculation + IO – These processes will perform printing to screen within a medium sized loop
	sum_wtime[2] = 0;
	sum_rtime[2] = 0;
	sum_iotime[2] = 0;
	for (i=0; i < NUM_OF_CHILDS; i++) {
		int pid;
		pid = fork();
		if(pid == 0){
			for (j=0; j < MEDIUM_SIZE_LOOP; ++j) {
				print();
			}
			exit();
		}
		else
		pids[i] = pid;

	}

	for (i = 0; i < NUM_OF_CHILDS; i++) {
		wait2(pids[i], &wtime, &rtime, &iotime);
		sum_wtime[2] += wtime;
		sum_rtime[2] += rtime;
		sum_iotime[2] += iotime;

	}

	// Calculation + IO – These processes will perform printing to screen within a very large loop
	sum_wtime[3] = 0;
	sum_rtime[3] = 0;
	sum_iotime[3] = 0;
	for (i = 0; i < NUM_OF_CHILDS; i++) {
		int pid;
		pid = fork();
		if (pid == 0) {
			for (j=0; j < LARGE_SIZE_LOOP; ++j) {
				print();
			}
			exit();
		}
		else
		pids[i] = pid;

	}

	for (i=0; i< NUM_OF_CHILDS; i++) {
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
