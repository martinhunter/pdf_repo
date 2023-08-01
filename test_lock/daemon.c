#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "daemon.h"
#include "server.h"

void clean(void *arg) {
  pid_t pid = *(pid_t *)arg;
  kill(pid, SIGTERM);
  printf("killed, %d\n", pid);

}

void *SingleProbe(void *arg){
  ExtendProbe *mgr = (ExtendProbe *)arg;
    pid_t pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
    } else if (pid == 0) {
        // Child process
        execl("/root/test_lock/loop", "loop", NULL);
    } else {
      pthread_cleanup_push(clean, &pid);
      mgr->exist = 1;
      int status;
      waitpid(pid, &status, 0);
      pthread_cleanup_pop(0);
    }
    mgr->exist = 0;

}

ExtendProbe *InitProbe(){
    ExtendProbe *mgr = NULL;
    mgr = (ExtendProbe *)malloc(sizeof(ExtendProbe));
    if (mgr == NULL)
        return NULL;
    memset(mgr, 0, sizeof(ExtendProbe));
    pthread_mutex_init(&mgr->mutex, NULL);
    mgr->exist = 10;
    return mgr;
}

void DestroyProbe(ExtendProbe *probe){
    if (probe != NULL)
        free(probe);
}

int main(){
  system("rm -rf /tmp/unix_socket");
  sleep(1);
  ExtendProbe *probe = InitProbe();
  printf("---probe:%p -- %d\n", probe, probe->exist);
  pthread_t server_tid;
  pthread_create(&server_tid, NULL, serve, probe);
  pthread_create(&probe->tid, NULL, SingleProbe, probe);

  pthread_join(probe->tid, NULL);
  pthread_join(server_tid, NULL);
  DestroyProbe(probe);
  return 0;
}
