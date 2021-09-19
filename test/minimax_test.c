#include <dlfcn.h>

#include "minimax_test.h"
#include "helper_tests_func.h"
#include "../src/graph_struct.h"

void minimax__tst(void) {
    size_t m = 10;
    struct graph_t * g = hexa_graph__empty(m);
    size_t num_vertices = g->num_vertices;

    struct test_module mod_init = test_module__create("minimax_initialize");
    initialize_graph(g);
    
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 0, 1) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 0, 11) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 1, 0) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 1, 2) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 1, 12) == 1);
    test_module__add_test(&mod_init, gsl_spmatrix_get(g->t, 1, 11) == 1);
    test_module__conclude(&mod_init);

    struct test_module mod = test_module__create("minimax_move");
    struct move_t move = propose_opening();
    test_module__add_test(&mod,move.m < num_vertices);
    test_module__add_test(&mod,move.m == 60);
    
    // Two bridging in direction 1
    struct move_t opponent_move = {12,WHITE};
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == 81);

    // Two bridging in direction 0
    opponent_move.m = 13;
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == 39);

    // Two bridging in direction 1
    opponent_move.m = 14;
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == 102);

    // Two Bridging in direction 0
    opponent_move.m = 40;
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == 18);

    // Linking Two bridge
    opponent_move.m = 42;
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == 101);

    // Linking Two Bridge 
    opponent_move.m = 50;
    move = play(opponent_move);
    test_module__add_test(&mod, move.m < num_vertices);
    test_module__add_test(&mod,move.m == 49);

    
  test_module__conclude(&mod);

  finalize();
}

int main(void){
    minimax__tst();
    return 0;
}