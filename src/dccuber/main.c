#include <stdio.h>
#include <unistd.h>

#include "../file_manager/manager.h"

void handle_sigusr1(int sig, siginfo_t *siginf, void *ptr) 
{
  printf ("Signal RECIBIDA EN FABRICA: %d\n", sig);
}

int main(int argc, char const *argv[])
{
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



  printf("%d \n", tiempo_de_creacion);
  printf("%d \n", envios_necesarios);
  printf("%d \n", tiempo1);
  printf("%d \n", tiempo2);
  printf("%d \n", tiempo3);

  input_file_destroy(data_in);


  //connect_sigaction(SIGUSR1, handle_sigusr1);

  // ----- FORK

  int id_parent = getpid();
  printf("id parent: %d \n", id_parent);
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
    //connect_sigaction(SIGUSR1, handle_sigusr1);
    int pid_fabrica = getpid();
    //pid_fabrica = getpid();
    // crear los repartidores aqui
    printf("Child about to do exec REPARTIDORES\n"); 
    printf("id child: %d \n", getpid());

    for (int i = 0; i < envios_necesarios; i++){

    //sleep(tiempo_de_creacion);
    int pid;
    pid = fork();
    if (pid < 0) {
    /* error occurred */
    fprintf(stderr, "Fork Failed"); 
    return 1;
    }

    else if (pid == 0) { 
      /* child process Repartidor */
      char *args[] = {"Iniciando", "repartidor", "i", NULL};
      execv("./repartidor", args);
      printf("Child Complete\n");
    }
  
    else {
      /* parent  process FABRICA */
      wait(NULL);
    }
    }
    // fin for repartidores

    while(true){
      connect_sigaction(SIGUSR1, handle_sigusr1);
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
      /* 
      Semaforo recibe: 
      (1) id de libre eleccion
      (2) delay = tiempo_i: 3 ultimos numeros de la 2da linea del input.txt
      (3) parent = PID fabrica
      */
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
    }
    }
  }
}

