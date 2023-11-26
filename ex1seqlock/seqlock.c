#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "seqlock.h"  /* TODO implement this header file */

pthread_seqlock_t lock;

void *wr(void *i) {
  pthread_seqlock_wrlock(&lock);
  //printf("Counter in lock: %d\n", lock.counter);
  printf("Writer %d enters\n", * (int*)i);
  sleep(1);
  printf("Writer goes out\n");
  pthread_seqlock_wrunlock(&lock);
  pthread_exit(NULL);
}

void *rd(void *i) {
  pthread_seqlock_rdlock(&lock);
  //printf("Counter in lock: %d\n", lock.counter);
  printf("Reader %d enters\n", * (int*)i);
  sleep(1);
  printf("Reader goes out\n");
  pthread_seqlock_rdunlock(&lock);
  pthread_exit(NULL);
}
int main()
{
  int val = 0;
  pthread_t tid[10];
  int n[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  pthread_seqlock_init(&lock);
  //printf("Counter in lock: %d\n", lock.counter);
  for (int i = 0; i < 3; i++) {
     pthread_create(&tid[i], NULL, wr, (void*) &n[i]);
     pthread_create(&tid[i+3], NULL, rd, (void*) &n[i+3]);
   }
   for (int i = 0; i < 2; i++) {
     pthread_create(&tid[i+6], NULL, rd, (void*) &n[i+6]);
   }
   for (int i = 0; i < 2; i++) {
     pthread_create(&tid[i+8], NULL, wr, (void*) &n[i+8]);
   }
  //for (int i = 0; i < 5; i++) {
  //   pthread_create(&tid[i], NULL, wr, (void*) &n[i]);
  //}
  //for (int i = 0; i < 5; i++) {
  //   pthread_create(&tid[i+5], NULL, rd, (void*) &n[i+5]);
  //}
  for (int i = 0; i < 10; i++) {
    pthread_join(tid[i], NULL);

  }
  pthread_exit(NULL);
}
