#include "set_void_test.h"
#include "helper_tests_func.h"

void set_void__tst() {
    struct test_module mod_func = test_module__create("set_void__functionnal_tests");
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    char * f = "27";
    double g = 8749;

    struct set_void * s = set_void__empty();
    test_module__add_test(&mod_func, set_void__is_empty(s));
    test_module__add_test(&mod_func, set_void__size(s) == 0);

    test_module__add_test(&mod_func, set_void__add(s, &a) == SUCCESS);
    test_module__add_test(&mod_func, set_void__add(s, &b) == SUCCESS);
    test_module__add_test(&mod_func, set_void__add(s, &c) == SUCCESS);
    test_module__add_test(&mod_func, set_void__add(s, &d) == SUCCESS);
    test_module__add_test(&mod_func, set_void__size(s) == 4);

    test_module__add_test(&mod_func, set_void__add(s, &d) == !SUCCESS);
    test_module__add_test(&mod_func, set_void__size(s) == 4);

    test_module__add_test(&mod_func, set_void__find(s, &a));
    test_module__add_test(&mod_func, set_void__find(s, &b));
    test_module__add_test(&mod_func, set_void__find(s, &c));
    test_module__add_test(&mod_func, set_void__find(s, &d));
    test_module__add_test(&mod_func, set_void__size(s) == 4);

    test_module__add_test(&mod_func, set_void__get_element(s, 0) == &a);
    test_module__add_test(&mod_func, set_void__get_element(s, 1) == &b);
    test_module__add_test(&mod_func, set_void__get_element(s, 3) == &d);

    test_module__add_test(&mod_func, set_void__remove(s, &c) == SUCCESS);
    test_module__add_test(&mod_func, set_void__size(s) == 3);
    test_module__add_test(&mod_func, set_void__get_element(s, 0) == &a);
    test_module__add_test(&mod_func, set_void__get_element(s, 1) == &b);
    test_module__add_test(&mod_func, set_void__get_element(s, 2) == &d);
    test_module__add_test(&mod_func, !set_void__find(s, &c)); //20

    struct set_void * s2 = set_void__empty();
    test_module__add_test(&mod_func, set_void__add(s, &e) == SUCCESS);
    test_module__add_test(&mod_func, set_void__union(s2, s)); //22
    test_module__add_test(&mod_func, set_void__size(s2) == 4);
    test_module__add_test(&mod_func, set_void__find(s2, &a));
    test_module__add_test(&mod_func, set_void__find(s2, &d));
    test_module__add_test(&mod_func, set_void__find(s2, &d));
    test_module__add_test(&mod_func, set_void__find(s2, &e));
    test_module__add_test(&mod_func, !set_void__find(s2, &c));

    test_module__add_test(&mod_func, set_void__add(s2, &f) == SUCCESS);
    test_module__add_test(&mod_func, set_void__add(s2, &g) == SUCCESS);
    test_module__add_test(&mod_func, set_void__size(s2) == 6);
    test_module__add_test(&mod_func, set_void__find(s2, &f));
    test_module__add_test(&mod_func, set_void__remove(s2, &f) == SUCCESS);
    test_module__add_test(&mod_func, !set_void__find(s2, &f));

    for(size_t i = 0; i < set_void__size(s2) - 2; ++i) {
        test_module__add_test(&mod_func, (set_void__get_element(s2, i) - set_void__get_element(s2, i + 1)) < 0);
    }

    set_void__free(s);
    set_void__free(s2);
    test_module__conclude(&mod_func);
}