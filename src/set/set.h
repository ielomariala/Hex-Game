#ifndef SET_H
#define SET_H

#include <stdlib.h>

#define SUCCESS 0

struct set *set__empty(int (*cmp)(const void *, const void *), void *(*copy)(const void *), void (*free)(void *)); 

int set__is_empty(const struct set* s);

int set__find(const struct set *s, void* e);

size_t set__size( const struct set *s);

int set__add( struct set *s, void* e );

int set__remove( struct set *s, void* e);

void set__free(struct set *s);

// Adds set s2 to set set1
int set__union(struct set *s1, struct set *s2);

void set__debug_data(const struct set *s, char *c );

void * set__get_element(const struct set *s, size_t i);
#endif
