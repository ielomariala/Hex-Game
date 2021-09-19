#ifndef GRAPH_STRUCT_H
#define GRAPH_STRUC_H

#define EMPTY_BOX -1

#include "move.h"
#include "graph.h"

// Returns an empty graph
struct graph_t* graph__empty (int vertices);

// Returns a allocated copy of the graph
struct graph_t* graph_copy(struct graph_t * g);

// Return a graph with the c shape and m*m vertices
struct graph_t* make_graph(int m, char c);
void print_graph(struct graph_t* g, char c);

// Return a square graph with vertices vertices
struct graph_t* square_graph__empty(int m);
// Displays a square graph in the terminal
void square_graph__print (struct graph_t* g);

// Return a triangular graph with vertices vertices
struct graph_t* triangular_graph__empty(int m);
// Displays a triangular graph in the terminal
void triangular_graph__print (struct graph_t* g);

// Return a hexagonal graph with vertices vertices
struct graph_t* hexa_graph__empty(int m);
// Displays a hexagonal graph in the terminal
void hexa_graph__print (struct graph_t* g);

// Adds a move to the matrix (don't check if the box is free)
void add__move_to_graph (struct graph_t* g, int vertice, int player);
// Returns who own the box (player id if one owns, -1 otherwise)
int get__box_owner (struct graph_t* g, int vertice);
// Free the graph
void graph__free (struct graph_t* g); 

#endif
