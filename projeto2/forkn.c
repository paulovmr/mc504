/* 
 * Nova chamada de sistema forkn.
 * Equipe: Paulo Vitor Martins do Rego - RA 118343
 *         Guilherme Henrique Nunes    - RA 117108
 *         Thiago de Oliveira Pires    - RA 123153
 */ 
#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>

asmlinkage long sys_forkn(int n, long* ids) {
  int i, id, createdProcesses;
  
  createdProcesses = 0;
  
  for (i = 0; i < n; i++) {
    ids[i] = sys_fork();
    
    if (id != -1) createdProcesses++;
  }
  
  return createdProcesses;
}
