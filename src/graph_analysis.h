#ifndef __graph_analysis__H
#define __graph_analysis__H

#include "graph.h"
#include "./set/set_void.h"

#define AS_NODE(V) (((struct graph_cluster_node *) V))
#define AS_VERTEX(V) (((struct graph_cluster_vertex *) V))

/**
 * Structures for representing vertex in graph_cluster structures
 * number is the number given as a name to the vertices, it reflects its connectin with graph_t graph
 * links is the list of vertex from which the vertex is connected
 * is_empty says if the vertex is really placed on the board
 **/
struct graph_cluster_vertex {
    size_t number;
};

/**
 * Structures for visualizing virtual connections between vertices as clusters. 
 * cluster regroups all the vertices forming the group (meaning that all the vertices here are connected together)
 * out_links regroups all the links of the cluster that have a link with another cluster
 **/
struct graph_cluster_group {
    // size_t vertex_number; // Merci Ismael
    // size_t max_vertex; //
    // list of pointer
    // struct graph_cluster_vertex ** cluster; //

    struct set_void * set_vertex; // List of the vertices that composed the group
                             // This set is of type graph_cluster_vertex
};

/**
 * Structure that represents groups of groups processed by compute fonction (Theorem proving)
 **/
struct graph_cluster_virtual_group {
    struct graph_cluster_node * n1; // n1 and n2 have the same color
    struct graph_cluster_node * n2;

    struct graph_cluster_node_link * l; // This should always be a two-bridge or anything like that
    // Should be at least not a simple link
};

/**
 * Structure to encapsulate graph_cluster_group which is the atom type and virtual_group type
 **/
struct graph_cluster_node {
    int type; // 0 = vertex, 1 = group, 2 = virtual_group
    void * group; // Can be either graph_cluster_group or graph_cluster_virtual_group o graph_cluster_vertex
    
    int color; // -1 empty else color of the player

    struct set_void * set_links; // List of links of the node -> neighbours
                            // This set is of type graph_cluster_node_link
};

/**
 * Structure to encapsulate link
 **/
struct graph_cluster_node_link {
    int is_simple; // 1 means that link is a node, 0 would mean that link is graph_cluster_two_bridge
    void * link; // Can either be a cluster_node if is_simple is true
                 // or a graph_cluster_two_bridge if it is not
};

struct graph_cluster_two_bridge {
    struct graph_cluster_node * neighbour; // might not be needed since it is contained into component
    struct graph_cluster_node * component[2]; // Those can have complex links
};

/**
 * Structure to help analysing graphs using virtual connections and clusters
 * 
 **/
struct graph_cluster {
    int color;
    // not used directly, mostly to free at the end of usage
    size_t nb_vertices;
    struct graph_cluster_vertex * graph_v;

    struct set_void * set_node; // The set of node that composed the current representation
                           // This set is of type graph_cluster_node
};

/**
 * Creates a new graph_cluster using the graph_t g_t and following the color (of the player) and aggregates the nodes of the same colour together
 **/
struct graph_cluster * graph_cluster__create_graph(struct graph_t * g_t, int color);

/**
 * Creates a new graph_cluster using the graph_t g_t and following the color (of the player)
 **/
struct graph_cluster * graph_cluster__init_simple_graph(struct graph_t * g_t, int color);

void graph_cluster__create_aggregated_groups(struct graph_cluster * g_c);

/**
 * Factorize the graph_cluster as much as possible so the cluster merge into a bigger cluster everytime they can be seen as a virtual connection
 * might need additionnal parameter begin and end to form a path to join
 **/
void graph_cluster__compute(struct graph_cluster * graph_cluster);

/**
 * hmmm retourner la forme n-1 de compute
 * might not be needed
 **/
struct graph_cluster_group graph_cluster__develop(struct graph_cluster_group * g);

void graph_cluster__free(struct graph_cluster * graph_cluster);

// Functions for cluster ... en fait c'est un set caché mais j'ai la flemme
struct graph_cluster_group * graph_cluster__create_cluster_group();

void graph_cluster__add_vertex_to_group(struct graph_cluster_group * g, struct graph_cluster_vertex * v);

struct graph_cluster_node * graph_cluster__create_new_node(int is_virtual, void * g);

struct graph_cluster_virtual_group * graph_cluster__create_new_virtual_group(struct graph_cluster_node * n1, struct graph_cluster_node * n2);

struct graph_cluster_node * graph_cluster__get_neighbor_from_link(struct graph_cluster_node_link  * l);
// #################################################
// Priority
struct graph_cluster_node * graph_cluster__merge_node(struct graph_cluster * g, struct graph_cluster_node * n1, struct graph_cluster_node * n2);

// Free Functions
void graph_cluster__free_node(struct graph_cluster_node * node);
void graph_cluster__free_group(struct graph_cluster_group * g);
void graph_cluster__free_link(struct graph_cluster_node_link * l);
// #################################################

int graph_cluster__cmp_node(void * n1, void * n2);

struct graph_cluster_node * graph_cluster__merge_node_type01(struct graph_cluster_node * n1, struct graph_cluster_node * n2);
int graph_cluster__node_add_nodes_type01(struct graph_cluster_node * receiving_n, struct graph_cluster_node * giving_n);
struct graph_cluster_node * inspect(struct graph_cluster * g_c, int * tab_seen, struct graph_cluster_node * n);

// ################################################
// TO COMPLETE
size_t graph_cluster__get_number(struct graph_cluster_node * n);

// ################################################

void graph_cluster__print_node(struct graph_cluster_node * n);
#endif