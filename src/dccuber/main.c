#include <stdio.h>
#include <unistd.h>

#include "../file_manager/manager.h"

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

  // ----- FORK

  int id_parent = getpid();
  printf("id parent: %d \n", id_parent);
  pid_t pid;

  /* fork a child process */
  pid = fork();

  if (pid < 0) { 
    /* error occurred */
    fprintf(stderr, "Fork Failed"); 
    return 1;
  }
  else if (pid == 0) { 
    /* child process */
    printf("Child about to do exec\n"); 
    printf("id child: %d \n", getpid());
    execlp("/bin/ls","ls",NULL);
    printf("Child done with exec\n");
  }
  else {
    /* parent process */
    /* parent will wait for the child to complete */
    wait(NULL);
    printf("Child Complete\n");
  }
}

