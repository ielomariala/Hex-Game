#ifndef _HEX_GRAPH_H_
#define _HEX_GRAPH_H_

#include <stdlib.h>
#include <gsl/gsl_spmatrix.h>

struct graph_t {
  size_t num_vertices;  // Number of vertices in the graph
  gsl_spmatrix* t;      // Sparse matrix of size n*n,
                        // t[i][j] == 1 means there is an edge from i to j
  gsl_spmatrix* o;      // Sparse matrix of size 2*n, one line per player
                        // o[p][i] == 1 means that the vertex i is owned by p
};

#endif // _HEX_GRAPH_H_
