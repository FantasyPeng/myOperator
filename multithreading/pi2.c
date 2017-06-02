#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define LEN 10000
#define MAX_WORKERS 100
struct param {
	int start;
	int end;	
};
struct result {
	float sum;	
};

void *compute(void * arg) {
	int i;
	float j;
	struct param * myparam;
	myparam = (struct param *)arg;
	int start = myparam -> start;
	int end = myparam -> end;
	struct result * myresult;
	float sum1 = 0,sum2 = 0,sum3 = 0;
	for (i = start; i <= end; i++) {
		j = i;
		if (i % 2 == 0) {
			sum1 += 1/(2 * j - 1);	
		}	
		if (i % 2 == 1) {
			sum2 += 1/(2 * j - 1);
		}
	}	
	myresult = malloc (sizeof (struct result));
	myresult -> sum = sum2 - sum1;
	return myresult;
}

int main() {
	int thread_num = 1;
	struct param myparams[MAX_WORKERS + 1];
	pthread_t workers[MAX_WORKERS];
	printf("please input thread number:");
	scanf("%d",&thread_num);
	int i;
	myparams[0].start = 0;
	myparams[0].end = 0;
	for (i = 1; i <= thread_num - 1; i++) {
		struct param * myparam;
		myparam = &myparams[i];
		myparam->start = myparams[i - 1].end + 1;
		myparam->end = myparams[i].start + (LEN/thread_num) - 1;
		pthread_create(&workers[i-1],NULL,compute,myparam);	
	}
	myparams[thread_num].start = myparams[thread_num - 1].end + 1;
	myparams[thread_num].end = LEN;
	pthread_create(&workers[thread_num - 1],NULL,compute,&myparams[thread_num]);
	int j;
	float sum = 0;
	for (j = 0; j < thread_num - 1; j++) {
		struct result * myresult;
		pthread_join(workers[j],(void **)&myresult);
		sum += myresult->sum;
		free(myresult);
	}
	printf("%f\n",sum);
	return 0;
}
