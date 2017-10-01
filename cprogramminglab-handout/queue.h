/*
 * Código para diagnostico de conhecimento básico em C.
 * Desenvolvido para o curso CET-088, CET-082
 * Modificado por Leard Fernandes, 2017
 * Developed by R. E. Bryant, 2017
 */

/*
 * Este programa implementa uma fila que suporta ambas as operações FIFO e LILO.
 *
 * Ele utiliza uma lista unicamente ligada para representar o conjunto de
 * elementos da fila
 */

#include <stdbool.h>

/************** Declaração da estrutura de dados ****************/

/* Elemento da lista ligada (Você não deve modificar isso) */
typedef struct ELE {
    int value;
    struct ELE *next;
} list_ele_t;

/* Estrutura da fila */
typedef struct {
    list_ele_t *head;
    list_ele_t *tail;
    int count;
    /* Lista ligada de elementos */
    /*
      Você precisará adicionar mais campos a esta estrutura
      para efetivamente implementar q_size e q_insert_tail
    */
} queue_t;

/************** Operações sobre a fila ************************/

/*
  Cria uma fila vazia.
  Retorna NULL se o espaço nao puder ser alocado.
*/
queue_t *q_new();

/*
  Libera todo o espaço utilizado pela fila.
  Nenhum efeito se q é NULL
*/
void q_free(queue_t *q);

/*
  Tenta inserir o elemento na cabeça da fila.
  Retorna true se houve sucesso
  Retorna false se q é NULL ou não foi possível alocar espaço
 */
bool q_insert_head(queue_t *q, int v);

/*
  Tenta inserir o elemento na calda da fila.
  Retorna true se houve sucesso
  Retorna false se q é NULL ou não foi possível alocar espaço
 */
bool q_insert_tail(queue_t *q, int v);

/*
  Tenta remover o elemento na cabeça da fila.
  Retorna true se houve sucesso
  Retorna false se q é NULL ou vazia
  Se vp é não-NULL e o elemento removido, armazena o valor removido em *vp
  Qualquer armazenamento não utilizado deve ser liberado
*/
bool q_remove_head(queue_t *q, int *vp);

/*
  Retorna o número de elementos na fila.
  Retorna 0 se q é NULL ou vazia
 */
int q_size(queue_t *q);

/*
  Inverte os elementos na fila.
  Nenhum efeito se q é NULL ou vazia
 */
void q_reverse(queue_t *q);