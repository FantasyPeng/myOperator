#include<stdio.h>
#include<pthread.h>

double worker_out;
void * worker(void * arg) {
	int i;
	double sum = 0;
	int * par = (int * ) arg;
	int temp;
	int state;
	if (((*par) / 2) % 2)
		state = -1;
	else 
		state = 1;
	if ((*par) % 2)
		temp = *par;
	else 
		temp = *par + 1;
	for (i = (*par) / 2; i < *par; i++) {
		sum += 1.0 /temp * state;
		state *= -1;
		temp += 2;	
	}
	worker_out = sum;
}

int main() {
	pthread_t worker_tid;
	double sum = 0;
	int par,temp = 1,state = 1;
	printf("Please enter the number of items:");
	scanf("%d",&par);
	pthread_create(&worker_tid,NULL,worker,(void*)&par);
	
	for (int i = 0; i < par/2; i++) {
		sum += 1.0 / temp * state;
		temp += 2;
		state *= -1;	
	}	
	pthread_join(worker_tid,NULL);
	sum += worker_out;
	printf("res:%f\n",sum);
	return 0;
}
