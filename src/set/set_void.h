#ifndef __SET_VOID_H
#define __SET_VOID_H

#include <stdlib.h>
#include <stdio.h>
#ifndef SET_VOID__SIZE
#define SET_VOID__SIZE 10
#endif 

struct set_void{
    void** s;
    size_t capacity;
    size_t size;
    void *(*copy)(const void *);
    void (*free)(void *);
    int (*cmp)(const void *, const void *);
};

#define SUCCESS 0

struct set_void *set_void__empty(); 

int set_void__is_empty(const struct set_void * s);

int set_void__find( const struct set_void *s, void* e);

size_t set_void__size( const struct set_void *s);

int set_void__add(struct set_void *s, void* e );

int set_void__remove( struct set_void *s, void* e);

void set_void__free(struct set_void *s);

// Adds set s2 to set set1
int set_void__union(struct set_void *s1, struct set_void *s2);

void set_void__debug_data(const struct set_void *s, char *c );

// Hmmm ... this might break the "you can't modify the element in the set" property ...
// Be careful when using this function !! Especially when adding or removing an element !
// This function could return a copy ... but ...hm
void * set_void__get_element(const struct set_void *s, size_t i);

#endif