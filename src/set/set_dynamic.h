#ifndef __SET_DYNAMIC_H
#define __SET_DYNAMIC_H

#include <stdlib.h>
#include <stdio.h>
#include "set.h"

#ifndef SET__SIZE
#define SET__SIZE 10
#endif 

struct set{
    void** s;
    size_t capacity;
    size_t size;
    void *(*copy)(const void *);
    void (*free)(void *);
    int (*cmp)(const void *, const void *);
};

#endif