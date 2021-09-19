#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_spmatrix.h>
#include <assert.h>

#include "graph_struct.h"

// pour améliorer la complexité:
// -> Les matrices de graph sont symétriques, on peut donc n'effectuer les opérations lors de la création du graph que sur une moitié de celui-ci
// -> Faire l'attribution des couleurs en même temps que la matrice d'adjacence

struct graph_t* graph__empty (int vertices){

  struct graph_t* g = malloc(sizeof(*g));
  g -> num_vertices = vertices;
  g -> t = gsl_spmatrix_alloc(vertices, vertices);
  g -> o = gsl_spmatrix_alloc(2, vertices);
  
  return g;
}

struct graph_t* graph_copy(struct graph_t * g){
  struct graph_t *copy = malloc(sizeof(struct graph_t));
  copy -> num_vertices = g -> num_vertices;
  copy -> o = gsl_spmatrix_alloc(2, g->num_vertices);
  gsl_spmatrix_memcpy(copy -> o, g -> o);
  copy -> t = gsl_spmatrix_alloc(g -> num_vertices, g -> num_vertices); 
  gsl_spmatrix_memcpy(copy -> t, g -> t);
  return copy;
}

struct graph_t* make_graph(int m, char c){
  struct graph_t* g;
  
  switch (c){
  case 'c' :
    g = square_graph__empty(m);
    return g;
  case 't':
    g = triangular_graph__empty(m);
    return g;
  case 'h':
    g = hexa_graph__empty(m);
    return g;
  default:
    return NULL;
  }
}

struct graph_t* square_graph__empty(int m){
  int vertices = (m+1) * (m+1);
  struct graph_t* g = graph__empty (vertices);
  
  int line_length = m+1;
  
  for (int i = 0; i < vertices; i++){
    int index = i%line_length;
    
    // Adds right links
    if (index+1 < line_length)
      gsl_spmatrix_set(g -> t, i, i+1, 1);
    // Adds left links
    if (index-1 >= 0)
      gsl_spmatrix_set(g -> t, i, i-1, 1);
    // Adds down links
    if (i+line_length < vertices)
      gsl_spmatrix_set(g -> t, i, i+line_length, 1);
    // Adds up links
    if (i-line_length >= 0)
      gsl_spmatrix_set(g -> t, i, i-line_length, 1);
  }

  // Ownership of initial boxes
  for (int i = 0; i < line_length-1; i++){
    gsl_spmatrix_set(g -> o, 0, i, 1);
    gsl_spmatrix_set(g -> o, 0, vertices-1-i, 1);
    gsl_spmatrix_set(g -> o, 1, (i+1)*line_length, 1);
    gsl_spmatrix_set(g -> o, 1, vertices-1-(i+1)*line_length, 1);
  }
  
  return g;
}


struct graph_t* triangular_graph__empty(int m){
  int num_vertices = 6 * m * m;
  struct graph_t* g = graph__empty(num_vertices);
  int first_vertice_row=0;
  int row = 0;
  while(row < m){
    //Edges in a row
    for(int vertice = first_vertice_row; vertice < first_vertice_row + m + row; vertice++){
      gsl_spmatrix_set(g -> t, vertice, vertice + m + row, 1);
      gsl_spmatrix_set(g -> t, vertice + m + row, vertice, 1);
      gsl_spmatrix_set(g -> t, vertice, vertice + m + row + 1, 1);
      gsl_spmatrix_set(g -> t, vertice + m + row + 1, vertice, 1);
      
      gsl_spmatrix_set(g -> t, num_vertices - 1 - (vertice + m + row), num_vertices - 1 - vertice, 1);
      gsl_spmatrix_set(g -> t, num_vertices - 1 - vertice, num_vertices - 1 - (vertice + m + row ), 1);
      gsl_spmatrix_set(g -> t, num_vertices - 1 - (vertice + m + row + 1), num_vertices - 1 - vertice, 1);
      gsl_spmatrix_set(g -> t, num_vertices - 1 - vertice, num_vertices-1 - (vertice + m + row + 1), 1);
    }
    //Edges between 2 rows
    for(int vertice = first_vertice_row + m + row; vertice <= first_vertice_row + 2*(m + row); vertice++){
      gsl_spmatrix_set(g -> t, vertice, vertice + m + row + 1, 1);
      gsl_spmatrix_set(g -> t, vertice + m + row + 1, vertice, 1);
      
      gsl_spmatrix_set(g -> t, num_vertices - 1 - vertice, num_vertices - 1 - (vertice + m + row + 1), 1);
      gsl_spmatrix_set(g -> t, num_vertices - 1 - (vertice + m + row + 1), num_vertices - 1 - vertice, 1);
    }
    row++;
    first_vertice_row += 2*m + row*(row+1)/2;
  }
  //black starting tiles
  for(int vertice = 0; vertice < m; vertice ++){
    gsl_spmatrix_set(g -> o, BLACK, vertice, 1);
    gsl_spmatrix_set(g -> o, BLACK, vertice + m + 1, 1);
    
    gsl_spmatrix_set(g -> o, BLACK, num_vertices - 1 - vertice, 1);
    gsl_spmatrix_set(g -> o, BLACK, num_vertices - 1 - (vertice + m + 1), 1);
  }
  for(int row = 1; row < m/2 ; row++){
    gsl_spmatrix_set(g -> o, BLACK, m-1 + 2*row*m + row*(row+1), 1);
    gsl_spmatrix_set(g -> o, BLACK, m-1 + 2*row*m + row*(row+1) + m + row + 1, 1);
    
    gsl_spmatrix_set(g -> o, BLACK, num_vertices - 1 - (m-1 + 2*row*m + row*(row+1)), 1);
    gsl_spmatrix_set(g -> o, BLACK, num_vertices - 1 - (m-1 + 2*row*m + row*(row+1) + m + row + 1), 1);
  }
  //white starting tiles
  for(int row = 0; row < m; row++){
    gsl_spmatrix_set(g -> o, WHITE, m + 2*row*m + row*(row+1), 1);
    gsl_spmatrix_set(g -> o, WHITE, m + 2*row*m + row*(row+1) + m + row + 1, 1);

    gsl_spmatrix_set(g -> o, WHITE, num_vertices - 1 - (m + 2*row*m + row*(row+1)), 1);
    gsl_spmatrix_set(g -> o, WHITE, num_vertices - 1 - (m + 2*row*m + row*(row+1) + m + row + 1), 1);
  }
  for(int row = m/2+1; row < m; row++){
    gsl_spmatrix_set(g -> o, WHITE, m-1 + 2*row*m + row*(row+1), 1);
    gsl_spmatrix_set(g -> o, WHITE, m-1 + 2*row*m + row*(row+1) - (m + row), 1);
    
    gsl_spmatrix_set(g -> o, WHITE, num_vertices - 1 - (m-1 + 2*row*m + row*(row+1)), 1);
    gsl_spmatrix_set(g -> o, WHITE, num_vertices - 1 - (m-1 + 2*row*m + row*(row+1) - (m + row)),1);
  }
  return g;
}


struct graph_t* hexa_graph__empty(int m){
  int vertices = (m+1) * (m+1);
  struct graph_t* g = graph__empty (vertices);

  int line_length = m+1;
  
  for (int i = 0; i < vertices; i++){
    int index = i%line_length;

    // Adds right links
    if (index+1 < line_length)
      gsl_spmatrix_set(g -> t, i, i+1, 1);
    // Adds left links
    if (index-1 >= 0)
      gsl_spmatrix_set(g -> t, i, i-1, 1);
    // Adds down links
    if (i/line_length < line_length-1 && index > 0)
      gsl_spmatrix_set(g -> t, i, i+line_length - 1, 1);  
    if (i+line_length < vertices && i/line_length < line_length-1)
      gsl_spmatrix_set(g -> t, i, i+line_length, 1);
    // Adds up links
    if (i/m > 0 && index < line_length-1)
      gsl_spmatrix_set(g -> t, i, i-line_length + 1, 1);  
    if (i-line_length >= 0 && i/line_length > 0)
      gsl_spmatrix_set(g -> t, i, i-line_length, 1);
    
    // Ownership of initial boxes
    for (int i = 0; i < line_length-1; i++){
      gsl_spmatrix_set(g -> o, 0, i, 1);
      gsl_spmatrix_set(g -> o, 0, vertices-1-i, 1);
      gsl_spmatrix_set(g -> o, 1, (i+1)*line_length, 1); // Pour m = 3, l_l = 4 : 4, 8, 12 
      gsl_spmatrix_set(g -> o, 1, vertices-1-(i+1)*line_length, 1); // 
    }
  }
  return g;
}

void add__move_to_graph (struct graph_t* g, int vertice, int player){
  assert (g != NULL);
  
  gsl_spmatrix_set(g -> o, player, vertice, 1);
}

int get__box_owner (struct graph_t* g, int vertice){
  for (int p = 0; p < 2; p++){
    if (gsl_spmatrix_get(g -> o, p, vertice) == 1)
      return p;
  }
  return EMPTY_BOX;
}

void graph__free (struct graph_t* g){
  assert (g != NULL);
  
  gsl_spmatrix_free(g -> o);
  gsl_spmatrix_free(g -> t);
  
  free(g);
}

void print_graph (struct graph_t* g, char c){
  switch (c){
  case 'c' :
    square_graph__print (g);
    break;
  case 't':
    triangular_graph__print (g);
    break;
  case 'h':
    hexa_graph__print (g);
    break;
  default:
    printf("Don't find this type of graph\n");
    break;
  }
}

void square_graph__print (struct graph_t* g){
  assert (g != NULL);
  
  int n = sqrt(g -> num_vertices);

  for (int i = 0; i < n; i++){
    printf("\t");
    for (int j = 0; j < n; j++){
      int box_owner = get__box_owner (g, i*n + j); 
      if (box_owner == 0)
	printf("\033[34;01m■ \033[00m");
      else if (box_owner == 1)
	printf("\033[31;01m■ \033[00m");
      else
	printf("■ ");
    }
    printf("\n");
  }
}

void triangular_graph__print (struct graph_t* g){(void) g;}

void hexa_graph__print (struct graph_t* g){
    assert (g != NULL);
  
  int n = sqrt(g -> num_vertices);
  char space[1000] = "";
    
  for (int i = 0; i < n; i++){
    printf("\t%s", space);

    for (int space = 0; space < i; space++){
      printf(" ");
    }
    for (int j = 0; j < n; j++){
      int box_owner = get__box_owner (g, i*n + j); 
      if (box_owner == 0)
	printf("\033[34;01m⬢ \033[00m");
      else if (box_owner == 1)
	printf("\033[31;01m⬢ \033[00m");
      else
	printf("⬢ ");
    }
    printf("\n");
  }
}
