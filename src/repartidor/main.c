#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  printf("dist1: %s dist2: %s dist3: %s distBodega:\n", argv[0], argv[1], argv[2], argv[3]);
  printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());

  while (1)
  {
    //send_signal_with_int(atoi(argv[2]), atoi(argv[0]));
    sleep(1);
    //cantidad_cambios++;
    //printf("cantidad  de cambios = %i\n", cantidad_cambios);
  }
}
