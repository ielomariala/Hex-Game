#include <stdlib.h>
#include "pile.h"
static struct lelement sentinel = {-1, &sentinel};
#define SENTINEL &sentinel

struct pile* pile__init(){
  struct pile* p=malloc(sizeof(struct pile));
  p->first=SENTINEL;
  return p;
}

int pile__is_empty (const struct pile* p){
  return (p->first->next==p->first);
}

void pile__add(struct pile* p,int x){
  struct lelement* added_llm = malloc(sizeof(struct lelement));
  added_llm->data=x;
  added_llm->next=p->first;
  p->first=added_llm;
}

int pile__top(struct pile* p){
  if(!pile__is_empty(p)){
    return p->first->data;
  }else{
    return -1;
  }
}

struct lelement* pile__pop(struct pile* p){
  if(pile__is_empty(p)){
    return NULL;
  }
  struct lelement *poped_llm=p->first;
  p->first=poped_llm->next;
  poped_llm->next=NULL;
  return poped_llm;
}

void pile__free(struct pile* p){
  while(!pile__is_empty(p)){
    free(pile__pop(p));
  }
  free(p);
}
  



  
