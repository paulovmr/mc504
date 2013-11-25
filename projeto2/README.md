Projeto 2 - Criação da syscall "forkn"
=====

Descrição
=====
O objetivo do código é a criação de uma syscall (forkn), que cria n processos com a chamada da syscall "fork". Ela possui o protocolo "long sys_forkn(int n, long* ids)", onde:
 * O parâmetro "long n" é a quantidade de processos filhos que se deseja serem criados.
 * O parâmetro "long* ids" é um ponteiro para um vetor com memória já alocada, onde serão colocados os ids dos processos criados.
 * O retorno "long" da função é a quantidade de processos filhos criados para o pai, e 0 para os filhos. Caso ocorra erro em alguma das criações, o valor -1 é colocado no vetor de ids.
 
Também foi criada uma syscall para testes, chamada "mycall". Ela não recebe parâmetros e sempre retorna o valor 4096.

Tutorial
=====
Para montar o ambiente e utilizar as syscalls, os seguintes passos são necessários:
 * Baixar o arquivo http://www.ic.unicamp.br/~islene/2s2013-mc504/system-call/mc504-linux.tar.xz ;
 * Descompactá-lo com o comando "tar xJvf mc504-linux.tar.xz";
 * Substituir o arquivo "linux-3.12/arch/x86/syscalls/syscall_32.tbl" pelo novo arquivo versionado;
 * Substituir o arquivo "linux-3.12/include/linux/syscalls.h" pelo novo arquivo versionado;
 * Substituir o arquivo "linux-3.12/arch/x86/kernel/Makefile" pelo novo arquivo versionado;
 * Incluir o arquivo versionado "forkn.c" no diretório "linux-3.12/arch/x86/kernel/";
 * Incluir o arquivo versionado "mycall.c" no diretório "linux-3.12/arch/x86/kernel/";
 * Rodar o comando "make" no diretório "linux-3.12";
 * Rodar o programa no QEMU, utilizando o comando "qemu-system-x86_64 -hda mc504.img -kernel linux-3.12/arch/i386/boot/bzImage -append "ro root=/dev/hda" -hdb <PROGRAMA>", onde <PROGRAMA> deve ser substituido pelo programa de teste que usa a syscall ("forkn_test" ou "mycall_test", por exemplo);
 * Para logar no sistema, utilizar o usuário "root" com senha "root";
 * Rodar o comando "cat /dev/hdb > teste";
 * Rodar o comando "chmod +x teste";
 * Rodar o comando "./teste".
 
 Pronto!
