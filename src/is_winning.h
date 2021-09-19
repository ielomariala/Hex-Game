#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "move.h"
#include "pile.h"
#include "player.h"
#include "graph_struct.h"

size_t search_path(struct graph_t* graph, int start, int finish, enum color_t color);

enum color_t is_winning(struct graph_t* graph, char shape);
