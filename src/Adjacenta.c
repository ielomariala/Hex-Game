
#include "is_winning.h"

#include <stdio.h>
#include "player.h"
#include "graph.h"
#include "graph_struct.h"
#include "move.h"

#define FALSE 0
#define TRUE 1

static enum color_t color;
static struct graph_t *graph_game;
static int color_initialized = FALSE;
static int graph_initialized = FALSE;
static int m = 9;
static char shape = 'h';


static size_t last_played_0 = 0; // Mémorisation du dernier coup à la direction 0
static size_t last_played_1 = 0; // Mémorisation du dernier coup à la direction 0
static size_t direction = 1; // Mémorisation de la dernière direction 

enum color_t reverse_color(enum color_t color) {
    return !color;
}

/* Is this position owned?
 * Returns a Bool
 * PARAM:
 * -position : a position between 0 and num_vertices
 */
int is_owned(size_t position){
    return (gsl_spmatrix_get(graph_game->o, 0 , position) || gsl_spmatrix_get(graph_game->o, 1 , position));
}

/* Counters ennemy one bridges
 * RETURNS:
 * - A move to counter ennemy's one bridge
 */
size_t previous_move_is_danger(struct move_t p){
    //int line_length = m+1;
    int vertices = graph_game->num_vertices;
    //int index = p.m%line_length;
    if (shape == 'h' || shape == 'c'){
        if (p.m+2 < (unsigned) vertices && gsl_spmatrix_get(graph_game->o, p.c, p.m+2) && !is_owned(p.m+1))
            return p.m+1;
        else if (p.m + 2*(m + 1)< (unsigned) vertices && gsl_spmatrix_get(graph_game->o, p.c, p.m + 2*(m + 1)) && !is_owned(p.m+m+1))
            return p.m+m+ 1;
        else if (p.m >= (unsigned) 2*(m + 1) && gsl_spmatrix_get(graph_game->o, p.c, p.m-2) && !is_owned(p.m-1))
            return p.m-1;
        else if (p.m >= (unsigned) 2*(m + 1) && gsl_spmatrix_get(graph_game->o, p.c, p.m-2*(m + 1)) && !is_owned(p.m-m-1))
            return p.m-m- 1;
    }
    return 0;
}

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name() {
    return "Adjacenta";
}

/* Returns a single move for an opening
 * PRECOND:
 * - a call to initialize_graph has been made
 * RETURNS:
 * - a correct move inside the graph
 */
struct move_t propose_opening() {
    struct move_t move = {
        .m = ( m%2 == 0  ) ? (graph_game->num_vertices)/2 : (graph_game->num_vertices)/2 + (m+1)/2,
        .c = BLACK
    };
    last_played_0 = move.m;
    last_played_1 = move.m;
    gsl_spmatrix_set(graph_game->o, BLACK, move.m, TRUE);
    return move;
}

/* Acceptation of the opening
 * PARAM:
 * - opening : a move inside the graph
 * PRECOND:
 * - a call to initialize_graph has been made
 * - the move is valid with regard to this graph
 * RETURNS:
 * - a boolean telling if the player accepts the opening
 */
int accept_opening(const struct move_t opening) {
    gsl_spmatrix_set(graph_game->o, BLACK, opening.m, TRUE);
    if (opening.m == (unsigned) (graph_game->num_vertices)/2){
        last_played_1 = (graph_game->num_vertices)/2;
        last_played_0 = (graph_game->num_vertices)/2;
        return FALSE;
    }
    return TRUE;
}

/* Player graph initialization
 * PARAM:
 * - graph : the graph where the game is played
 * PRECOND:
 * - `graph` is a heap-allocated copy of the graph where the game is
 *   played, that must be freed in the end
 * - initialize_graph has never been called before
 */
void initialize_graph(struct graph_t * graph) {
    if (!graph_initialized) {
        size_t count = 0;
        graph_game = graph;
        graph_initialized = TRUE;
        for (size_t i = 0; i < graph_game->num_vertices; i++){
            if (gsl_spmatrix_get(graph_game->t, 1, i))
                count++;
        }
        if (count == 3){
            shape = 'c';
            m = sqrt(graph_game->num_vertices) - 1;
        }
        else if (count == 4){
            shape = 'h';
            m = sqrt(graph_game->num_vertices) - 1;
        }
        else if (count == 2){
            shape = 't';
            m = sqrt(graph_game->num_vertices/6);
        }
    }
    else {
        printf("ERROR : %s has its graph initialized 2 times \n", get_player_name());
    }
}

/* Player color initialization
 * PARAM:
 * - id : the color assigned to the player
 * PRECOND:
 * - id is either BLACK or WHITE
 * - initialize_color has never been called before
 */
void initialize_color(enum color_t id) {
    if(!color_initialized) {
        color = id;
        color_initialized = TRUE;
        return;
    }
    printf("ERROR : %s has its color initialized 2 times", get_player_name());
}




/* Choses an adjecant optimized position (This function can be optimized)
 * Returns a position
 * PARAM:
 * -g: the game graph
 * -bool: If we don't find an optimized pos in one direction, we search the other. After that, we stop. This is bool's role.
 * -valid : Memorizes a valid (unoptimized) move to use in case of not finding an otimized one.
 */
size_t chose_adjacence (struct graph_t *g, int bool, size_t valid){
    size_t k = valid, i, j, last_hope = valid;
    size_t line_length = m+1;
    size_t j_index , last_index;

            if (color == BLACK){
                if (direction == 1){
                    i = last_played_1;
                    for (j = i ; j < (unsigned) graph_game->num_vertices; j++){
                        j_index = j/line_length;
                        last_index = last_played_1/line_length; 
                        if (j < (unsigned) graph_game->num_vertices && gsl_spmatrix_get(g->t, i, j) && ! is_owned(j)){
                            last_hope = j;
                            if (!k && !bool && j_index >= last_index)
                                k = j;
                            if (j_index > last_index){
                                last_played_1 = j;
                                return j;
                            }
                        }
                    }
                }
                else {
                    i = last_played_0;
                    for (j = i; j > line_length; j--){ 
                        j_index = j/line_length;
                        last_index = last_played_0/line_length; 
                        if (j < (unsigned) graph_game->num_vertices && gsl_spmatrix_get(g->t, i, j) && ! is_owned(j)){
                            last_hope = j;
                            if (!k && !bool && j_index <= last_index)
                                k = j;
                            if (j_index < last_index){
                                last_played_0 = j; 
                                return j;
                            }
                        } 
                    }
                }
            }
            if (color == WHITE){ // Player is WHITE
                if (direction == 1){ // going in direction 1
                    i = last_played_1;
                    for (j = i ; j < (unsigned) graph_game->num_vertices; j++){
                        j_index = j%line_length;
                        last_index = last_played_1%line_length; 
                        if (j < (unsigned) graph_game->num_vertices && gsl_spmatrix_get(g->t, i, j) && ! is_owned(j)){
                            last_hope = j;
                            if (!k && !bool && j_index >= last_index)
                                k = j;
                            if (j_index > last_index){
                                last_played_1 = j; 
                                return j;
                            }
                        }
                    }
                }
                else { // going in direction 0
                     i = last_played_0;
                    for (j = i ; j > line_length ; j--){
                        j_index = j%line_length;
                        last_index = last_played_0%line_length; 
                        if (j < (unsigned) graph_game->num_vertices && gsl_spmatrix_get(g->t, i, j) && ! is_owned(j)){
                            last_hope = j;
                            if (!k && !bool && j_index <= last_index )
                                k = j;
                            if (j_index < last_index){
                                last_played_0 = j; 
                                return j;
                            }
                        }
                    }
                }
            }
            if ( bool == 0 ){ // In case of not finding an optimized move in one direction, we search in the other
                direction = !direction;
                return chose_adjacence(g, bool +1, k);
            }
            else { 
                direction = !direction;
            }
    // Updating last move in directions
    if (direction == 1)
        last_played_1 = k;
    else last_played_0 = k;
    // We haven't find an optimized move, we've probably memorized 2 valid moves through out the algorithm 'k' and 'last_hope'
    if (!is_owned(k))
        return k; 
    return last_hope;
}


/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player
 * RETURNS:
 * - the next move for the player.
 */
struct move_t play(struct move_t previous_move) {
    gsl_spmatrix_set(graph_game->o, previous_move.c, previous_move.m, TRUE);
    struct move_t move;
    move.c = color;
    size_t s = ( m%2 == 0  ) ? (graph_game->num_vertices)/2 : (graph_game->num_vertices)/2 + (m+1)/2;
    if (!is_owned(s)){ // Focuses on the mid move first
        last_played_1 = s;
        last_played_0 = s;
        move.m = s;
    }
    else{ // Then choses a move "chose_adjacence" and changing direction
        size_t k = previous_move_is_danger(previous_move);
        s = (k != 0 ) ? k : chose_adjacence(graph_game, 0, 0);
        move.m = s;
        direction = !direction;
    }
    // Proposing valid moves is essential
    if (is_owned(s)){
        for(size_t i = 0; i<graph_game->num_vertices; i++){
            if (!is_owned(i)){
                move.m = i;
            }
        }
    }
    gsl_spmatrix_set(graph_game->o, color, move.m, TRUE);
    return move;
}

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize() {
    gsl_spmatrix_free(graph_game->t);
    gsl_spmatrix_free(graph_game->o);
    free(graph_game);
};
