#include "graph_struct_test.h"

void print_spmatrix(gsl_spmatrix* g){
  size_t n = g -> size1;
  size_t m = g -> size2;

  for (size_t i = 0; i < n; i++){
    for (size_t j = 0; j < m; j++){
      printf("%d ", (int) gsl_spmatrix_get(g, i, j));
    }
    printf("\n");
  }
}

void graph__empty_tst (){
  // Structural tests
  int err = 0;
  int tst_done = 0;
  
  size_t vertices = 25;
  struct graph_t* g = graph__empty(vertices);
  
  if (!err){
    tst_done++;
    if (g -> num_vertices != vertices){
      print_int_err(vertices, g -> num_vertices);
      err = 1;
    }
  }

  if (!err){
    tst_done++;
    for (size_t i = 0; i < vertices; i++){
      for (size_t j = 0; j < vertices; j++){
	if (!err && gsl_spmatrix_get(g -> t, i, j) != 0){
	  print_spmatrix(g -> t);
	  err = 1;
	}  
      }
    }
  }

  if (!err){
    tst_done++;
    for (size_t i = 0; i < 2; i++){
      for (size_t j = 0; j < vertices; j++){
	if (!err && gsl_spmatrix_get(g -> o, i, j) != 0){
	  print_spmatrix(g -> o);
	  err = 1;
	}  
      }
    }
  }
  
  print_tst_result("graph__empty structural tests", !err, tst_done, 3);

  graph__free(g);
}

void square_graph__empty_tst (){
  // Structural tests
  int err = 0;
  int tst_done = 0;
  
  int m = 2;
  size_t vertices = (m+1) * (m+1);
  struct graph_t* g = square_graph__empty(m);
  
  int square_graph_t[9][9] = {{0, 1, 0, 1, 0, 0, 0, 0, 0},
			    {1, 0, 1, 0, 1, 0, 0, 0, 0},
			    {0, 1, 0, 0, 0, 1, 0, 0, 0},
			    {1, 0, 0, 0, 1, 0, 1, 0, 0},
			    {0, 1, 0, 1, 0, 1, 0, 1, 0},
			    {0, 0, 1, 0, 1, 0, 0, 0, 1},
			    {0, 0, 0, 1, 0, 0, 0, 1, 0},
			    {0, 0, 0, 0, 1, 0, 1, 0, 1},
			    {0, 0, 0, 0, 0, 1, 0, 1, 0}};

  int square_graph_o[2][9] = {{1, 1, 0, 0, 0, 0, 0, 1, 1},
			      {0, 0, 1, 1, 0, 1, 1, 0, 0}};
  
  if (!err){
    tst_done++;
    if (g -> num_vertices != vertices){
      print_int_err(vertices, g -> num_vertices);
      err = 1;
    }
  }

  if (!err){
    tst_done++;
    for (size_t i = 0; i < vertices; i++){
      for (size_t j = 0; j < vertices; j++){
	if (!err && gsl_spmatrix_get(g -> t, i, j) != square_graph_t[i][j]){
	  print_spmatrix(g -> t);
	  err = 1;
	}
      }
    }    
  }

  if (!err){
    tst_done++;
    for (size_t i = 0; i < 2; i++){
      for (size_t j = 0; j < vertices; j++){
	if (!err && gsl_spmatrix_get(g -> o, i, j) != square_graph_o[i][j]){
	  print_spmatrix(g -> o);
	  err = 1;
	}  
      }
    }
  }
  
  print_tst_result("square_graph__empty structural tests", !err, tst_done, 3);

  graph__free(g);
}

void triangular_graph__empty_tst(){
  // Structural tests
  int err = 0;
  int tst_done = 0;
  
  int m = 2;
  size_t vertices = 6*m*m;
  struct graph_t* g = triangular_graph__empty(m);
  
  int triangular_graph_t[24][24] =
    
    {{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
     {0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0}};

  int triangular_graph_o[2][24] = {{1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1},
				   {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}};
  
  if (!err){
    tst_done++;
    if (g -> num_vertices != vertices){
      print_int_err(vertices, g -> num_vertices);
      err = 1;
    }
  }

  if (!err){
    tst_done++;
    for (size_t i = 0; i < vertices; i++){
      for (size_t j = 0; j < vertices; j++){
	if (!err && gsl_spmatrix_get(g -> t, i, j) != triangular_graph_t[i][j]){
	  print_spmatrix(g -> t);
	  err = 1;
	}
      }
    }    
  }

  if (!err){
    tst_done++;
    for (size_t i = 0; i < 2; i++){
      for (size_t j = 0; j < vertices; j++){
	if (!err && gsl_spmatrix_get(g -> o, i, j) != triangular_graph_o[i][j]){
	  print_spmatrix(g -> o);
	  err = 1;
	}  
      }
    }
  }
  
  print_tst_result("triangular_graph__empty structural tests", !err, tst_done, 3);

  graph__free(g);
}

void hexa_graph__empty_tst(){
  // Structural tests
  int err = 0;
  int tst_done = 0;

  int m = 2;
  size_t vertices = (m+1) * (m+1);
  struct graph_t* g = hexa_graph__empty(m);
  
  int hexa_graph_t[9][9] =
    {{0, 1, 0, 1, 0, 0, 0, 0, 0},
     {1, 0, 1, 1, 1, 0, 0, 0, 0},
     {0, 1, 0, 0, 1, 1, 0, 0, 0},
     {1, 1, 0, 0, 1, 0, 1, 0, 0},
     {0, 1, 1, 1, 0, 1, 1, 1, 0},
     {0, 0, 1, 0, 1, 0, 0, 1, 1},
     {0, 0, 0, 1, 1, 0, 0, 1, 0},
     {0, 0, 0, 0, 1, 1, 1, 0, 1},
     {0, 0, 0, 0, 0, 1, 0, 1, 0}};
  
  int hexa_graph_o[2][9] = {{1, 1, 0, 0, 0, 0, 0, 1, 1},
		      {0, 0, 1, 1, 0, 1, 1, 0, 0}};
  
  if (!err){
    tst_done++;
    if (g -> num_vertices != vertices){
      print_int_err(vertices, g -> num_vertices);
      err = 1;
    }
  }

  if (!err){
    tst_done++;
    for (size_t i = 0; i < vertices; i++){
      for (size_t j = 0; j < vertices; j++){
	if (!err && gsl_spmatrix_get(g -> t, i, j) != hexa_graph_t[i][j]){
	  print_spmatrix(g -> t);
	  err = 1;
	}
      }
    }    
  }

  if (!err){
    tst_done++;
    for (size_t i = 0; i < 2; i++){
      for (size_t j = 0; j < vertices; j++){
	if (!err && gsl_spmatrix_get(g -> o, i, j) != hexa_graph_o[i][j]){
	  print_spmatrix(g -> o);
	  err = 1;
	}  
      }
    }
  }
  
  print_tst_result("hexa_graph__empty structural tests", !err, tst_done, 3);

  graph__free(g);
  
}

void add__move_to_graph_tst (){
  // Structural tests
  int err = 0;
  int tst_done = 0;

  size_t vertices = 25;
  struct graph_t* g = graph__empty(vertices);
  
  if (!err){
    tst_done++;
    add__move_to_graph(g, 5, 1);
    
    if (gsl_spmatrix_get(g -> o, 1, 5) != 1){
      print_int_err(1, gsl_spmatrix_get(g -> o, 1, 5));
      err = 1;
    }
  }

  print_tst_result("add__move to graph structural tests", !err, tst_done, 1);

  graph__free(g);
}

void get__box_owner_tst (){
  // Structural tests
  int err = 0;
  int tst_done = 0;

  int vertices = 25;
  struct graph_t* g = graph__empty(vertices);
  gsl_spmatrix_set (g -> o, 0, 1, 1);
  gsl_spmatrix_set (g -> o, 1, 2, 1);
  
  if (!err){
    tst_done++;
    int owner = get__box_owner (g, 1);
    if (owner != 0){
      print_int_err(0, owner);
      err = 1;
    }
  }

  if (!err){
    tst_done++;
    int owner = get__box_owner (g, 2);
    if (owner != 1){
      print_int_err(1, owner);
      err = 1;
    }
  }

  if (!err){
    tst_done++;
    int owner = get__box_owner (g, 0);
    if (owner != EMPTY_BOX){
      print_int_err(EMPTY_BOX, owner);
      err = 1;
    }
  }
  
  print_tst_result("get__box_owner structural tests", !err, tst_done, 3);

  graph__free(g);
}
