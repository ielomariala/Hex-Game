#include "is_winning.h"
#define TRUE 1
#define FALSE 0


//return True if a path of vertices of the color color exists from the vertice start to the vertice finish, False if not
size_t search_path(struct graph_t* graph, int start, int finish, enum color_t color){
  int n=graph->num_vertices;
  int mark_vertices[n];
  for(int i=0; i<n; i++){
    mark_vertices[i]=0;
  }
  int v = start;
  int v_next;
  struct pile* p = pile__init();
  pile__add(p, v);
  mark_vertices[v]=1;
  while(!pile__is_empty(p)){
    v_next=0;
    //loop stops when vertex v_next out of bound, or when v_next is a neighbour of v, of the right color and not marked
    while(v_next<n && (!gsl_spmatrix_get(graph->t, v, v_next) || !gsl_spmatrix_get(graph->o, color, v_next) || mark_vertices[v_next])){
      v_next++;
    }
    if(v_next<n){
      if(v_next == finish){
	      pile__free(p);
	      return TRUE;
      }
      pile__add(p, v_next);
      mark_vertices[v_next]=1;
      v=v_next;
    }else{
      free(pile__pop(p));
      v=pile__top(p);
    }
  }
  pile__free(p);
  return FALSE;
}

//integer square root, used to get the size m of a graph
int intsqrt(int x){ 
  if (x == 0 || x == 1) 
    return x; 
  
  int i = 1, result = 1; 
  while (result <= x){ 
    i++; 
    result = i * i; 
  } 
  return i - 1; 
}

//if a player win, return his color, if not return NO_COLOR
enum color_t is_winning(struct graph_t * graph, char shape){
  int start_v_black;
  int finish_v_black;
  int start_v_white;
  int finish_v_white;
  int m;
  switch (shape){
  case 'c':
    m=intsqrt(graph->num_vertices)-1;
    start_v_black=0;
    finish_v_black=graph->num_vertices-1;
    start_v_white=m;
    finish_v_white=graph->num_vertices-m-1;
    break;
  case 'h':
    m=intsqrt(graph->num_vertices)-1;
    start_v_black=0;
    finish_v_black=graph->num_vertices-1;
    start_v_white=m;
    finish_v_white=graph->num_vertices-m-1;
    break;
  case 't':
    return NO_COLOR;
    break;
  }
  if(search_path(graph, start_v_black, finish_v_black, BLACK)){
    return BLACK;
  }else if(search_path(graph, start_v_white, finish_v_white, WHITE)){
    return WHITE;
  }else{
  return NO_COLOR;
  }
}

