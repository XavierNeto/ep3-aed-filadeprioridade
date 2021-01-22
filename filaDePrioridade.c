/*********************************************************************/
/**   ACH2023 - Algoritmos e Estruturas de Dados I                  **/
/**   EACH-USP - Segundo Semestre de 2020                           **/
/**   <2020204> - Prof. Luciano Antonio Digiampietri                **/
/**                                                                 **/
/**   EP3 - Fila de Prioridade (utilizando heap)                    **/
/**                                                                 **/
/**   <Augusto Xavier Neto>                   <11367581>            **/
/**                                                                 **/
/*********************************************************************/

#include "filaDePrioridade.h"

PFILA criarFila(int max)
{
  PFILA res = (PFILA)malloc(sizeof(FILADEPRIORIDADE));
  res->maxElementos = max;
  res->arranjo = (PONT *)malloc(sizeof(PONT) * max);
  res->heap = (PONT *)malloc(sizeof(PONT) * max);
  int i;
  for (i = 0; i < max; i++)
  {
    res->arranjo[i] = NULL;
    res->heap[i] = NULL;
  }
  res->elementosNoHeap = 0;
  return res;
}

bool exibirLog(PFILA f)
{
  printf("Log [elementos: %i]\n", f->elementosNoHeap);
  PONT atual;
  int i;
  for (i = 0; i < f->elementosNoHeap; i++)
  {
    atual = f->heap[i];
    printf("[%i;%f;%i] ", atual->id, atual->prioridade, atual->posicao);
  }
  printf("\n\n");
}

// Função para retornar o número de elementos na fila
int tamanho(PFILA f)
{
  int tam = 0;

  // Função muito simples, já que temos esse número guardado em elementosNoHeap
  tam = f->elementosNoHeap;

  return tam;
}

// Função auxiliar para aumentar a prioridade ajustando o Heap
void aumentarHeap(PFILA f, int filho)
{
  PONT temp;

  int no;

  // Conta para descobrir o pai do filho apontado
  no = (filho - 1) / 2;

  // Enquanto a prioridade do filho for maior que a do pai
  while (filho > 0 && f->heap[no]->prioridade < f->heap[filho]->prioridade)
  {
    PONT p = malloc(sizeof(ELEMENTO));

    // A posição deles é trocada
    p->posicao = f->heap[filho]->posicao;
    f->heap[filho]->posicao = f->heap[no]->posicao;
    f->heap[no]->posicao = p->posicao;

    temp = f->heap[filho];
    f->heap[filho] = f->heap[no];
    f->heap[no] = temp;

    // O filho se torna o pai
    filho = no;

    // E calculamos o pai do novo pai
    no = (filho - 1) / 2;
  }
}

// Função auxiliar para reduzir a prioridade ajustando o Heap
void reduzirHeap(PFILA f, int no)
{
  PONT temp;

  int filho;

  // Conta para descobrir o filho do pai apontado
  filho = (2 * no) + 1;

  if (f->elementosNoHeap > 1)
  {
    // Se houver apenas 2 elementos no heap, e a prioridade do pai já for maior ou igual a do filho,
    // a função é interrompida
    if (f->heap[0]->prioridade >= f->heap[1]->prioridade && f->elementosNoHeap == 2)
    {
      return;
    }
    while (filho < f->elementosNoHeap)
    {
      // Checando se o filho tem irmao
      if (filho < f->elementosNoHeap - 1)
        // Caso tenha, verificar qual é o maior
        if (f->heap[filho]->prioridade < f->heap[filho + 1]->prioridade)
        {
          // Se o irmão for maior, então é ele que vamos usar
          filho++;
        }

      PONT p = malloc(sizeof(ELEMENTO));

      // Trocando o pai e o filho de posição
      p->posicao = f->heap[no]->posicao;
      f->heap[no]->posicao = f->heap[filho]->posicao;
      f->heap[filho]->posicao = p->posicao;

      temp = f->heap[no];
      f->heap[no] = f->heap[filho];
      f->heap[filho] = temp;

      // O pai passa a ser o filho
      no = filho;

      // E calculamos o filho do novo filho
      filho = (2 * no) + 1;
    }
  }
}

bool inserirElemento(PFILA f, int id, float prioridade)
{
  bool res = false;
  int i = 0;

  // Validando id
  if (id < 0 || id >= f->maxElementos)
  {
    return res;
  }

  // Verificando se já existe um elemento na fila com mesmo id

  while (i < f->maxElementos && f->elementosNoHeap != 0)
  {
    while (f->arranjo[i] == NULL && i < f->maxElementos - 1)
    {
      i++;
    }
    if (f->arranjo[i] != NULL)
    {
      if (f->arranjo[i]->id == id && f->arranjo[i] != NULL)
      {
        // Caso exista, false é retornado
        return res;
      }
    }
    i++;
  }

  // Criando elemento e acertando os valores de seu campo
  PONT p = malloc(sizeof(ELEMENTO));
  p->id = id;
  p->prioridade = prioridade;

  f->arranjo[id] = p;

  // Caso o Heap esteja vazio, o elemento criado estará na primeira posição
  if (f->elementosNoHeap == 0)
  {
    f->heap[0] = p;
    p->posicao = 0;
  }

  // Caso já exista elementos no heap
  if (f->elementosNoHeap != 0)
  {
    // Será colocado na primeira posição disponível
    f->heap[f->elementosNoHeap] = p;
    p->posicao = f->elementosNoHeap;
  }

  // Nesse momento o elemento novo está no fim do Heap, a função auxiliar aumentarHeap é chamada
  // para reorganizar a arvore no caso de o elemento novo ter prioridade maior que de seus pais
  aumentarHeap(f, f->elementosNoHeap);

  // É somado 1 no número de elementos no Heap
  f->elementosNoHeap++;

  return true;
}

bool aumentarPrioridade(PFILA f, int id, float novaPrioridade)
{
  bool res = false;

  int x, n;
  x = 0;

  // Validando id
  if (id < 0 || id >= f->maxElementos)
  {
    return res;
  }

  // Verificar se id nao existe
  while (x < f->maxElementos)
  {
    while (f->arranjo[x] == NULL && x < f->maxElementos - 1)
    {
      x++;
    }
    if (f->arranjo[x] != NULL)
    {
      if (f->arranjo[x]->id == id && f->arranjo[x] != NULL)
      {
        // Se for encontrado o id procurado, a variável n recebe o valor 1
        n = 1;
      }
    }
    x++;
  }

  // Se n for diferente de 1 significa que o id não foi encontrado, entao false é retornado
  if (n != 1)
  {
    return res;
  }

  // Validando a nova prioridade
  if (f->arranjo[id]->prioridade >= novaPrioridade)
  {
    return res;
  }

  // Acertando o valor da nova prioridade
  f->arranjo[id]->prioridade = novaPrioridade;

  // Chamando a função auxiliar aumentarHeap para reorganizar a árvore
  aumentarHeap(f, f->arranjo[id]->posicao);

  return true;
}

bool reduzirPrioridade(PFILA f, int id, float novaPrioridade)
{
  bool res = false;

  int x, n;
  x = 0;

  // Validando id
  if (id < 0 || id >= f->maxElementos)
  {
    return res;
  }

  // Verificar se id nao existe
  while (x < f->maxElementos)
  {
    while (f->arranjo[x] == NULL && x < f->maxElementos - 1)
    {
      x++;
    }
    if (f->arranjo[x] != NULL)
    {
      if (f->arranjo[x]->id == id && f->arranjo[x] != NULL)
      {
        // Se for encontrado o id procurado, a variável n recebe o valor 1
        n = 1;
      }
    }
    x++;
  }

  // Se n for diferente de 1 significa que o id não foi encontrado, entao false é retornado
  if (n != 1)
  {
    return res;
  }

  // Validando a nova prioridade
  if (f->arranjo[id]->prioridade <= novaPrioridade)
  {
    return res;
  }

  // Acertando o valor da nova prioridade
  f->arranjo[id]->prioridade = novaPrioridade;

  // Chamando a função auxiliar aumentarHeap para reorganizar a árvore
  reduzirHeap(f, f->arranjo[id]->posicao);

  return true;
}

PONT removerElemento(PFILA f)
{
  PONT res = NULL;

  PONT temp;

  // Verificando se a fila está vazia, caso sim, retorna NULL
  if (f->elementosNoHeap == 0)
  {
    return res;
  }

  // Colocando NULL na posição do arranjo referente ao elemento a ser removido
  f->arranjo[f->heap[0]->id] = NULL;

  // Subtraindo um no número de elementos no Heap
  f->elementosNoHeap--;

  // Guardando o endereço do elemento removido em uma variável auxiliar
  temp = f->heap[0];

  // A primeira posição do Heap começa a apontar para quem estava na última posição
  f->heap[0] = f->heap[f->elementosNoHeap];

  // A ultima posição agora apontará para NULL, pois haverá menos um elemento
  f->heap[f->elementosNoHeap] = NULL;

  // Se a fila não estiver vazia
  if (f->heap[0] != NULL)
  {
    // O novo Heap no topo vai receber a mesma posição que o antigo topo (agora removido)
    f->heap[0]->posicao = temp->posicao;
  }

  // Existe a possibilidade de a prioridade do novo topo ser menor que a de seus filhos,
  // então é chamada a função auxiliar reduzirHeap para verificar e reorganizar se necessário
  reduzirHeap(f, 0);

  return temp;
}

bool consultarPrioridade(PFILA f, int id, float *resposta)
{
  bool res = false;

  int x, n;
  x = 0;

  // Validando id
  if (id < 0 || id >= f->maxElementos)
  {
    return res;
  }

  // Verificar se id nao existe
  while (x < f->maxElementos)
  {
    while (f->arranjo[x] == NULL && x < f->maxElementos - 1)
    {
      x++;
    }
    if (f->arranjo[x] != NULL)
    {
      if (f->arranjo[x]->id == id && f->arranjo[x] != NULL)
      {
        // Se for encontrado o id procurado, a variável n recebe o valor 1
        n = 1;
      }
    }
    x++;
  }

  // Se n for diferente de 1 significa que o id não foi encontrado, entao false é retornado
  if (n != 1)
  {
    return res;
  }

  // É colocado no endereço da variável resposta o valor da prioridade do elemento buscado
  *resposta = f->arranjo[id]->prioridade;
  return true;
}
