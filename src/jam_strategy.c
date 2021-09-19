#include <stdlib.h>
#include "graph.h"
#include "compo_connexes.h"
#include "jam_strategy.h"
#include "queue.h"
#include "set/set_dynamic.h"

int get_color_int(struct graph_t *g_t, int i)
{
  return gsl_spmatrix_get(g_t->o, 0, i) ? 0 : gsl_spmatrix_get(g_t->o, 1, i) ? 1 : -1;
}

size_t find_way(struct Queue *queue, int *father, int v)
{
  int x = father[v];

  size_t count = 0;
  while (father[x] != x)
  {
    enqueue(queue, x);
    x = father[x];
    count++;
  }

  return count;
}

void print_array(int len, int *array)
{
  printf("[");
  for (int i = 0; i < len - 1; i++)
  {
    printf("%d, ", array[i]);
  }
  printf("%d", array[len - 1]);
  printf("]\n");
}

size_t parcours_largeur(int vertex_begin, int vertex_end, struct set *cc_list, struct graph_t *M, int color, struct Queue *queue)
{
  // Initialisation
  int n = M->num_vertices;
  const struct set *cc_end = find__cc(cc_list, vertex_end);
  if (cc_end == NULL){
    printf("[ERREUR] CC_END EST NUL !!!\n");
    return 0;
  }
    
  int father[n];
  int T[n];
  for (int i = 0; i < n; i++)
  {
    father[i] = 0;
    T[i] = 0;
  }

  // On met notre élément initial comme déjà visité et comme étant son propre père
  struct Queue *N = empty__queue();

  T[vertex_begin] = 1;
  father[vertex_begin] = vertex_begin;
    
  // On considère les vertex de la composante connexe initiale comme déjà visitée
  const struct set *cc_begin = find__cc(cc_list, vertex_begin);
  if (cc_begin == NULL){
    printf("[ERREUR] CC_BEGIN EST NUL !!!\n");
    return 0;
  }
  size_t l = set__size(cc_begin);
  for (size_t i = 0; i < l; i++)
  {
    T[i] = 1;
  }

  // On ajoute tous les voisins du vertex initial (non visités) dans N
  struct set *set_init = voisins_vertex(vertex_begin, M, color);
  l = set__size(set_init);
  for (size_t i = 0; i < l; i++)
  {
    int x = *((int *)set__get_element(set_init, i));
    if (T[x] == 0)
    {
      enqueue(N, x);
      father[x] = vertex_begin;
      T[x] = 1;
    }
  }
  set__free(set_init);

  // On parcours tout les voisins jusqu'à tomber sur un des sommets de fin
  int flag = 0;
  int v = -1;
  
  while (!queue__is_empty(N) && !flag)
  {
    int u = dequeue(N);

    struct set *V = voisins_vertex(u, M, color);

    for (size_t i = 0; i < set__size(V); ++i)
    {
      v = *((int *)set__get_element(V, i));
      
      // Si notre élément est dans la composante connexe finale
      if (set__find(cc_end, &v))
      {
        flag = 1;
        father[v] = u;
        break;
      }
      // Si notre élément est dans une des composantes connexes
      const struct set *set_tmp1 = find__cc(cc_list, v);
      if (set_tmp1 != NULL && T[v] == 0)
      {
        size_t m = set__size(set_tmp1);
        // On fait en sorte que tous les sommets de notre cc soient considérés comme visités.
        for (size_t j = 0; j < m; j++)
        {
          int w = *((int *)set__get_element(set_tmp1, j));
          T[w] = 1;
        }
        // On ajoute tous les voisins de notre cc à N
        struct set *set_tmp2 = voisins(set_tmp1, M, color);
        m = set__size(set_tmp2);
        for (size_t j = 0; j < m; j++)
        {
          int w = *((int *)set__get_element(set_tmp2, j));
	  if (T[w] == 0){
	    T[w] = 1;
	    father[w] = u;
	    enqueue(N, w);
	  }
        }
	set__free(set_tmp2);
      }
      // Si notre sommet est un sommet libre
      else if (T[v] == 0)
      {
	T[v] = 1;
	father[v] = u;
	enqueue(N, v);
      }
    }
    set__free(V);
  }
  
  size_t nb = find_way(queue, father, v);
  
  queue__free(N);
  
  return nb;

}
