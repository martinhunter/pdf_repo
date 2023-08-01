#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
  int ct = 0;
  while (ct < 30){
    sleep(5);
    ct += 5;
    printf("waiting: %d\n", ct);
  }
}
