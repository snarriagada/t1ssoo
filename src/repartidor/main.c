#include <stdio.h>
#include <unistd.h>
#include "../file_manager/manager.h"


int estado_semaforo1 = 0; // 0 es verde; 1 es rojo
int estado_semaforo2 = 0;
int estado_semaforo3 = 0;

void handle_sigusr1(int sig, siginfo_t *siginf, void *ptr) 
{
  //printf ("Signal RECIBIDA EN REPARTIDOR: %d\n", sig);
  int number_received = siginf->si_value.sival_int;
  //printf ("info RECIBIDA EN REPARTIDOR pid %d: semaforo_id %d\n",getpid(), number_received);
  if (number_received == 0){
    if (estado_semaforo1 == 0){
      estado_semaforo1 = 1;
    }else{
      estado_semaforo1 = 0;
    }
  }else if (number_received == 1){
    if (estado_semaforo2 == 0){
      estado_semaforo2 = 1;
    }else{
      estado_semaforo2 = 0;
    }
  }else{
    if (estado_semaforo2 == 0){
      estado_semaforo2 = 1;
    }else{
      estado_semaforo2 = 0;
    }
  }
}

int main(int argc, char const *argv[])
{
  connect_sigaction(SIGUSR1, handle_sigusr1);

  //printf("dist1: %s dist2: %s dist3: %s distBodega:%s\n", argv[0], argv[1], argv[2], argv[3]);
  //printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());

  int dist_semaforo1 = argv[0];
  int dist_semaforo2 = argv[1];  
  int dist_semaforo3 = argv[2];
  int dist_bodega = argv[3];

  int tiempo_semaforo1 = 0;
  int tiempo_semaforo2 = 0;  
  int tiempo_semaforo3 = 0;
  int tiempo_bodega = 0;

  int turnos = 0;

  int dist_repartidor = 0;

  //int estado_semaforo1 = 0; // 0 es verde; 1 es rojo
  //int estado_semaforo2 = 0;
  //int estado_semaforo3 = 0;

  while (1)
  {
    //send_signal_with_int(atoi(argv[2]), atoi(argv[0]));
    sleep(1);
    turnos++;
    //cantidad_cambios++;
    printf("dist_s1 %i dist_s2 %i dist_s3 %i dist_rep %i\n", dist_semaforo1,dist_semaforo2,dist_semaforo3,dist_repartidor);

    if(dist_repartidor == (dist_semaforo1 - 1)){
      printf("if1\n");
      // consultar estado semaforo -> avanzo/no avanzo
      if(estado_semaforo1 == 0){
        // puedo avanzar
        tiempo_semaforo1 = turnos;
        dist_repartidor++;
        printf("%i paso el primer semaforo \n", getpid());
      }
    }else if(dist_repartidor == (dist_semaforo2 - 1)){
      printf("if2\n");
      // consultar estado semaforo -> avanzo/no avanzo
      if(estado_semaforo2 == 0){
        // puedo avanzar
        tiempo_semaforo2 = turnos;
        dist_repartidor++;
        printf("%i paso el segundo semaforo \n", getpid());
      }
    }else if(dist_repartidor == (dist_semaforo3 - 1)){
      printf("if3\n");
      // consultar estado semaforo -> avanzo/no avanzo
      if(estado_semaforo3 == 0){
        // puedo avanzar
        tiempo_semaforo3 = turnos;
        dist_repartidor++;
        printf("%i paso el tercer semaforo \n", getpid());
      }
    }else{
      printf("if4\n");
      // si llega aqui puede avanzar
      dist_repartidor++; 
    }
    if(dist_repartidor == dist_bodega){
      tiempo_bodega = turnos;
      printf("%i llego a bodega \n", getpid());
      // terminar ejecucion aqui !! y generar output
    }
  }
}
