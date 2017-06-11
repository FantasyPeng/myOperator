#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#define CAPACITY 4
#define ITEM_COUNT 8
char buffer1[CAPACITY];
char buffer2[CAPACITY];
int c1 = 0;
int c2 = 0;

typedef struct {
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} sema_t;

void sema_init(sema_t * sema,int value) {
	sema->value = value;
	pthread_mutex_init(&sema->mutex,NULL);
	pthread_cond_init(&sema->cond,NULL);
}

void sema_wait(sema_t * sema) {
	pthread_mutex_lock(&sema->mutex);
	sema->value--;
	while(sema->value < 0) 
		pthread_cond_wait(&sema->cond,&sema->mutex);
	pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t * sema) {
	pthread_mutex_lock(&sema->mutex);
	++sema->value;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}
int buffer_is_empty(int c) {
	if (c == 1) 
		return c1 == 0;
	else 
		return c2 == 0;
}

int buffer_is_full(int c) {
	if (c == 1)
		return  c1 == 4;
	else 
		return c2 == 4;
}

int get_item1() {
	int i;
	char c = buffer1[0];
	c1--;
	if (c1 == 0)
		buffer1[c1] == '\0';
	else {
		for (i = 0; i < c1; i++ ) 
			buffer1[i] = buffer1[i + 1];
	}
	return c;
}

int get_item2() {
	int i;
	char c = buffer2[0];
	c2--;
	if (c2 == 0)
		buffer2[c2] == '\0';
	else {
		for (i = 0; i < c2; i++ ) 
			buffer2[i] = buffer2[i + 1];
	}
	return c;
}
void put_item1(char c) {
	int i;
	buffer1[c1] = c;
	c1++;	
}
void put_item2(char c) {
	int i;
	buffer2[c2] = c;
	c2++;	
}
/*pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t wait_buffer1_empty;
pthread_cond_t wait_buffer2_empty;

pthread_cond_t wait_buffer1_full;
pthread_cond_t wait_buffer2_full;
*/

sema_t mutex1;
sema_t mutex2;
sema_t wait_buffer1_empty;
sema_t wait_buffer2_empty;
sema_t wait_buffer1_full;
sema_t wait_buffer2_full;

void * consume(void * arg) {
	int i;
	char c;
	for (i = 0; i < ITEM_COUNT; i++) {
//	        printf("*** in the cosume BEGIN ***\n");
		sema_wait(&wait_buffer2_full);
		sema_wait(&mutex2);

		c = get_item2();

		sema_signal(&mutex2);
		sema_signal(&wait_buffer2_empty);
		printf("        consume item: %c \n",c);
	}
	return NULL;
}
void * compute(void * arg) {
	int i;
	char c;
	for(i = 0; i < ITEM_COUNT; i++) {

		sema_wait(&wait_buffer1_full);
		sema_wait(&mutex1);

		c = get_item1();		
		c = c - 32;
	
		sema_signal(&wait_buffer1_empty);
		sema_signal(&mutex1);

		sema_wait(&wait_buffer2_empty);
		sema_wait(&mutex2);
		
		put_item2(c);

		sema_signal(&mutex2);
		sema_signal(&wait_buffer2_full);
//                printf("****in the conpute end*** \n");
		printf("    compute item: %c \n",c);
	}	
	return NULL;
}
void produce() {
	int i;
	char c;
	for (i = 0; i < ITEM_COUNT; i++) {
	sema_wait(&wait_buffer1_empty);
	sema_wait(&mutex1);

	c = i + 'a';
	put_item1(c);
//	printf("buffer[%d],%c\n",i,c);
	printf("produce item: %c \n",c);
	
	sema_signal(&mutex1);
	sema_signal(&wait_buffer1_full);
	}	
}

int main() {
	pthread_t consumer_tid[2];

	sema_init(&mutex1,1);
	sema_init(&mutex2,1);
	sema_init(&wait_buffer1_empty,CAPACITY);
	sema_init(&wait_buffer2_empty,CAPACITY);
	sema_init(&wait_buffer1_full,0);
	sema_init(&wait_buffer2_full,0);

	pthread_create(&consumer_tid[0],NULL,compute,NULL);
	pthread_create(&consumer_tid[1],NULL,consume,NULL);
	produce();
	pthread_join(consumer_tid[1],NULL);
	return 0;
}
