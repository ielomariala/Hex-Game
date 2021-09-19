#include "randomista_test.h"
#include "helper_tests_func.h"

void playerRandom__tst() {
  
  size_t num_vertices = 100;
  struct graph_t *g = malloc(sizeof(struct graph_t));
  g->num_vertices = num_vertices;
  g->t = gsl_spmatrix_alloc(num_vertices, num_vertices);
  g->o = gsl_spmatrix_alloc(2, num_vertices);
  gsl_spmatrix_set(g->t, 0, 2, 1);
  gsl_spmatrix_set(g->t, 5, 6, 1);
  gsl_spmatrix_set(g->o, 0, 4, 1);
  gsl_spmatrix_set(g->o, 1, 14, 1);

  struct test_module mod_init = test_module__create("Randomista_initialize");
  initialize_graph(g);
  
  test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 0, 2) == 1);
  test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 5, 6) == 1);
  test_module__add_test(&mod_init, gsl_spmatrix_get(g->o, 0, 4) == 1);
  test_module__add_test(&mod_init, gsl_spmatrix_get(g->o, 1, 14) == 1);
  test_module__add_test(&mod_init, gsl_spmatrix_get(g->o, 0, 14) == 0);
  test_module__add_test(&mod_init, gsl_spmatrix_get(g->o, 0, 13) == 0);
  test_module__conclude(&mod_init);

  struct test_module mod = test_module__create("Randomista_move");
  struct move_t m = {5, 1};
  struct move_t move = play(m);
  test_module__add_test(&mod, move.m < num_vertices);
  move = play(m);
  test_module__add_test(&mod, move.m < num_vertices);
  move = play(m);
  test_module__add_test(&mod, move.m < num_vertices);
  move = play(m);
  test_module__add_test(&mod, move.m < num_vertices);
    
  test_module__conclude(&mod);
  finalize();
}

int main(void){
  playerRandom__tst();
}
