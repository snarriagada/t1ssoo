#include <stdio.h>
#include <unistd.h>
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
  int cantidad_cambios = 0;
  printf("id semaforo:%s delay:%s pid_fabrica:%s\n", argv[0], argv[1], argv[2]);
  printf("I'm the SEMAFORO process an,d my PID is: %i\n", getpid());
  //send_signal_with_int(atoi(argv[2]), 99);
  //int delay = strtol(argv[1], NULL, 10);
  while (true)
  {
    send_signal_with_int(atoi(argv[2]), atoi(argv[0]));
    sleep(atoi(argv[1]));
    cantidad_cambios++;
    printf("cantidad  de cambios = %i\n", cantidad_cambios);
  }
}
