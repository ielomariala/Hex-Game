#include "is_winning_test.h"

void test_win_square(){
  
  struct graph_t* graph1 = make_graph(5, 'c');

  struct test_module mod = test_module__create("is_winning square graph");

  enum color_t res = is_winning(graph1, 'c');
  test_module__add_test(&mod, res == NO_COLOR);
    
  gsl_spmatrix_set(graph1->o, BLACK, 8, 1);
  gsl_spmatrix_set(graph1->o, BLACK, 14, 1);
  gsl_spmatrix_set(graph1->o, BLACK, 15, 1);
  gsl_spmatrix_set(graph1->o, BLACK, 16, 1);
  gsl_spmatrix_set(graph1->o, BLACK, 21, 1);
  gsl_spmatrix_set(graph1->o, BLACK, 27, 1);

  gsl_spmatrix_set(graph1->o, WHITE, 7, 1);
  gsl_spmatrix_set(graph1->o, WHITE, 9, 1);
  gsl_spmatrix_set(graph1->o, WHITE, 10, 1);
  gsl_spmatrix_set(graph1->o, WHITE, 13, 1);
  gsl_spmatrix_set(graph1->o, WHITE, 19, 1);
  gsl_spmatrix_set(graph1->o, WHITE, 22, 1);
  gsl_spmatrix_set(graph1->o, WHITE, 27, 1);
  
  //print_graph(graph1, 'c');
  
  test_module__add_test(&mod, is_winning(graph1, 'c') == BLACK);
  graph__free(graph1);
  
  struct graph_t* graph2 = make_graph(5, 'c');

  gsl_spmatrix_set(graph2->o, WHITE, 19, 1);
  gsl_spmatrix_set(graph2->o, WHITE, 20, 1);
  gsl_spmatrix_set(graph2->o, WHITE, 14, 1);
  gsl_spmatrix_set(graph2->o, WHITE, 15, 1);
  gsl_spmatrix_set(graph2->o, WHITE, 16, 1);
  gsl_spmatrix_set(graph2->o, WHITE, 9, 1);
  gsl_spmatrix_set(graph2->o, WHITE, 27, 1);

  test_module__add_test(&mod, is_winning(graph2, 'c')==WHITE);

  graph__free(graph2);
  
  test_module__conclude(&mod);  
}

void test_win_hexa(){
  struct graph_t* graph1 = make_graph(4, 'h');
  
  struct test_module mod = test_module__create("is_winning hexagonal graph");

  enum color_t res = is_winning(graph1, 'h');
  test_module__add_test(&mod, res == NO_COLOR);
    
  gsl_spmatrix_set(graph1->o, BLACK, 8, 1);
  gsl_spmatrix_set(graph1->o, BLACK, 12, 1);
  gsl_spmatrix_set(graph1->o, BLACK, 17, 1);
  gsl_spmatrix_set(graph1->o, BLACK, 11, 1);
  
  gsl_spmatrix_set(graph1->o, WHITE, 18, 1);
  gsl_spmatrix_set(graph1->o, WHITE, 13, 1);
  gsl_spmatrix_set(graph1->o, WHITE, 6, 1);
  gsl_spmatrix_set(graph1->o, WHITE, 13, 1);
  
  //print_graph(graph1, 'h');
  
  test_module__add_test(&mod, is_winning(graph1, 'h') == BLACK);
  
  graph__free(graph1);

  struct graph_t* graph2 = make_graph(4, 'h');
  
  gsl_spmatrix_set(graph2->o, WHITE, 16, 1);
  gsl_spmatrix_set(graph2->o, WHITE, 17, 1);
  gsl_spmatrix_set(graph2->o, WHITE, 12, 1);
  gsl_spmatrix_set(graph2->o, WHITE, 8, 1);

  gsl_spmatrix_set(graph2->o, BLACK, 11, 1);
  gsl_spmatrix_set(graph2->o, BLACK, 6, 1);
  gsl_spmatrix_set(graph2->o, BLACK, 7, 1);
  gsl_spmatrix_set(graph2->o, BLACK, 13, 1);
  gsl_spmatrix_set(graph2->o, BLACK, 18, 1);

  //print_graph(graph2, 'h');

  test_module__add_test(&mod, is_winning(graph2, 'h') == WHITE);

  graph__free(graph2);
  
  test_module__conclude(&mod);  
}
