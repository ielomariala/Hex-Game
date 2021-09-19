#ifndef __COMPO_CONNEXES_H
#define __COMPO_CONNEXES_H

#include <stdio.h>
#include "./set/set_dynamic.h"
#include "graph.h"

// Creates a set meant to contains int, this equal to connected component
struct set * create__set_int();

// Creates a set meant to contains set of int
struct set * create__set_cc();

// Returns the color of the vertexi in the graph g_t, Returns -1 if the case is empty
int get_color_size_t(struct graph_t * g_t, size_t i);

// Returns the connected component ie set of int in set_cc that contains vertex
const struct set *find__cc(struct set *set_cc, int vertex);

// Returns a set of conected componnent (ie set of int) that have the color given in parameters
struct set * ignite_last_hope(struct graph_t * g_t, int color);

// Print a connected component
void print_cc(struct set * s);

// Returns a set containing all the neighbours of the connected_component cc. Neighbours are int contains into a set.
struct set * voisins(const struct set * cc, struct graph_t * g_t, int color);

// Returns a set containing all the neighbours of the vertex vertex. Neighbours are int contains into a set.
struct set * voisins_vertex(int vertex, struct graph_t * g_t, int color);

#endif
