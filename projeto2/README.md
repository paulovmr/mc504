Projeto 2 - Criação da syscall "forkn"
=====

Descrição
=====
O objetivo do código é a criação de uma syscall (forkn), que cria n processos com a chamada da syscall "fork". Ela possui o protocolo "long sys_forkn(int n, long* ids)", onde:
 * O parâmetro "long n" é a quantidade de processos filhos que se deseja serem criados.
 * O parâmetro "long* ids" é um ponteiro para um vetor com memória já alocada, onde serão colocados os ids dos processos criados.
 * O retorno "long" da função é a quantidade de processos filhos criados para o pai, e 0 para os filhos. Caso ocorra erro em alguma das criações, o valor -1 é colocado no vetor de ids.
