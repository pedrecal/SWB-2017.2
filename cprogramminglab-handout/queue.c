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

#include <stdlib.h>
#include <stdio.h>

#include "harness.h"
#include "queue.h"

/*
  Cria uma fila vazia.
  Retorna NULL se o espaço na puder ser alocado.
*/
queue_t *q_new()
{
  queue_t *q = malloc(sizeof(queue_t));
  if (q == NULL)
    return NULL;
  /* E se malloc retornar NULL? */
  q->head = q->tail = NULL;
  q->count = 0;
  return q;
}

/*  Libera todo o espaço utilizado pela fila. */
void q_free(queue_t *q)
{
  if(q == NULL)
    return;
  list_ele_t *aux = q->head;
  /* Como liberar os elementos da lista? */
  /* Libera a estrutura da fila */
  while (aux != NULL)
  {
    aux = q->head->next;
    free(q->head);
    q->head = aux;
    q->count--;
  }
  free(q);
}

/*
  Tenta inserir o elemento na cabeça da fila.
  Retorna true se houve sucesso
  Retorna false se q é NULL ou não foi possível alocar espaço
*/
bool q_insert_head(queue_t *q, int v)
{
  list_ele_t *newh;
  if (q == NULL)
    return false;
  /* O que você deverá fazer se q é NULL ? */
  newh = malloc(sizeof(list_ele_t));
  if (newh == NULL)
    return false;
  newh->value = v;
  newh->next = q->head;
  q->head = newh;
  if(q->tail == NULL)
    q->tail = q->head;
  q->count++;
  return true;
}

/*
  Tenta inserir o elemento na calda da fila.
  Retorna true se houve sucesso
  Retorna false se q é NULL ou não foi possível alocar espaço
*/
bool q_insert_tail(queue_t *q, int v)
{
  /* Você precisa escrever o código completo para esta função */
  /* lembre-se: Você deverá operar no tempo de O(1) */
  list_ele_t *newt;
  if (q == NULL)
    return false;
  newt = malloc(sizeof(list_ele_t));
  if (newt == NULL)
    return false;
  newt->value = v;
  if(q->tail == NULL)
    q->tail = newt;
  q->tail->next = newt;
  q->tail = newt;
  newt->next = NULL;
  if(q->head == NULL)
    q->head = q->tail;
  q->count++;
  return true;
}

/*
  Tenta remover o elemento na cabeça da fila.
  Retorna true se houve sucesso
  Retorna false se q é NULL ou vazia
  Se vp é não-NULL e o elemento removido, armazena o valor removido em *vp
  Qualquer armazenamento não utilizado deve ser liberado
*/
bool q_remove_head(queue_t *q, int *vp)
{
  /* Você precisa consertar este código. */
  list_ele_t *aux;

  if (q == NULL || q->head == NULL)
    return false;

  if (vp != NULL)
    *vp = q->head->value;

  aux = q->head;
  q->head = q->head->next;
  free(aux);
  q->count--;
  return true;
}

/*
  Retorna o número de elementos na fila.
  Retorna 0 se q é NULL ou vazia
*/
int q_size(queue_t *q)
{
  /* Você precisa escrever o código completo para esta função */
  /* lembre-se: Você deverá operar no tempo de O(1) */
  if (q == NULL || q->head == NULL)
    return 0;
  return q->count;
}

/*
  Inverte os elementos na fila.

  Sua implementação não dever alocar ou liberar quaisquer elementos (e.g., pela
  chamada de q_insert_head ou q_remove_head). Ao invés disso, ela deverá
  modificar os ponteiros na estrutura de dados existente.
*/
void q_reverse(queue_t *q)
{
  if(q == NULL || q->head == q->tail || q->head == NULL)
    return;
    
  list_ele_t *aux1 = q->head;
  list_ele_t *aux2 = aux1->next;
  list_ele_t *aux3 = aux2->next;

  aux1->next = NULL; //FINAL DA FILA NAO MUDA, E SEMPRE NULL
  aux2->next = aux1;
  aux1 = aux2;
  aux2 = aux3;

  while(aux3 != NULL)
  {
    aux3 = aux3->next;
    aux2->next = aux1;
    aux1 = aux2;
    aux2 = aux3;
  }

  q->tail = q->head;
  q->head = aux1;
  return;
}