#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define MAX_LOG_LENGTH 10
#define MAX_BUFFER_SLOT 6
#define MAX_LOOPS 30

char logbuf[MAX_BUFFER_SLOT][MAX_LOG_LENGTH];
pthread_mutex_t mtx;
int count;

void flushlog();


void *wrlog(void *data)
{
   char str[MAX_LOG_LENGTH];
   int id = *(int*) data;
   sprintf(str, "%d", id);
   sleep(1);
   pthread_mutex_lock(&mtx);

   strcpy(logbuf[count], str);
   
   
   if (count == MAX_BUFFER_SLOT) raise(SIGUSR1);
   else count++;

   pthread_mutex_unlock(&mtx);

   //printf("wrlog(): %d \n", id);

   pthread_exit(NULL);
}

void flushlog()
{
   int i;
   char nullval[MAX_LOG_LENGTH];

   //printf("flushlog()\n");
   sprintf(nullval, "%d", -1);
   for (i = 0; i < count; i++)
   {
      printf("Slot  %i: %s\n", i, logbuf[i]);
      strcpy(logbuf[i], nullval);
   }

   fflush(stdout);

   /*Reset buffer */
   count = 0;
   return;
}

void signalHandler(int signalNum) {
  if (signalNum == SIGUSR1) {
   flushlog();
  }
}

int main()
{
   int i;
   count = 0;
   pthread_t tid[MAX_LOOPS];
   pthread_t lgrid;
   int id[MAX_LOOPS];

   pthread_mutex_init(&mtx, NULL);

   // Use SIGUSR1 to handle flushlog predically
   signal(SIGUSR1, signalHandler);

   /*Asynchronous invoke task writelog */
   for (i = 0; i < MAX_LOOPS; i++)
   {
      id[i] = i;
      pthread_create(&tid[i], NULL, wrlog, (void*) &id[i]);
   }

   for (i = 0; i < MAX_LOOPS; i++)
      pthread_join(tid[i], NULL);

   sleep(5);

   pthread_mutex_destroy(&mtx);
   return 0;
}
