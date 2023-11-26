#ifndef PTHREAD_H
#define PTHREAD_H
#include <pthread.h>
typedef struct pthread_seqlock { /* TODO: implement the structure */
  int odd;
  int counter;
  pthread_mutex_t lock;
} pthread_seqlock_t;

static inline void pthread_seqlock_init(pthread_seqlock_t *seqlock) {
  /* TODO: ... */
  seqlock->odd = 0;
  seqlock->counter = 0;
  pthread_mutex_init(&seqlock->lock, NULL);
}

static inline void pthread_seqlock_wrlock(pthread_seqlock_t *seqlock) {
  /* TODO: ... */
  pthread_mutex_lock(&seqlock->lock);
  while (__sync_fetch_and_add(&seqlock->counter, 0) > 1) {
    pthread_mutex_unlock(&seqlock->lock);
    pthread_mutex_lock(&seqlock->lock);
  }
  __sync_fetch_and_or(&seqlock->odd, 1);
  __sync_fetch_and_add(&seqlock->counter, 1);
  return;
}

static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *seqlock) {
  /* TODO: ... */
  __sync_fetch_and_sub(&seqlock->counter, 1);
  __sync_fetch_and_and(&seqlock->odd, 0);
  pthread_mutex_unlock(&seqlock->lock);
  return;
}

static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *seqlock) {
  /* TODO: ... */
  if (__sync_fetch_and_and(&seqlock->odd, 1)) {
    pthread_mutex_lock(&seqlock->lock);
  }
  __sync_fetch_and_add(&seqlock->counter, 1);
  pthread_mutex_unlock(&seqlock->lock);
  return 0;
}

static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *seqlock) {
  /* TODO: ... */
  __sync_fetch_and_sub(&seqlock->counter, 1);
  return 0; 
}
#endif
