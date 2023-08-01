#ifndef __DAEMON_H__
#define __DAEMON_H__

#include <pthread.h>

typedef struct {
  pthread_t tid;
  int exist;
  pthread_mutex_t mutex;
} ExtendProbe;

void *SingleProbe(void *arg);

#endif
