#include <stdio.h>
#include <unistd.h>
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
  printf("%s %s %s\n", argv[0], argv[1], argv[2]);
  printf("I'm the SEMAFORO process an,d my PID is: %i\n", getpid());
  send_signal_with_int(getppid(), 99);
}
