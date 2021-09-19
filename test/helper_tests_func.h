#ifndef __HELPER_TESTS_FUNC
#define __HELPER_TESTS_FUNC

struct test_module {
    char func_name[40];
    int total_tests;
    int nb_success;
};

// Create a new module for testing purpose. Func_name refers to the name of the function the user wants to test
// func_name cannot exceed 40 characters.
struct test_module test_module__create(char * func_name);

// Adds a new test to the module. The result variable refers to the outcome of a test on the function
// Example : test_module__add_test(module, Equals(Bernard, Juliette))
void test_module__add_test(struct test_module * module, int result);

// Summarize and display the overall success of the module.
void test_module__conclude(struct test_module * module);


// ========================================================================================================================================
// Extra functions
void print_string_err(const char* exp, const char* rec);

void print_int_err (int exp, int rec);

void print_tst_result (char* func, int b, int num_tst, int tot);
#endif