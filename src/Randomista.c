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
static int m = 8;
static char shape = 'h';


enum color_t reverse_color(enum color_t color) {
    return !color;
}

/* Is this position owned?
 * Returns a Bool
 * PARAM:
 * -position : a position between 0 and num_vertices
 */
int is_owned(size_t position){
    return (gsl_spmatrix_get(graph_game->o, 0 , position) == TRUE || gsl_spmatrix_get(graph_game->o, 1 , position) == TRUE);
}


/* Choses a random valid move
 * Returns a position
 * PARAM:
 * -g : the game graph
 */
size_t chose_random_but_valid(struct graph_t *g){
    size_t i = rand()%(g->num_vertices - m -2);
    if (is_owned(i)){
        for (size_t j = 0; j < i; j++){
            if ( ! is_owned(j))
                return j;
        }
        for(size_t j = g->num_vertices - m - 3; j > i; j--){
            if (! is_owned(j))
                return j;
        }
    }
    return i;
}



/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name() {
    return "Randomista";
}


/* Returns a single move for an opening
 * PRECOND:
 * - a call to initialize_graph has been made
 * RETURNS:
 * - a correct move inside the graph
 */
struct move_t propose_opening() {
    struct move_t move = {
        .m = chose_random_but_valid(graph_game),
        .c = BLACK
    };
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
    size_t count = 0;
    if (!graph_initialized) {
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

/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player
 * RETURNS:
 * - the next move for the player.
 */
struct move_t play(struct move_t previous_move) {
    gsl_spmatrix_set(graph_game->o, previous_move.c, previous_move.m, TRUE);
    struct move_t move = {
        .m = chose_random_but_valid(graph_game),
        .c = color
    };

    // Proposing valid moves is necessary
    if (is_owned(move.m)){
        printf("Randomista : Chosing unoptimized valid move\n");
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
