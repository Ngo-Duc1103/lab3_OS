#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_ITEMS 1
#define THREADS 1
#define LOOPS 2*MAX_ITEMS

int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;

sem_t smphr1, smphr2;

void put (int value);
int get();

void *producer(void *arg){
	int i;
	int tid = (int) arg;
	for (i = 0; i < LOOPS; i++){
		sem_wait(&smphr1);
		put(i);
		printf("Producer %d put data %d\n", tid, i);
		sleep(1);
		sem_post(&smphr1);
	}
	pthread_exit(NULL);
}

void *consumer(void *arg){
	int i, tmp = 0;
	int tid = (int)arg;
	while (tmp != -1){
		sem_wait(&smphr2);
		tmp = get();
		printf("Consumer %d get data %d\n", tid, tmp);
		sleep(1);
		sem_post(&smphr2);
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv){
	int i, j;
	int tid[THREADS];
	pthread_t producers[THREADS];
	pthread_t consumers[THREADS];

	sem_init(&smphr1, 0, 1);
	sem_init(&smphr2, 0, 1);

	for (i = 0; i < THREADS; i++){
		tid[i] = i;

		pthread_create(&producers[i], NULL, producer, (void*) tid[i]);
		pthread_create(&consumers[i], NULL, consumer, (void*) tid[i]);
	}

	for (i = 0; i < THREADS; i++){
		pthread_join(producers[i], NULL);
		pthread_join(consumers[i], NULL);
	}

	sem_destroy(&smphr1);
	sem_destroy(&smphr2);

return 0;
}

void put(int value){
buffer[fill] = value;
fill = (fill + 1) % MAX_ITEMS;
}

int get(){
int tmp = buffer[use];
use = (use + 1) % MAX_ITEMS;
return tmp;
}
