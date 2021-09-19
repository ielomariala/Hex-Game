#include <stdio.h>
#include <stdlib.h>
#include "graph_analysis.h"
#include <assert.h>

static struct set_void * graph_cluster__create_empty_node_set() {
    return set_void__empty();
}

static struct set_void * graph_cluster__create_empty_vertex_set() {
    return set_void__empty();
}

static struct set_void * graph_cluster__create_empty_link_set() {
    return set_void__empty();
}

void graph_cluster__free_node_set(struct set_void * set_node);

// =====================================================================================================================

struct graph_cluster * graph_cluster__init_simple_graph(struct graph_t * g_t, int color) {
    struct graph_cluster_vertex * graph = malloc(g_t->num_vertices * sizeof(struct graph_cluster_vertex));

    struct graph_cluster * g = malloc(sizeof(struct graph_cluster));
    g->nb_vertices = g_t->num_vertices;
    g->graph_v = graph;
    g->color = color;
    g->set_node = graph_cluster__create_empty_node_set();

    // Initializing single vertex node 
    for(size_t i = 0; i < g_t->num_vertices; ++i) {
        graph[i].number = i;

        struct graph_cluster_node * gcn = malloc(sizeof(struct graph_cluster_node));

        gcn->type = 0;
        gcn->group = &(graph[i]);
        gcn->color = gsl_spmatrix_get(g_t->o, color, i) ? color : gsl_spmatrix_get(g_t->o, !color, i) ? !color : -1;
        gcn->set_links = graph_cluster__create_empty_link_set();

        int result = set_void__add(g->set_node, gcn);

        if (result != SUCCESS) {
            printf("Error while adding basic node in graph_cluster__create_graph : vertex n°%ld \n", i);
        }
    }

    // Initializing simple links between nodes
    for(size_t i = 0; i < g_t->num_vertices; ++i) {
        for(size_t j = 0; j < g_t->num_vertices; ++j) {
            if(gsl_spmatrix_get(g_t->t, i, j)) {
                struct graph_cluster_node_link * l = malloc(sizeof(struct graph_cluster_node_link));
                // Creates a simple link to j
                l->is_simple = 1;
                l->link = set_void__get_element(g->set_node, j);
                set_void__add(((struct graph_cluster_node *) set_void__get_element(g->set_node, i))->set_links, l); // Add l to the links of i
            }
        }
    }

    return g;
}

/**
 * Creates a new graph_cluster using the graph_t g_t and following the color (of the player)
 **/
struct graph_cluster * graph_cluster__create_graph(struct graph_t * g_t, int color) {
    struct graph_cluster * g = graph_cluster__init_simple_graph(g_t, color);

    // Need to be changed !!
    assert(set_void__size(g->set_node) == g_t->num_vertices);
    //graph_cluster__create_aggregated_groups(g);

    return g;
}

void graph_cluster__remove_link_from_node(struct graph_cluster_node * n1, struct graph_cluster_node * n2) {
    for(size_t i =0; i < set_void__size(n1->set_links); ++i) {
        struct graph_cluster_node_link * l = (struct graph_cluster_node_link *) set_void__get_element(n1->set_links, i);
        if(graph_cluster__cmp_node(n2, graph_cluster__get_neighbor_from_link(l))) {
            set_void__remove(n1->set_links, l);
            graph_cluster__free_link(l);
        }
    }
}

// This function should be used whenever two nodes need to be merged to prevent wrong usage of the set
// It will merge the two nodes together if they are part of the set and add the merged node to the set
// Type of merge needs to be detected ...
// Pb : Merging virtual group with normal group... ? should simplify the structure by factorizing
struct graph_cluster_node * graph_cluster__merge_node(struct graph_cluster * g, struct graph_cluster_node * n1, struct graph_cluster_node * n2);

/** This function merge two nodes that are adjacent into one big node. The nodes composing the future node are factorized in the process.
 * This implies that if two nodes of type 0 or 1 are adjacent, then only one big node of type 1 will now represent them.
 * If a node of type 3 is merged with a type 0 or 1 then the funciton search for the component of the node of type 3 that is adjacent to
 * the other node.
 *  Pb : Merging virtual group with normal group... ? should simplify the structure by factorizing
* type 0 or 1 -> ez
* True pb is type 2
* Solution : Search for the group of type 0 or 1 that is adjacent to the group of type 0 or 1
**/
struct graph_cluster_node * graph_cluster__merge_adjacent_nodes(struct graph_cluster_node * n1, struct graph_cluster_node * n2) {
    if(n1->color != n2->color) {
        printf("ERROR in graph_cluster__merge_node : n1 and n2 does not share the same color \n");
        graph_cluster__print_node(n1);
        graph_cluster__print_node(n2);
        printf("==================== \n");
        return NULL;
    }

    graph_cluster__remove_link_from_node(n1, n2);
    graph_cluster__remove_link_from_node(n2, n1);

    // TYPE 0 AND TYPE 1 case
    if(n1->type <= 1 && n2->type <= 1) {
        return graph_cluster__merge_node_type01(n1, n2);
    }
    else { // TYPE 2 CASE
        // TODO
        printf("ozjfnjaiofa type merge node \n");
        return NULL;
    }
}

/** This function merge two node of type 0 or 1 and returns the result node
 * Returns : The result node if success, NULL otherwise 
 * Warning : It doesn't free the memory of the nodes given in parameters !
 **/
struct graph_cluster_node * graph_cluster__merge_node_type01(struct graph_cluster_node * n1, struct graph_cluster_node * n2) {
    if(n1->type > 1 || n2->type > 1) {
        printf("ERROR in graph_cluster__inject_node_into_type01 : nodes are not of type 0 or 1!! \n");
        return NULL;
    }
    if(n1->color != n2->color) {
        printf("ERROR in graph_cluster__inject_node_into_type01 : nodes does not share the same color \n");
        
        return NULL;
    }

    struct graph_cluster_node * merged_node = malloc(sizeof(struct graph_cluster_node));
    merged_node->type = 1;
    merged_node->group = malloc(sizeof(struct graph_cluster_group));
    merged_node->color = n1->color;
    merged_node->set_links = graph_cluster__create_empty_link_set();
    ((struct graph_cluster_group *) merged_node->group)->set_vertex = graph_cluster__create_empty_vertex_set();

    graph_cluster__node_add_nodes_type01(merged_node, n1);
    graph_cluster__node_add_nodes_type01(merged_node, n2);

    
    return merged_node;
}

/** This function adds all the component vertices of giving_n into receiving_n and the links that doesn't point to a vertex that is already pointed
 * into one of the links of receiving_n
 * Parameters : receiving_n and giving_n are of type 0 or 1
 * Returns : 1 if success, 0 otherwise
 **/
int graph_cluster__node_add_nodes_type01(struct graph_cluster_node * receiving_n, struct graph_cluster_node * giving_n) {
    if(receiving_n->type != 1) {
        printf("ERROR in graph_cluster__node_add_nodes_type01 : receiving node is not of type 1 \n");
        graph_cluster__print_node(receiving_n);
        graph_cluster__print_node(giving_n);
        printf("==================== \n");
        return 0;
    }
    if(giving_n->type > 1) {
        printf("ERROR in graph_cluster__node_add_nodes_type01 : giving node is not of type 0 or 1 \n");
        graph_cluster__print_node(receiving_n);
        graph_cluster__print_node(giving_n);
        printf("==================== \n");
        return 0;
    }

    if(giving_n->type == 0) {
        return set_void__add(((struct graph_cluster_group *) receiving_n->group)->set_vertex, giving_n->group);
    }
    else {
        return set_void__union(((struct graph_cluster_group *) receiving_n->group)->set_vertex, ((struct graph_cluster_group *) giving_n->group)->set_vertex);
    }

    for(size_t i = 0; i < set_void__size(giving_n->set_links); ++i) {
        // Faut enlever les liens qui pointent vers les mêmes nodes .... ._.
        // Il aurait crm mieux fallu utiliser des copies pour moins se casser la tete T-T
        set_void__add(receiving_n->set_links, set_void__get_element(giving_n->set_links, i));
    }
}

struct graph_cluster_node * graph_cluster__get_neighbor_from_link(struct graph_cluster_node_link  * l) {
    return l->is_simple ? ((struct graph_cluster_node *) l->link) : ((struct graph_cluster_two_bridge *) l->link)->neighbour;
}

// Returns the 1st generation groups for graph_cluster ... 1st generation groups are only formed of vertices of the same color that are neighbours
void graph_cluster__create_aggregated_groups(struct graph_cluster * g_c) {
    // Initialization
    struct set_void * new_node_set = graph_cluster__create_empty_node_set();
    int * tab_seen = malloc(g_c->nb_vertices * sizeof(int));
    for(size_t i = 0; i < set_void__size(g_c->set_node); ++i) {
        tab_seen[i] = 0;
    }

    for(size_t i = 0; i < set_void__size(g_c->set_node); ++i) {
        // Parcours en profondeur ?
        struct graph_cluster_node * n = set_void__get_element(g_c->set_node, i);
        if(!tab_seen[graph_cluster__get_number(AS_NODE(n))]) {
            set_void__add(new_node_set, inspect(g_c, tab_seen, n));
        }
    }
    
    free(tab_seen);
    set_void__free(g_c->set_node);
    g_c->set_node = new_node_set;

}

// TODO
size_t graph_cluster__get_number_virtual_group(struct graph_cluster_node * n) {
    (void) n;
    return 0;
}

size_t graph_cluster__get_number(struct graph_cluster_node * n) {
    return n->type == 0 ? ((struct graph_cluster_vertex *) n->group)->number 
    : n->type == 1 ? ((struct graph_cluster_vertex *) set_void__get_element(((struct graph_cluster_group *) n->group)->set_vertex, 0))->number
    : graph_cluster__get_number_virtual_group(n);
}

struct graph_cluster_node * inspect(struct graph_cluster * g_c, int * tab_seen, struct graph_cluster_node * n) {
    int a = graph_cluster__get_number(AS_NODE(n));
    tab_seen[a] = 1;
    if (n->color == -1) {
        return n;
    }

    struct graph_cluster_node * stack_node = n;
    // For each neighbour
    for(size_t j = 0; j < set_void__size(n->set_links); ++j) {
        struct graph_cluster_node * neighbour = graph_cluster__get_neighbor_from_link(set_void__get_element(n->set_links, j));
        if((neighbour->color == n->color) && !tab_seen[graph_cluster__get_number(neighbour)]) { // There shouldn't be any type 2 tho ...actually there should only be type 0
            stack_node = graph_cluster__merge_adjacent_nodes(stack_node, inspect(g_c, tab_seen, neighbour));
        }
    }
    return stack_node;
}

// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA TODO
int graph_cluster__cmp_node(void * n1, void * n2) {
    struct graph_cluster_node * node1 = n1;
    struct graph_cluster_node * node2 = n2;

    if(node1->type == node2->type) {
        return graph_cluster__get_number(node1) == graph_cluster__get_number(node2);
    }

    return -1;
}

void graph_cluster__print_node(struct graph_cluster_node * n) {
    if(n->type == 0) {
        printf("Node is a vertex : number = %ld \n", AS_VERTEX(n->group)->number);
    }
    else {
        printf("Node is of type %d \n", n->type);
    }
}

// ============================================== TODO ====================================================

// void graph_cluster__find_nearest_cluster()

/**
 * Factorize the graph_cluster as much as possible so the cluster merge into a bigger cluster everytime they can be seen as a virtual connection 
 * --> Theorem proving
 **/
void graph_cluster__compute(struct graph_cluster * graph_cluster);


// ################################## FREE FUNCTIONS, don't need to pay ay ##################

void graph_cluster__free(struct graph_cluster * g) {
    for (size_t i = 0; i < set_void__size(g->set_node); ++i) {
        graph_cluster__free_node(AS_NODE(set_void__get_element(g->set_node, i)));
    }
    set_void__free(g->set_node);
    free(g->graph_v);
    free(g);
    return;
}

void graph_cluster__free_node(struct graph_cluster_node * node) {
    if (node->type == 1) {
        graph_cluster__free_group((struct graph_cluster_group *) node->group);
    }
    else if(node->type > 1) {
        printf("OULAH FREE TYPE 2 PAS DEFINI \n");
    }

    for (size_t i = 0; i < set_void__size(node->set_links); ++i) {
        graph_cluster__free_link(set_void__get_element(node->set_links, i));
    }

    set_void__free(node->set_links);
    free(node);
}

void graph_cluster__free_group(struct graph_cluster_group * g) {
    set_void__free(g->set_vertex);
    free(g);
}

void graph_cluster__free_link(struct graph_cluster_node_link * l) {
    free(l);
}