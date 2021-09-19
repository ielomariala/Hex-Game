#include <stdio.h>
#include <string.h>
#include "helper_tests_func.h"

#define SUCCESS "\033[32;01mSUCCESS\033[00m"
#define FAILED "\033[31;01mFAILED\033[00m"

struct test_module test_module__create(char* func_name) {
    struct test_module module = {
        .total_tests = 0,
        .nb_success = 0
    };
    strcpy(module.func_name, func_name);
    return module;
}

void test_module__add_test(struct test_module * module, int result) {
    module->total_tests++;
    if(result) {
        module->nb_success++;
    }
    else {
        printf("%s : Test number %d of %s \n", FAILED, module->total_tests, module->func_name);
    }
}

void test_module__conclude(struct test_module * module) {
    print_tst_result(module->func_name, module->total_tests == module->nb_success, module->nb_success, module->total_tests);
}

void print_string_err(const char* exp, const char* rec){
  if (exp == NULL && rec == NULL)
    printf("Expected : NULL, Received : NULL\n");
  else if (exp == NULL)
    printf("Expected : NULL, Received : %s\n", rec);
  else if (rec == NULL)
    printf("Expected : %s, Received : NULL\n", exp);
  else
    printf("Expected : %s, Received : %s\n", exp, rec);
}

void print_int_err (int exp, int rec){
  printf ("Expected : %d, Received : %d\n", exp, rec);
}

void print_tst_result (char* func, int b, int num_tst, int tot){
  if (b)
    printf("%s : %s (%d/%d)\n", func, SUCCESS, num_tst, tot);
  else
    printf("%s : %s (%d)\n", func, FAILED, num_tst);
}
