/* 
 * Teste da nova chamada de sistema mycall.
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int r = syscall(352); 
  printf("Retorno da chamada de sistema: %d.\\n", r);
  return r;
}
