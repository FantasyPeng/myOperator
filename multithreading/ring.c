#include<unistd.h>
#include<stdio.h>
#include<pthread.h>

int globe = 0;

typedef struct {
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;	
}sema_t;

void sema_init(sema_t * sema,int value) {
	sema->value = value;
	pthread_mutex_init(&sema->mutex,NULL);
	pthread_cond_init(&sema->cond,NULL);	
}

void sema_wait(sema_t * sema) {
	pthread_mutex_lock(&sema->mutex);
	sema->value--;
	while(sema->value<0) {
		pthread_cond_wait(&sema->cond,&sema->mutex);	
	}	
	pthread_mutex_unlock(&sema->mutex);
}
void sema_signal(sema_t * sema) {
	pthread_mutex_lock(&sema->mutex);
	++sema->value;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);	
}
int N;
int count = 0;
//pthread_t workers[N+1];
pthread_t init_id;
int fdata;
struct param{
	int data;
};

void *add(void * arg) {
	count++;
	struct param * pa = (struct param *)arg;
	pthread_t id;
	printf(" %lu : %d\n",pthread_self(),pa->data);
	if (count < N) {
		pa->data++;
		pthread_create(&id,NULL,add,pa);
		pthread_join(id,NULL);
		return NULL;	
	} else {
		pa->data++;
		fdata = pa->data;
		return NULL;	
	}
}

int main() {
	struct param mp;
	mp.data = 0 ;
//	struct result * re;
	printf("Please input the N:");
	scanf("%d",&N);
	
	pthread_create(&init_id,NULL,add,&mp);
	pthread_join(init_id,NULL);

	mp.data = fdata;
	
	printf("FINAL DATA: %d\n",mp.data);	

}
