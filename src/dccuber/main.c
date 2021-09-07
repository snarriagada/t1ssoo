#include <stdio.h>
#include <unistd.h>

#include "../file_manager/manager.h"



int main(int argc, char const *argv[])
{
  //int envios_necesarios;
  int pid_semaforo1;
  int pid_semaforo2;
  int pid_semaforo3;

  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());

  char *filename = argv[1];
  InputFile *data_in = read_file(filename);

  printf("Leyendo el archivo %s...\n", filename);
  printf("- Lineas en archivo: %i\n", data_in->len);
  printf("- Contenido del archivo:\n");

  printf("\t- ");
  for (int i = 0; i < 4; i++)
  {
    printf("%s, ", data_in->lines[0][i]);
  }
  printf("\n");

  printf("\t- ");
  for (int i = 0; i < 5; i++)
  {
    printf("%s, ", data_in->lines[1][i]);
  }
  printf("\n");

  printf("Liberando memoria...\n");

  int distancia1 = atoi(data_in->lines[0][0]);
  int distancia2 = atoi(data_in->lines[0][1]);
  int distancia3 = atoi(data_in->lines[0][2]);
  int distancia_bodega = atoi(data_in->lines[0][3]);

  printf("%d", distancia1);
  printf("%d", distancia2);
  printf("%d", distancia3);
  printf("%d", distancia_bodega);

  int tiempo_de_creacion = atoi(data_in->lines[1][0]);
  int envios_necesarios = atoi(data_in->lines[1][1]);
  int tiempo1 = atoi(data_in->lines[1][2]);
  int tiempo2 = atoi(data_in->lines[1][3]);
  int tiempo3 = atoi(data_in->lines[1][4]);

  int repartidores_finalizados = 0;

  int array_repartidores[envios_necesarios];
  for (int i = 0; i < envios_necesarios; i++)
    {
    array_repartidores[i] = 0;
    }  
  

  printf("%d \n", tiempo_de_creacion);
  printf("%d \n", envios_necesarios);
  printf("%d \n", tiempo1);
  printf("%d \n", tiempo2);
  printf("%d \n", tiempo3);

  input_file_destroy(data_in);

  void handle_sigusr1(int sig, siginfo_t *siginf, void *ptr) 
  {
  //printf ("Signal RECIBIDA EN FABRICA: %d\n", sig);
  int number_received = siginf->si_value.sival_int;
  printf ("info RECIBIDA EN FABRICA pid %d: semaforo_id %d\n",getpid(), number_received);

  for (int i = 0; i < envios_necesarios; i++)
    {
      if(array_repartidores[i] == 0){
        break;
      }
      send_signal_with_int(array_repartidores[i], number_received);
      printf("**** Enviando señal a %d \n", array_repartidores[i]);
    }  
  }

  void handle_sigusr2(int sig, siginfo_t *siginf, void *ptr) 
  {
  printf ("Signal RECIBIDA EN FABRICA desde REPARTIDOR: %d\n", sig);
  int number_received = siginf->si_value.sival_int;
  printf ("info RECIBIDA EN FABRICA pid %d: repartidor_id %d\n",getpid(), number_received);
  repartidores_finalizados++;
  if(repartidores_finalizados==envios_necesarios){
    //enviar señal main para terminar todo
    //printf("******TERMINO TODO, MATARE A GETPPID: %i ****\n", getppid());
    //printf("******TERMINO TODO, MATARE A GETPID: %i ****\n", getpid());

    kill(getppid(), SIGINT);
  }
  /*
  for (int i = 0; i < envios_necesarios; i++)
    {
      if(array_repartidores[i] == 0){
        break;
      }
      send_signal_with_int(array_repartidores[i], number_received);
      printf("**** Enviando señal a %d \n", array_repartidores[i]);
    }  
  */
  }

void handle_sigint(int sig)
{
  printf("Gracefully finishing\n");
  kill(pid_semaforo1, SIGABRT);
  kill(pid_semaforo2, SIGABRT);
  kill(pid_semaforo3, SIGABRT);
}

  //connect_sigaction(SIGUSR1, handle_sigusr1);

  // ----- FORK
  signal(SIGINT, handle_sigint);
  int id_parent = getpid();
  printf("id parent PRINCIPAL: %d \n", id_parent);
  pid_t pid_fabrica;

  /* fork a child process */
  pid_fabrica = fork();

  if (pid_fabrica < 0) { 
    /* error occurred */
    fprintf(stderr, "Fork Failed"); 
    return 1;
  }
  else if (pid_fabrica == 0) { 
    /* child process FABRICA */
    
    connect_sigaction(SIGUSR1, handle_sigusr1);
    connect_sigaction(SIGUSR2, handle_sigusr2);

    int pid_fabrica = getpid();
    //pid_fabrica = getpid();
    // crear los repartidores aqui
    printf("Child about to do exec REPARTIDORES\n"); 
    printf("id child FABRICA: %d \n", getpid());

    for (int i = 0; i < envios_necesarios; i++){

    sleep(tiempo_de_creacion);
    int pid;
    pid = fork();
    // array_repartidores[i] = pid;
    if (pid < 0) {
    /* error occurred */
    fprintf(stderr, "Fork Failed"); 
    return 1;
    }

    else if (pid == 0) { 
      /* child process Repartidor */

      char char_distancia1[20];
      sprintf(char_distancia1, "%d", distancia1);
      char char_distancia2[20];
      sprintf(char_distancia2, "%d", distancia2);
      char char_distancia3[20];
      sprintf(char_distancia3, "%d", distancia3);
      char char_distancia_bodega[20];
      sprintf(char_distancia_bodega, "%d", distancia_bodega);
      char char_indice[20];
      sprintf(char_indice, "%d", i);

      char *args[] = {char_distancia1, char_distancia2, char_distancia3, char_distancia_bodega, char_indice, NULL};
      execv("./repartidor", args);
      printf("Child Complete\n");
    }
  
    else {
      /* parent  process FABRICA */
      array_repartidores[i] = pid;
      wait(NULL);
    }
    }
    // fin for repartidores

    while(1){
      pause();
    }


    printf("Child done with exec\n");


  }
  else {
    /* parent process PRINCIPAL */
    /* parent will wait for the child to complete */
    // CREAR SEMAFOROS
    //wait(NULL);

    for (int i = 0; i < 3; i++){
      int pid;
      pid = fork();
      if (pid < 0) {
      /* error occurred */
      fprintf(stderr, "Fork Failed"); 
      return 1;
      }

      else if (pid == 0) { 
        /* child process SEMAFORO */
        if(i==0){
          pid_semaforo1 = pid;
        }else if(i==1){
          pid_semaforo2 = pid;
        }else if(i==2){
          pid_semaforo3 = pid;
        }
        /* 
        Semaforo recibe: 
        (1) id de libre eleccion
        (2) delay = tiempo_i: 3 ultimos numeros de la 2da linea del input.txt
        (3) parent = PID fabrica
        */
        //printf("***EN ITERACION %d DEL FORK SEMAFOROS****\n", i);
        char id[20];
        sprintf(id, "%d", i); 

        char str_tiempo[20];
        if(i==0){
          sprintf(str_tiempo, "%d", tiempo1); 
        }else if(i==1){
          sprintf(str_tiempo, "%d", tiempo2); 
        }else if(i==2){
          sprintf(str_tiempo, "%d", tiempo3);
        }
        char char_pid_fabrica[20];
        sprintf(char_pid_fabrica, "%d", pid_fabrica);
        char *args[] = {id, str_tiempo, char_pid_fabrica, NULL};
        execv("./semaforo", args);
        printf("Child Complete\n");
      }
    
      else {
        /* parent  process PRINCIPAL */
        wait(NULL);
        printf("principal no espera a semaforo");
      }
    }
  }
}

