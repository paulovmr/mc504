/* 
 * Teste da nova chamada de sistema forkn.
 * Equipe: Paulo Vitor Martins do Rego - RA 118343
 *         Guilherme Henrique Nunes    - RA 117108
 *         Thiago de Oliveira Pires    - RA 123153
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int idFather = 0, n = 10, i;
  long *ids, r;
  
  // Guarda o id do processo pai.
  idFather = syscall(20);
  
  // Vetor que armazenará o id dos processos filhos gerados.
  ids = (long*) malloc(n * sizeof(long));
  
  // Chamada ao forkn.
  r = syscall(351, n, ids);
  
  printf("Id father: %d. Meu PID: %d. Retorno de forkn: %ld.", idFather, getpid(), r);
  
  // Apenas o pai printa os filhos.
  if (idFather == getpid()) {
      printf(" Ids filhos:");
      for (i = 0; i < n; i++) {
        printf(" %ld |", ids[i]);
      }
  }
  
  printf("\\n");
  
  free(ids);
  
  return 0;
}
