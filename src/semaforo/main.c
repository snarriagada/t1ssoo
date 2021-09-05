#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  printf("%s %s %s\n", argv[0], argv[1], argv[2]);
  printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
}
