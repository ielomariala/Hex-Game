#include "graph_analysis_test.h"
#include "helper_tests_func.h"
#include "../src/graph_struct.h"

void graph_init__tst() {
    size_t num_vertices = 10;
    struct graph_t * g = hexa_graph__empty(num_vertices);
    int player = 0;
    int opponent = 1;

    add__move_to_graph(g, 50, player);
    add__move_to_graph(g, 51, player);
    add__move_to_graph(g, 40, player);
    add__move_to_graph(g, 39, player);

    add__move_to_graph(g, 62, opponent);
    add__move_to_graph(g, 63, opponent);

    add__move_to_graph(g, 68, player);
    add__move_to_graph(g, 69, player);
    add__move_to_graph(g, 79, player);
    add__move_to_graph(g, 80, player);

    add__move_to_graph(g, 90, player);

    //hexa_graph__print(g);

    struct graph_cluster * g_clust = graph_cluster__init_simple_graph(g, player);

    int is_simple = 1;
    size_t player_count = 0;
    size_t opponent_count = 0;
    size_t two_neighb_count = 0;
    size_t three_neighb_count = 0;
    size_t four_neighb_count = 0;
    size_t six_neighb_count = 0;
    size_t unexpected_neighbours_number = 0;
    for(size_t i = 0; i < set_void__size(g_clust->set_node); ++i) {
        is_simple = ((AS_NODE(set_void__get_element(g_clust->set_node, i))->type == 0) && is_simple);
        if(AS_NODE(set_void__get_element(g_clust->set_node, i))->color == player) {
            player_count++;
        }
        else if (AS_NODE(set_void__get_element(g_clust->set_node, i))->color == opponent)
        {
            opponent_count++;
        }

        if (set_void__size(AS_NODE(set_void__get_element(g_clust->set_node, i))->set_links) == 6) {
            six_neighb_count++;
        }
        else if (set_void__size(AS_NODE(set_void__get_element(g_clust->set_node, i))->set_links) == 4) {
            four_neighb_count++;
        }
        else if (set_void__size(AS_NODE(set_void__get_element(g_clust->set_node, i))->set_links) == 3) {
            three_neighb_count++;
        }
        else if (set_void__size(AS_NODE(set_void__get_element(g_clust->set_node, i))->set_links) == 2) {
            two_neighb_count++;
        }
        else {
            printf("vertex number : %ld \n", AS_VERTEX(AS_NODE(set_void__get_element(g_clust->set_node, i))->group)->number);
            printf("number of neighbours = %ld \n", set_void__size(AS_NODE(set_void__get_element(g_clust->set_node, i))->set_links));
            unexpected_neighbours_number = 1;
        }
        
    }
    //AS_VERTEX(AS_NODE(set_void__get_element(g_clust->set_node, i))->group)->number
    struct test_module mod_func = test_module__create("graph_analysis init_simple_graph tests");
    test_module__add_test(&mod_func, set_void__size(g_clust->set_node) == g->num_vertices); //1

    test_module__add_test(&mod_func, is_simple);
    test_module__add_test(&mod_func, player_count == (2*num_vertices + 9)); //3
    test_module__add_test(&mod_func, opponent_count == (2*num_vertices + 2));

    test_module__add_test(&mod_func, !unexpected_neighbours_number); //5
    test_module__add_test(&mod_func, two_neighb_count == 2);
    test_module__add_test(&mod_func, three_neighb_count == 2);
    test_module__add_test(&mod_func, four_neighb_count == ((num_vertices-1) *4)); //8
    test_module__add_test(&mod_func, six_neighb_count == g->num_vertices - ((num_vertices) *4));

    test_module__conclude(&mod_func);
    
    //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA FREE GRAPH ANALYSIS
    graph_cluster__free(g_clust);
    graph__free(g);
}

void graph_analysis__aggregate_tst() {
    printf("graph_analysis tests \n");
    size_t num_vertices = 4;
    struct graph_t * g = hexa_graph__empty(num_vertices);
    int player = 0;
    int opponent = 1;

    add__move_to_graph(g, 6, player);
    
    add__move_to_graph(g, 18, opponent);

    hexa_graph__print(g);

     struct graph_cluster * g_clust = graph_cluster__init_simple_graph(g, player);
     graph_cluster__create_aggregated_groups(g_clust);

     graph_cluster__free(g_clust);
     graph__free(g);
}

void graph_analysis__tst() {
    graph_init__tst();
    
    // graph_analysis__aggregate_tst();
}
