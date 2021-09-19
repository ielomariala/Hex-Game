#include "compo_connexes.h"

// Finds in which connected component is vertex given a list of connected component
// Warning : it gives direct access to the component !
// Returns NULL if not found
const struct set *find__cc(struct set *set_cc, int vertex)
{
  for (size_t i = 0; i < set__size(set_cc); ++i)
  {
    if (set__find(((const struct set *)set__get_element(set_cc, i)), &vertex))
    {
      return (const struct set *)set__get_element(set_cc, i);
    }
  }
  return NULL;
}

int cmp_int(const void * v1, const void * v2) {
    return *((const int *) v2) - *((const int *) v1);
}

void * copy_int(const void * v) {
    int * i = malloc(sizeof(int));
    *i = *((int *) v);
    return i;
} 

void free_int(void * v) {
    free(v);
}

int cmp_set_int(const void * v1, const void * v2) {
    return v2 - v1;
}

void * copy_set_int(const void * v) {
    const struct set * s2 = (const struct set *) v;
    struct set * s = set__empty(cmp_int, copy_int, free_int);

    for (size_t i = 0; i < set__size(s2); ++i) {
        set__add(s, set__get_element(s2, i));
    }
    return s;
}

void free_set_int(void * v) {
    set__free((struct set *) v);
}

struct set * create__set_int() {
    return set__empty(cmp_int, copy_int, free_int);
}

struct set * create__set_cc() {
    return set__empty(cmp_set_int, copy_set_int, free_set_int);
}

// Returns -1 if empty, else the color of the vertex of number i
int get_color_size_t(struct graph_t * g_t, size_t i) {
    return gsl_spmatrix_get(g_t->o, 0, i) ? 0 :
           gsl_spmatrix_get(g_t->o, 1, i) ? 1 :
           -1;
}

// Returns 1 if the vertices i and j are neighbours in g_t
int are_neighbours(struct graph_t * g_t, size_t i, size_t j) {
    return gsl_spmatrix_get(g_t->t, i, j);
}

// Creates a connected component s starting from i
void create__cc(struct graph_t * g_t, struct set * s, int * tab_seen, size_t i) {
    tab_seen[i] = 1;
    set__add(s, &i);

    for(size_t j = 0; j < g_t->num_vertices; ++j) {
        if (!tab_seen[j] && are_neighbours(g_t, i, j) && get_color_size_t(g_t, i) == get_color_size_t(g_t, j)) {
            create__cc(g_t, s, tab_seen, j);
        }
    }
}

// Prays god that it works and creates a set containing all the connected component of the colour color in the graph_t g_t
struct set * ignite_last_hope(struct graph_t * g_t, int color) {
    struct set * s = set__empty(cmp_set_int, copy_set_int, free_set_int);
    int * tab_seen = malloc(sizeof(int) * g_t->num_vertices);

    for(size_t i = 0; i < g_t->num_vertices; ++i) {
        tab_seen[i] = 0;
    }

    for(size_t i = 0; i < g_t->num_vertices; ++i) {
        if(!tab_seen[i] && gsl_spmatrix_get(g_t->o, color, i)) {
            struct set * cc = set__empty(cmp_int, copy_int, free_int);
            create__cc(g_t, cc, tab_seen, i);
            set__add(s, cc);
            set__free(cc);
        }
    }
    free(tab_seen);
    
    return s;
}

struct set * voisins(const struct set * cc, struct graph_t * g_t, int color) {
    struct set * voisins = set__empty(cmp_int, copy_int, free_int);
    for(size_t i = 0; i < set__size(cc); ++i) {
        int vertex = *((int *) set__get_element(cc, i));
        for(size_t j = 0; j < g_t->num_vertices; ++j) {
            if(are_neighbours(g_t, vertex, j)) {
                int c = get_color_size_t(g_t, j); 
                if(c == -1 || c == color) { // Case is empty
                    set__add(voisins, &j);
                }
            }
        }
    }
    return voisins;
}

struct set * voisins_vertex(int vertex, struct graph_t * g_t, int color) {
    struct set * voisins = set__empty(cmp_int, copy_int, free_int);
    for(size_t j = 0; j < g_t->num_vertices; ++j) {
            if(are_neighbours(g_t, vertex, j)) {
                int c = get_color_size_t(g_t, j); 
                if(c == -1 || c == color) { // Case is empty
                    set__add(voisins, &j);
                }
            }
        }
    return voisins;
}

void print_cc(struct set * s) {
    printf("Connected componnent composed of : ");
    for (size_t i = 0; i < set__size(s); ++i) {
        printf("%d ", *((int*) set__get_element(s, i)));
    }
    printf("\n");
}
