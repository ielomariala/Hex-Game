#include <stddef.h>
#include "set_dynamic.h"
#include "set.h"
#include <stdio.h>
#include <errno.h>

static size_t find(void* const s[], void* c, size_t length, int (*cmp)(const void *, const void *)){
    size_t begin = 0;
    int end = length-1;
    size_t m = 0;
    while ((int) begin <= end){
        m = (end+begin)/2;
        if (s[m] != NULL && ! cmp(s[m], c))
            return m;
        else {
            if (s[m] != NULL && cmp(s[m], c)<0){
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

struct set *set__empty(int (*cmp)(const void *, const void *), void *(*copy)(const void *), void (*free)(void *)){
    struct set *s = malloc(sizeof(struct set));
    s->s = malloc(sizeof(void*));
    s->size = 0;
    s->capacity = 1;
    s->cmp = cmp;
    s->copy = copy;
    s->free = free;
    return s;
} 

int set__is_empty(const struct set* s){
    return (s->size == 0);
}

int set__find( const struct set *s, void* e){
    size_t length = s->size; 
    for(size_t i=0; i < length; i++){
        if ( s->s[i] != NULL && !s->cmp(s->s[i], e) ){
            return 1;
        }
    }
    return 0;
}

size_t set__size( const struct set *s){
    return s->size;
}

int set__add( struct set *s, void* e ){
    if ( s->size == s->capacity  ){
        s->capacity *= 2;
        s->s = realloc(s->s, s->capacity*sizeof(void*));
    }
    size_t length = s->size;
    size_t index = find(s->s, e, length, s->cmp);
    if (index < length && (s->s[index] != NULL && !s->cmp(s->s[index], e))) {return 1;}
    s->size++;
    shift_right(s->s, index, length - 1);
    s->s[index] = s->copy(e);
    return SUCCESS;
}

int set__remove( struct set *s, void* e){
    if ( set__is_empty(s)){
        return 1;
    }
    size_t length = s->size;
    size_t index = find(s->s, e, length, s->cmp);
    if ( index >= length || (s->s[index] != NULL && s->cmp(s->s[index], e))){
        return 1;
    }
    if ( 2*length <= s->capacity ){
        s->capacity /= 2;
        s->s = realloc(s->s, s->capacity*sizeof(void*));
    }
    s->free(s->s[index]);
    shift_left(s->s, index + 1, length - 1);
    s->size--;
    return SUCCESS; 
}

void set__free(struct set *s){
    size_t i = 0;
    while ( i < s->size ){
        s->free(s->s[i]);
        i++;
    }
    free(s->s);
    free(s);
}

int set__union(struct set *s1, struct set *s2){
    int n = set__size(s2);
    int j;
    for (int i = 0; i < n; i++){
        j += !set__add(s1, s2->s[i]);
    }
    return ( j == n );
}

void set__debug_data(const struct set *s, char *c ){
    size_t i = 0;
    while (i < s->size){
        int *adress = s->s[i];
        printf("%d%s", *adress, c);
        i++;
    }
    printf("\n");
}

void * set__get_element(const struct set *s, size_t i) {
    if(i < set__size(s)) {
        return s->s[i];
    }
    else {
        printf("Error set__get_element: Index out of bound \n");
        return NULL;
    }
}