#include "graph_struct_test.h"
#include "is_winning_test.h"
#include "set_void_test.h"
#include "graph_analysis_test.h"
#include "compo_connexes_test.h"
#include "jam_strategy_test.h"

int main(void){  
  graph__empty_tst();
  square_graph__empty_tst();
  set_void__tst();
  triangular_graph__empty_tst();
  hexa_graph__empty_tst();
  add__move_to_graph_tst();
  get__box_owner_tst();
  test_win_square();
  test_win_hexa();
  graph_analysis__tst();
  compo_connexes__tst();
  jam_strategy_tst();
  return 0;   
}
