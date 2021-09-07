#include <stdio.h>
#include <unistd.h>
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
  int cantidad_cambios = 0;
  //printf("id semaforo:%s delay:%s pid_fabrica:%s\n", argv[0], argv[1], argv[2]);
  printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
  //send_signal_with_int(atoi(argv[2]), 99);
  //int delay = strtol(argv[1], NULL, 10);

  void handle_sigabrt(int sig)
  {
    printf("Gracefully finishing\n");

    printf("EL INDICE DEL SEMAFORO ES: %d\n", argv[0]);
    char* fileName = "semaforo_";
    char* fileType = ".txt";
    //int count = 5;
    char nombre_output[20];
    FILE* f = NULL; 

    sprintf(nombre_output, "%s%s%s", fileName, argv[0], fileType);
    printf("generando output semaforo: %i\n", argv[0]);
    f = fopen(nombre_output, "w");
    //FILE *output = fopen(nombre_output, "w");
    fprintf(f, "%i", cantidad_cambios);
    fclose(f);

    exit(0);
  }
  signal(SIGABRT, handle_sigabrt);



  
  while (1)
  {
    send_signal_with_int(atoi(argv[2]), atoi(argv[0]));
    sleep(atoi(argv[1]));
    cantidad_cambios++;
    printf("cantidad  de cambios = %i\n", cantidad_cambios);
    if(cantidad_cambios==10){
      kill(getpid(), SIGABRT);
    }
  }
}
