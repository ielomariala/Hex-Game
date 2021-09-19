#include "compo_connexes_test.h"
#include "helper_tests_func.h"
#include "../src/graph_struct.h"

void find__cc_tst() {
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    struct test_module mod_func = test_module__create("compo_connexes find__cc tests");

    struct set * s1 = create__set_int();
    set__add(s1, &a);
    set__add(s1, &b);
    set__add(s1, &c);

    struct set * s2 = create__set_int();
    set__add(s2, &d);

    struct set * s3 = create__set_cc();
    test_module__add_test(&mod_func, find__cc(s3, 4) == NULL); // set is empty

    set__add(s3, s1);
    set__add(s3, s2);

    test_module__add_test(&mod_func, set__find(find__cc(s3, 1), &a));
    test_module__add_test(&mod_func, set__find(find__cc(s3, 4), &d));
    test_module__add_test(&mod_func, find__cc(s3, 5) == NULL);

    test_module__conclude(&mod_func);
    set__free(s1);
    set__free(s2);
    set__free(s3);
}

void ignite_final_hope_tst() {
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
    struct set * s = ignite_last_hope(g, player);
    struct set * s2 = ignite_last_hope(g, opponent);

    add__move_to_graph(g, 47, player);
    struct set * s3 = ignite_last_hope(g, player);

    struct test_module mod_func = test_module__create("compo_connexes ignite_last_hope tests");
    test_module__add_test(&mod_func, set__size(s) == 4);
    test_module__add_test(&mod_func, set__size(s2) == 3);
    test_module__add_test(&mod_func, set__size(s3) == 5);

    /*for(size_t i = 0; i < set__size(s); ++i) {
        print_cc((struct set *) set__get_element(s, i));
    }*/
    test_module__conclude(&mod_func);

    set__free(s);
    set__free(s2);
    set__free(s3);
    graph__free(g);
}

void compo_connexes__tst() {
    find__cc_tst();
    ignite_final_hope_tst();
}
