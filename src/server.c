#include <string.h>
#include "is_winning.h"
#include "player.h"
#include "graph_struct.h"
#include <dlfcn.h>
#include <getopt.h>
#include <time.h>


#define NB_PLAYERS 2

#define MAX_TURN 300

#define TRUE 1
#define FALSE 0
#define FAILURE 1

static char shape = 'h';
static int m = 9;


void parse_opts(int argc, char*argv[]){
  int opt;
  while ((opt = getopt(argc, argv, "t:m:")) != -1) {
    switch (opt) {
    case 't':
      if ( optarg[0] != 'c' && optarg[0] != 'h' && optarg[0] != 't' ) {
	printf("Vous avez choisi in format incorrect\n");
      }else{
	shape = optarg[0];
      }
      break;
    case 'm':
      m = atoi(optarg);
      if ( m <= 2) {
	printf("Le jeu ne peut pas avoir lieu avec le 'm' choisi\n");
      }
      break;
    default: 
      fprintf(stderr, "Usage: %s [-t shape of graph -m Number of side points] \n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}


struct player{
  char const* (* get_player_name)();
  struct move_t (* propose_opening)();
  int (*accept_opening)(const struct move_t opening);
  void (*initialize_graph)(struct graph_t* graph);
  void (*initialize_color)(enum color_t id);
  struct move_t (*play)(struct move_t previous_move);
  void (*finalize)();
  enum color_t color;
};

//initialize a player with the function of the library corresponding to handle
struct player make_player(void* handle){
  struct player p;
  p.get_player_name=dlsym(handle, "get_player_name");
  p.propose_opening=dlsym(handle,"propose_opening");
  p.accept_opening=dlsym(handle, "accept_opening");
  p.initialize_graph=dlsym(handle, "initialize_graph");
  p.initialize_color=dlsym(handle, "initialize_color");
  p.play=dlsym(handle, "play");
  p.finalize=dlsym(handle, "finalize");
  p.color=NO_COLOR;
  return p;
}

//apply the move move on the board graph, return 1 if the move is valid,  if not
int apply_move(struct graph_t* graph, struct move_t move){
  if((move.c==0 || move.c==1) && (!gsl_spmatrix_get(graph->o, WHITE, move.m) && !gsl_spmatrix_get(graph->o, BLACK, move.m))){
    gsl_spmatrix_set(graph->o, move.c, move.m, 1);
    return 1;
  }else{
    printf("invalid move : %ld \n", move.m);
    return 0;
  }
}

//determine the next player after the move prev_move
enum color_t compute_next_player(struct move_t prev_move){
  return !(prev_move.c);
}


int winning(){
  return FALSE;
}

//////////////////////////////////////main////////////////////////////////////////
int main(int argc, char* argv[]){
  srand(time(NULL));
  parse_opts(argc , argv);
  printf("Le format de graphe choisi est %c\n",shape);
  printf("Le nombre de sommets de cote choisi est %d\n",m);

  if (argc<NB_PLAYERS){ 
    return FAILURE;
  }
  //players initialisation 
  struct player tab_players[NB_PLAYERS];
  void* tab_handles[NB_PLAYERS];
  for(int i=0; i<NB_PLAYERS; i++){
    printf("%s\n",argv[i+optind]);
    tab_handles[i]=dlopen(argv[i+optind], RTLD_LAZY); // argv[argc-2+i]
    tab_players[i]=make_player(tab_handles[i]);
    tab_players[i].color=i;
  }
  enum color_t winner=NO_COLOR;
  
  //graph initialisation
  struct graph_t* graph=make_graph(m, shape);
  struct graph_t* tab_graph[NB_PLAYERS];
  for(int i=0; i<NB_PLAYERS; i++){
    tab_graph[i]=graph_copy(graph);
    tab_players[i].initialize_graph(tab_graph[i]);
    
  }

  //openning
  struct player *current_player;
  struct move_t move = tab_players[BLACK].propose_opening();
  printf("%s Choses %zu as an openning\n", tab_players[BLACK].get_player_name(),move.m);
  if(!tab_players[WHITE].accept_opening(move)){
    printf("%s Declines and Swaps\n", tab_players[WHITE].get_player_name());
    struct player tmp=tab_players[BLACK];
    tab_players[BLACK]=tab_players[WHITE];
    tab_players[WHITE]=tmp;

    void* tmp_handle = tab_handles[0];
    tab_handles[0]=tab_handles[1];
    tab_handles[1]=tmp_handle;
  }
  else{
    printf("%s accepts\n", tab_players[WHITE].get_player_name());
  }
  current_player=&tab_players[BLACK];
  tab_players[BLACK].initialize_color(BLACK);
  tab_players[WHITE].initialize_color(WHITE);
  apply_move(graph, move);

  
  //game loop
  for(int turn = 0; turn<MAX_TURN; turn ++){
    current_player=&tab_players[compute_next_player(move)];
    move = current_player->play(move);
    //printf("%s Choses move %zu\n", current_player->get_player_name(), move.m);
    if(!apply_move(graph, move)){
      winner=!move.c;
      break;
    }
    winner=is_winning(graph, shape);
    if (winner!= NO_COLOR){
      break;
    }
    //print_graph(graph, shape);
  }
  //finalization
  if(winner==NO_COLOR){
    printf("no winner\n");
  }else{
    printf("the winner is %s !\n",tab_players[winner].get_player_name());
  }
  print_graph(graph, shape);
  for(int i=0; i<NB_PLAYERS; i++){
    tab_players[i].finalize();
    dlclose(tab_handles[i]);
  }
  graph__free(graph);
  return 0;
}
