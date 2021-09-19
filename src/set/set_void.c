#include <stddef.h>
#include "set_void.h"
#include <stdio.h>
#include <errno.h>

static int set_void__element_cmp(void * e1, void * e2) {
    return e1 - e2;
}

static size_t find(void* const s[], void* c, size_t length){
    size_t begin = 0;
    int end = length-1;
    size_t m = 0;
    while ((int) begin <= end){
        m = (end+begin)/2;
        if (s[m] != NULL && !set_void__element_cmp(s[m], c))
            return m;
        else {
            if (s[m] != NULL && set_void__element_cmp(s[m], c)<0){
                begin = m + 1;
            }
            else {
                end = m - 1;
            }   
        }
    }
    return begin;
}


static void shift_right(void* s[], int begin, int to_end){
    for (int i = to_end; i >= begin; i--){
        s[i+1]=s[i];
    }
}

static void shift_left(void* s[], int begin, int to_end){
    for (int i = begin; i <=to_end; i++){
        s[i-1]=s[i];
    }    
}

struct set_void *set_void__empty(){
    struct set_void *s = malloc(sizeof(struct set_void));
    s->s = malloc(sizeof(void*));
    s->size = 0;
    s->capacity = 1;
    return s;
} 

int set_void__is_empty(const struct set_void * s) {
    return (s->size == 0);
}

int set_void__find(const struct set_void *s, void* e) {
    size_t length = s->size; 
    for(size_t i=0; i < length; i++){
        if ( s->s[i] != NULL && set_void__element_cmp(s->s[i], e) == 0){
            return 1;
        }
    }
    return 0;
}

size_t set_void__size(const struct set_void *s) {
    return s->size;
}

int set_void__add( struct set_void *s, void* e ){
    if ( s->size == s->capacity  ){
        s->capacity *= 2;
        s->s = realloc(s->s, s->capacity*sizeof(void*));
    }
    size_t length = s->size;
    size_t index = find(s->s, e, length);
    if (index < length && (s->s[index] != NULL && set_void__element_cmp(s->s[index], e) == 0)) {return 1;}
    s->size++;
    shift_right(s->s, index, length - 1);
    s->s[index] = e;
    return SUCCESS;
}

int set_void__remove(struct set_void *s, void* e){
    if (set_void__is_empty(s)){
        return 1;
    }
    size_t length = s->size;
    size_t index = find(s->s, e, length);
    if ( index >= length || (s->s[index] != NULL && set_void__element_cmp(s->s[index], e))){
        return 1;
    }
    if ( 2*length <= s->capacity ){
        s->capacity /= 2;
        s->s = realloc(s->s, s->capacity*sizeof(void*));
    }
    shift_left(s->s, index + 1, length - 1);
    s->size--;
    return SUCCESS; 
}

void set_void__free(struct set_void *s){
    free(s->s);
    free(s);
}

int set_void__union(struct set_void *s1, struct set_void *s2){
    int n = set_void__size(s2);
    int j = 0;
    for (int i = 0; i < n; i++){
        j += !set_void__add(s1, s2->s[i]);
    }
    return ( j == n );
}

void set_void__debug_data(const struct set_void *s, char *c ){
    size_t i = 0;
    while (i < s->size){
        int *adress = s->s[i];
        printf("%d%s", *adress, c);
        i++;
    }
    printf("\n");
}

void * set_void__get_element(const struct set_void *s, size_t i) {
    if(i < set_void__size(s)) {
        return s->s[i];
    }
    else {
        printf("Error: set__get_element: Index out of bound. set size is %ld and index asked is %ld \n", set_void__size(s), i);
        return NULL;
    }
}