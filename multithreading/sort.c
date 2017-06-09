#include<stdio.h>
#include<pthread.h>
struct param {
	int start;
	int end;
	int array[80];
};
void * worker(void *arg) {
	int i,j;
	int temp;
	int p;
	struct param * param = (struct param * )arg;
	for (i = param->start; i < param->end; i++) {
		temp = i;
		for (j = i + 1; j <= param->end;j++) {
			if ((param->array)[temp]>(param->array)[j])
				temp = j;
		}
		p = (param->array)[i];
		(param->array)[i] = (param->array)[temp];
		(param->array)[temp] = p;
	}	
}
int main() {
	pthread_t worker_tid;
	struct param param;
	int n,i,j,temp,p;
	printf("please input the number of array:");
	scanf("%d",&n);
	printf("please input the array:\n");
	for (i = 0; i < n;i++)
		scanf("%d",&(param.array[i]));
	param.start = n/2;
	param.end = n-1;
	pthread_create(&worker_tid,NULL,&worker,(void*)&param);
	for (i = 0; i < n/2-1;i++) {
		temp = i;
		for (j = i + 1;j < n/2;j++) {
			if (param.array[temp]>param.array[j])
				temp = j;			
		}	
		p = param.array[i];
		param.array[i] = param.array[temp];
		param.array[temp] = p;
	}	
	pthread_join(worker_tid,NULL);
	for(i = 0,j = n/2;j < n;) {
		if (param.array[j] < param.array[i]) {
			int k;
			temp = param.array[j];
			for (k = j; k > i; k--) {
				param.array[k] = param.array[k - 1];	
			}	
			param.array[i] = temp;
			i++;
			j++;
		}	
		else 
			i++;
	}
		for (i = 0; i < n;i++)
			printf("%d ",param.array[i]);
			printf("\n");
			return 0;
}
