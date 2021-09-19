#include <dlfcn.h>

#include "adjacenta_test.h"
#include "helper_tests_func.h"
#include "../src/graph_struct.h"

void adjacenta__tst() {

    size_t m = 10;
    struct graph_t * g = hexa_graph__empty(m);
    size_t num_vertices = g->num_vertices;

    struct test_module mod_init = test_module__create("Adjacenta_initialize");
    initialize_graph(g);
    
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 0, 1) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 0, 11) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 1, 0) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 1, 2) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 1, 12) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 1, 11) == 1);
    test_module__conclude(&mod_init);

    struct test_module mod = test_module__create("Adjacenta_move");
    struct move_t move = propose_opening();
    test_module__add_test(&mod,move.m < num_vertices);
    test_module__add_test(&mod,move.m == num_vertices/2);

    struct move_t opponent_move = {12,WHITE};
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == num_vertices/2+(m+1)-1);

    opponent_move.m = 13;
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == num_vertices/2-(m+1)+1);

    opponent_move.m = 14;
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == num_vertices/2+2*(m+1)-2);

    // Opennent Blocking him directly
    opponent_move.m = 40;
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == num_vertices/2-2*(m+1)+1);

    // Oppenins choose a very dangerous move
    opponent_move.m = 42;
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == 41);
    
  test_module__conclude(&mod);

  finalize();
}

int main(void){
  adjacenta__tst();
}