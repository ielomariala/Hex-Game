#include "projet_jam_test.h"
#include "helper_tests_func.h"
#include "compo_connexes.h"
#include "../src/graph_struct.h"
#include "../src/projet_jam.h"


void projet_jam__tst() {
  size_t num_vertices = 10;
  struct graph_t * g = hexa_graph__empty(num_vertices);
  int opponent = WHITE;

  struct graph_t * copy_g = graph_copy(g);
  
  initialize_graph(copy_g);
  
  struct test_module mod = test_module__create("projet_Jam_functionnal tests");
    
  struct move_t opening = propose_opening();
  test_module__add_test(&mod, get_color_size_t(g, opening.m) == -1);

  add__move_to_graph(g, opening.m, BLACK);
    
  initialize_color(BLACK);
    
  //hexa_graph__print(g);
  
  for(size_t i = 0; i < 8; ++i) {
    struct move_t oppenent_move = get_random_move(opponent);
    add__move_to_graph(g, oppenent_move.m, oppenent_move.c);
    
    struct move_t test_move = play(oppenent_move);
    test_module__add_test(&mod, get_color_size_t(g, test_move.m) == -1);

    add__move_to_graph(g, test_move.m, test_move.c);
    // hexa_graph__print(g);
  }
  
  test_module__conclude(&mod);

  finalize();
  graph__free(g);
  
}

int main(void){
  projet_jam__tst();
}
