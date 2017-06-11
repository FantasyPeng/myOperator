#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#define CAPACITY 4
#define ITEM_COUNT 8
char buffer1[CAPACITY];
char buffer2[CAPACITY];
int c1 = 0;
int c2 = 0;
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
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex_c1;
pthread_mutex_t mutex_c2;
pthread_cond_t wait_buffer1_empty;
pthread_cond_t wait_buffer2_empty;

pthread_cond_t wait_buffer1_full;
pthread_cond_t wait_buffer2_full;

void * consume(void * arg) {
	int i;
	char c;
	for (i = 0; i < ITEM_COUNT; i++) {
		pthread_mutex_lock(&mutex2);
	//	pthread_mutex_lock(&mutex_c2);
		while(buffer_is_empty(2)) {
		//	pthread_mutex_unlock(&mutex2);	
			pthread_cond_wait(&wait_buffer2_full,&mutex2);
		}	
		c = get_item2();
		pthread_cond_signal(&wait_buffer2_empty);
		pthread_mutex_unlock(&mutex2);
		printf("        consume item: %c \n",c);
	}
	return NULL;
}
void * compute(void * arg) {
	int i;
	char c;
	for(i = 0; i < ITEM_COUNT; i++) {
		pthread_mutex_lock(&mutex1);
		pthread_mutex_lock(&mutex2);
		while(buffer_is_empty(1))
			pthread_cond_wait(&wait_buffer1_full,&mutex1);
		while(buffer_is_full(2))
			pthread_cond_wait(&wait_buffer2_empty,&mutex2);
		c = get_item1();		
		c = c - 32;
		put_item2(c);
		pthread_cond_signal(&wait_buffer2_full);
		pthread_cond_signal(&wait_buffer1_empty);
		pthread_mutex_unlock(&mutex2);	
		pthread_mutex_unlock(&mutex1);	
		printf("    compute item: %c \n",c);
	}	
	return NULL;
}
void produce() {
	int i;
	char c;
	for (i = 0; i < ITEM_COUNT; i++) {
	pthread_mutex_lock(&mutex1);
	while(buffer_is_full(1))
		pthread_cond_wait(&wait_buffer1_empty,&mutex1);
	c = i + 'a';
	put_item1(c);
//	printf("buffer[%d],%c\n",i,c);
	pthread_cond_signal(&wait_buffer1_full);
	pthread_mutex_unlock(&mutex1);		
	printf("produce item: %c \n",c);
	}	
}

int main() {
	pthread_t consumer_tid[2];
	pthread_mutex_init(&mutex1,NULL);
	pthread_mutex_init(&mutex2,NULL);
	pthread_cond_init(&wait_buffer1_empty,NULL);	
	pthread_cond_init(&wait_buffer2_empty,NULL);	
	pthread_cond_init(&wait_buffer1_full,NULL);	
	pthread_cond_init(&wait_buffer2_full,NULL);
	pthread_create(&consumer_tid[0],NULL,compute,NULL);
	pthread_create(&consumer_tid[1],NULL,consume,NULL);
	produce();
	pthread_join(consumer_tid[1],NULL);
	return 0;
}
