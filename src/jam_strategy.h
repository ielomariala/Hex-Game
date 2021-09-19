#ifndef __JAM_STRATEGY_H
#define __JAM_STRATEGY_H

#include "set/set_dynamic.h"
#include "queue.h"
#include "graph.h"

// Returns the color of the vertex i in the graph_t g_t. Returns -1 the vertex is i is colorless
int get_color_int(struct graph_t *g_t, int i);

// Forms the queue that contains the smallest path, father must be instantiated and filled
size_t find_way(struct Queue* queue, int* father, int v);

// Returns the smallest path between vertex_begin and vertex_end using a set of connected component cc_list. Fill the the path at the adress queue
// Condition : vertex_begin, vertex_end and vertices in cc_list must be of colour color
//             Queue must be instantiate outside of the function
size_t parcours_largeur(int vertex_begin, int vertex_end, struct set *cc_list, struct graph_t* M, int color, struct Queue* queue);

#endif
