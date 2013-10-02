Projeto 1 - Problema da travessia do rio
=====

Problema
=====
O problema clássico consiste na travessia de um rio, dentro de um barco com capacidade para quatro passageiros, por dois tipos de pessoas: funcionários da Microsoft e hackers do Linux. As regras da travessia são bem simples: ou somente um tipo de pessoa está dentro do barco, ou o número de pessoas de cada tipo é o mesmo (dois de cada). Além disso, o barco só deve executar a travessia quando estiver totalmente cheio. Quando o último dos quatro passageiros entrar no barco, um deles deve pegar os remos (capitão) e remar até o outro lado do rio. Não é necessário se preocupar com a volta do barco.

Problema generalizado (parametrização)
=====
Para implementação da solução, foram parametrizados o número de barcos e o número de pessoas por barco (deve ser par para a regra continuar fazendo sentido). Quando o usuário for executar o programa, deve observar o tamanho de sua tela para que a animação não exceda-o, causando problemas na visualização.

Solução
=====
 * Gerenciamento de threads
A entrada e a espera de pessoas no barco é gerenciada por uma barreira, que aguarda o barco atingir sua lotação máxima para iniciar a travessia. A última thread (pessoa) que entra no barco se torna capitão e rema até a outra margem. Novas pessoas (threads) são criadas infinitamente e aguardam na margem até ter uma oportunidade de embarque. O acesso às regiões críticas (posicionamento do cursor, desenho na tela, alteração de variáveis compartilhadas) são gerenciados por mutexes que limitam a uma thread acessando-os simultaneamente.
 
 * Animação
A animação foi desenvolvida usando apenas caracteres ASCII e alterações de cores do terminal Unix. Cada thread (exceto o main) é representada por uma pessoa. Quando criada, a pessoa é desenhada na grama e aguarda para embarcar. Quando embarca, ela fica sentada no barco aguardando a última pessoa entrar, que por sua vez assume os remos e leva o barco até o outro lado do rio. Nesse ponto, as pessoas descem do barco, sendo desenhadas na margem.
  
Instruções de Uso
=====
 1. Compilação:
  - `make all`
 2. Execução:
  - `./main // Executa com um barco e quatro pessoas por barco`
  - `./main <B> // Executa com <B> barcos e quatro pessoas por barco`
  - `./main <B> <P> // Executa com <B> barcos e <P> pessoas por barco`
