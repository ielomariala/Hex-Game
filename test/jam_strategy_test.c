#include <stdlib.h>
#include <stdio.h>

#include "../src/compo_connexes.h"
#include "../src/jam_strategy.h"
#include "helper_tests_func.h"
#include "../src/queue.h"
#include "../src/graph_struct.h"


void find_way_tst(){
  struct test_module mod_func = test_module__create("find_way");
  
  struct Queue *queue = empty__queue();
  int t[] = {3, 1, 1, 5, 0, 2};
  int expected[] = {3, 5, 2};

  size_t count = find_way(queue, t, 0);
  int i = 0;
  while (!queue__is_empty(queue)){
    int x = dequeue(queue);
    test_module__add_test(&mod_func, x == expected[i]);
    i++;
  }
  test_module__add_test(&mod_func, ((int) count) == 3);

  test_module__conclude(&mod_func);
  queue__free(queue);
}

void parcours_largeur_tst(){
  struct test_module mod_func = test_module__create("parcours_largeur");
  
  struct graph_t* g= square_graph__empty(6);
  add__move_to_graph(g, 15, 0);
  add__move_to_graph(g, 16, 0);
  add__move_to_graph(g, 23, 0);
  
  add__move_to_graph(g, 25, 0);
  add__move_to_graph(g, 32, 0);
  
  add__move_to_graph(g, 30, 1);
  
  int v_begin = 1;
  int v_end = 48;
  int color = 0;

  int expected[] = {39, 24, 8};
  
  struct set* cc_list = ignite_last_hope(g, color);
  struct Queue* queue = empty__queue();
  
  size_t n = parcours_largeur(v_begin, v_end, cc_list, g, color, queue);

  test_module__add_test(&mod_func, n == 3);

  int i = 0;
  while (!queue__is_empty(queue)){
    int x = dequeue(queue);
    test_module__add_test(&mod_func, x == expected[i]);
    i++;
  }
  
  set__free(cc_list);
  graph__free(g);
  queue__free(queue);

  struct graph_t* g_h = hexa_graph__empty(6);
  add__move_to_graph(g_h, 18, 0);
  add__move_to_graph(g_h, 24, 0);
  //hexa_graph__print(g_h);

  int v_begin_h = 5;
  int v_end_h = 48;
  int color_h = 0;

  int expected_h[] = {38, 31, 12};
  
  struct set* cc_list_h = ignite_last_hope(g_h, color);
  struct Queue* queue_h = empty__queue();
  
  size_t n_h = parcours_largeur(v_begin_h, v_end_h, cc_list_h, g_h, color_h, queue_h);
  test_module__add_test(&mod_func, n_h == 3);
  
  i = 0;
  while (!queue__is_empty(queue_h)){
    int x = dequeue(queue_h);
    test_module__add_test(&mod_func, x == expected_h[i]);
    i++;
  }

  set__free(cc_list_h);
  graph__free(g_h);
  queue__free(queue_h);

  
  test_module__conclude(&mod_func);
}

void jam_strategy_tst(){
  find_way_tst();
  parcours_largeur_tst();
}
