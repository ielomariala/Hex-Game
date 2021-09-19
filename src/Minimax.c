#include "player.h"
#include "is_winning.h"

#define FALSE 0
#define TRUE 1

static enum color_t color;
static struct graph_t *graph_game;
static int color_initialized = FALSE;
static int graph_initialized = FALSE;
static int m = 9;
static char shape = 'h';

// ---------------------------------- Helper functions -------------------------------------

/*
* Returns the max between n and z
*/
size_t max(int n, int z){
    return ( (n >= z)? n : z);
}

/*
* Returns the min between n and z
*/
size_t min(int n, int z){
    return ( (n>= z)? z : n);
}

/* Is this position owned?
 * Returns a Bool
 * PARAM:
 * -position : a position between 0 and num_vertices
 */
int is_owned(size_t position){
    return (gsl_spmatrix_get(graph_game->o, 0 , position) || gsl_spmatrix_get(graph_game->o, 1 , position));
}


/* Is this position a graph cote?
 * Returns a Bool
 * PARAM:
 * -i : a position between 0 and num_vertices
 */
int is_cote(size_t i){
    if (shape != 't'){
        int line_length = m+1;
        int vertices = graph_game->num_vertices;
        int index = i%line_length;
        return( i <= (unsigned) m+1 || i >= (unsigned) vertices-(m+2) || index == m || index == 0 );
    }
    return 0;
}


/* Is this position vital for winning (a situation where one tile is surrounded by player's or ennemy's tiles, this tile is necessary for winning in square shaped graphs)
 * Returns a Bool
 * PARAM:
 * -position: a position between 0 and num_vertices
 */
int is_vital_point(size_t position){
    if (shape == 'c'){
        size_t mid = (m%2 == 0)? graph_game->num_vertices/2: (graph_game->num_vertices/2)+(m/2);
        return abs(position - mid) < 3*m && ( (is_owned(position+1) && is_owned(position-1) && is_owned(position - m - 1) && is_owned(position + m + 1)) ||
            (is_owned(position+1) && is_owned(position-1) && is_owned(position - m - 1)) || 
            (is_owned(position+1) && is_owned(position + m + 1) && is_owned(position - m - 1)) ||
            (is_owned(position+1) && is_owned(position + m + 1) && is_owned(position - 1)) || 
            (is_owned(position-1) && is_owned(position + m + 1) && is_owned(position - m - 1)) ||
            (is_owned(position-1) && is_owned(position + m + 1) && is_owned(position+1)) );
    }
    return 0;
}


/* is this position ALREADY two-bridgeD with other tiles from the same player? There are six situations of two bridges. 
 * Returns a Bool
 * PARAM:
 * -i : a position between 0 and num_vertices
 * -c : BLACK or WHITE
 */
int is_two_bridgeD(size_t i, enum color_t c){
    size_t line_length = m+1;
    size_t vertices = graph_game->num_vertices;
    size_t index = i%line_length;
    if (shape == 'h'){
        if ((i+line_length -1 < vertices) && gsl_spmatrix_get(graph_game->o, c , i+1) && (i/line_length < line_length-1 && index > 0) && gsl_spmatrix_get(graph_game->o, c , i+line_length - 1))
            return 1;
        else if (( i+1 >= line_length) && gsl_spmatrix_get(graph_game->o, c , i-1) && gsl_spmatrix_get(graph_game->o, c , i-line_length + 1))
            return 2;
        else if (i+1 < vertices && i>= line_length && gsl_spmatrix_get(graph_game->o, c , i+1) && gsl_spmatrix_get(graph_game->o, c, i-line_length))
            return 3;
        else if (( i >= 1) && gsl_spmatrix_get(graph_game->o, c , i-1) && i+line_length < vertices && gsl_spmatrix_get(graph_game->o, c, i+line_length))
            return 4;
        else if ( i+line_length < vertices && i+1 >= line_length && gsl_spmatrix_get(graph_game->o, c, i+line_length) && i/m > 0 && index < line_length-1 && gsl_spmatrix_get(graph_game->o, c, i-line_length + 1))
            return 5;
        else if ( i+line_length-1 < vertices && gsl_spmatrix_get(graph_game->o, c, i+line_length-1) && i >= line_length && gsl_spmatrix_get(graph_game->o, c, i-line_length))
            return 6;
    }
    return 0;
}


/* Does this position create a two-bridge with other tiles from the same player? There are six situations of two bridges. 
 * Returns a Bool
 * PARAM:
 * -i : a position between 0 and num_vertices
 * -c : BLACK or WHITE
 */
int is_two_bridge(size_t i, enum color_t c){
    size_t line_length = m+1;
    size_t vertices = graph_game->num_vertices;
    //size_t index = i%line_length;
    if (shape == 'h'){
        //printf("Erreur avec i = %zu", i);
        if ( i >1+line_length && !is_cote(i-1-line_length) && gsl_spmatrix_get(graph_game->o, c, i-1-line_length) && !is_owned(i-1) && !is_owned(i-line_length))
            return 1; 
        else if ( i-1+line_length < vertices && !is_cote(i-2+line_length) && gsl_spmatrix_get(graph_game->o, c, i-2+line_length) && !is_owned(i-1) && !is_owned(i+line_length-1))
            return 2;
        else if (i+1+line_length < vertices && !is_cote(i+1+line_length) && gsl_spmatrix_get(graph_game->o, c, i+1+line_length) && !is_owned(i+1) && !is_owned(i+line_length))
            return 3; 
        else if ( i+2 >= line_length && !is_cote(i+2-line_length) && gsl_spmatrix_get(graph_game->o, c, i-line_length+2) && !is_owned(i+1) && !is_owned(i-line_length+1))
            return 4;
        else if ( i >= 2*line_length-1 && gsl_spmatrix_get(graph_game->o, c, i-2*line_length+1) && !is_owned(i-line_length+1) && !is_owned(i-line_length))
            return 5;
        else if (i+2*line_length-1 < vertices && gsl_spmatrix_get(graph_game->o, c, i+2*line_length-1) && !is_owned(i+line_length-1) && !is_owned(i+line_length))
            return 6;
    }
    return 0;
}


/* Is i-f a two-bridge? 
 * Returns a Bool
 * PARAM:
 * -i : a position between 0 and num_vertices
 * -f : a position between 0 and num_vertices
 */
int a_two_bridge(size_t i, size_t f){
    size_t line_length = m+1;
    size_t i_x = i%line_length;
    size_t f_x = f%line_length;
    size_t i_y = i/line_length;
    size_t f_y = f/line_length;
    return ((f_x == i_x -1 && f_y == i_y-1) || (f_x == i_x+1 && f_y == i_y-2) ||
    (f_x == i_x+2 && f_y == i_y-1) || (f_x == i_x-2 && f_y == i_y+1) ||
    (f_x == i_x-1 && f_y == i_y+2) || (f_x == i_x+1 && f_y == i_y+1));
}


/* Does pos belong to ennemy? 
 * Returns a Bool
 * PARAM:
 * -pos : a position between 0 and num_vertices
 */
int is_ennemy(size_t pos){
    return (gsl_spmatrix_get(graph_game->o,!color,pos));
}


/* Does pos belong to our client? 
 * Returns a Bool
 * PARAM:
 * -pos : a position between 0 and num_vertices
 */
int is_mine(size_t pos){
    return (gsl_spmatrix_get(graph_game->o,color,pos));
}


/* Is i the only link in a two-bridge? 
 * Returns a Bool
 * PARAM:
 * -i : a position between 0 and num_vertices
 * -c : Who owns the two-bridge (BLACK or WHITE)
 */
int is_only_link_two_bridge(size_t i, enum color_t c){
    size_t line_length = m+1;
    if (is_owned(i))
        return 0;
    if (shape == 'h'){
        return( (is_two_bridgeD(i, c) == 1 && is_ennemy(i+line_length)) ||
            (is_two_bridgeD(i, c) == 2 && is_ennemy(i-line_length)) ||
            (is_two_bridgeD(i, c) == 3 && is_ennemy(i-line_length+1)) ||
            (is_two_bridgeD(i, c) == 4 && is_ennemy(i+line_length-1)) ||
            (is_two_bridgeD(i, c) == 5 && is_ennemy(i+1)) ||
            (is_two_bridgeD(i, c) == 6 && is_ennemy(i-1)) );
    }
    return 0;
}



/* Does i create a one-bridge with one of the player's tiles? 
 * Returns a Bool
 * PARAM:
 * -i : a position between 0 and num_vertices
 * -c : BLACK or WHITE
 */
int is_one_bridge(size_t i, enum color_t c){
    size_t vertices = graph_game->num_vertices;
    size_t line_length = m+1;
    return ( (i+1<vertices && i >= 1 && gsl_spmatrix_get(graph_game->o, c, i+1) && gsl_spmatrix_get(graph_game->o, c, i-1) )  ||
    (i+line_length<vertices && i >= line_length && gsl_spmatrix_get(graph_game->o, c, i+line_length) && gsl_spmatrix_get(graph_game->o, c, i-line_length) && (!is_cote(i+line_length)||!is_cote(i-line_length)) ) ||
    (i+line_length-1<vertices && i + 1 >= line_length && gsl_spmatrix_get(graph_game->o, c, i+line_length-1) && gsl_spmatrix_get(graph_game->o, c, i-line_length+1) && (!is_cote(i+line_length-1)||!is_cote(i-line_length+1)) ) );
}

// --------------------------------------------- End of Helper Functions ------------------------------------



// --------------------------------------------- Strategy functions --------------------------------------
static size_t last_position_0 = 0; // Last position played by client in direction 0
static size_t last_position_1 = 0; // Last position played by client in direction 1
static size_t direction = 1; // Direction in which minimax is searching for an optimized move


/* Evaluates the position
 * Returns an integer value
 * PARAM:
 * -i : a position between 0 and num_vertices
 */
int evaluation(size_t position,size_t direction){
    size_t line_length = m+1;
    //size_t mid = (m%2 == 0)? graph_game->num_vertices/2: (graph_game->num_vertices/2)+(m/2);
    size_t index = position%line_length;
    size_t y_index = position/line_length; 
    if (is_owned(position)) // Position is owned , value: 0
        return 0;

    else if ((!is_owned(last_position_1) || is_ennemy(last_position_1)) && y_index == line_length/2-1 && index == line_length/2) // Another valuable mid tile if the first one is swapped
        return 150;
    
    else if (shape == 'c' && is_one_bridge(position, !color))
        return 300;
    
    else if (is_only_link_two_bridge(position, color)) // Last link of client's precedently created 2 bridge, very valuable for the client
        return 250;

    else if (is_only_link_two_bridge(position, !color)) // last link of ennemy's precedently created 2 bridge, very valuable for ennemy
        return -250;

    else if (is_two_bridge(position, color) && ((direction == 1 && a_two_bridge(position, last_position_1)) || (direction == 0 && a_two_bridge(position, last_position_0)))  ){ 
        if (color == BLACK ){
            if ((direction == 1 && y_index > (last_position_1/line_length)) || (direction == 0 && y_index < (last_position_0/line_length))){
                int i = (direction == 0) ? abs(y_index - last_position_0/line_length) : abs(y_index - last_position_1/line_length);
                return 130 + i;
            }
        }
        else if (color == WHITE ){
            if (( direction == 0 && abs(y_index - last_position_0/line_length) == 1) || (direction == 1 && abs(y_index - last_position_1/line_length) == 1)){
                if( (direction == 1 && index > last_position_1%line_length ) || (direction == 0 && index < last_position_0%line_length ) ){
                    if ( (direction == 1 && index > (last_position_1%line_length) + 1) || (direction == 0 && index < (last_position_0%line_length) - 1))
                        return 135;
                    int j = (direction == 1) ? abs(index - last_position_1%line_length) : abs(index - last_position_0%line_length);
                    return 130 + j;
                }
            }
        }
    }

    else if ( (color == WHITE && ( (direction == 1 && gsl_spmatrix_get(graph_game->t, last_position_1, position) && index >= last_position_1%line_length) || 
                                (direction == 0 &&  gsl_spmatrix_get(graph_game->t, last_position_0, position) && index <= last_position_0%line_length)) )      || 
        (color == BLACK && ((direction == 1 && gsl_spmatrix_get(graph_game->t, last_position_1, position) && y_index >= last_position_1/line_length) ||
                         (direction == 0 && gsl_spmatrix_get(graph_game->t, last_position_0, position) && y_index <= last_position_0/line_length)) ) ){
        if ((color == WHITE && ( (direction == 1 && gsl_spmatrix_get(graph_game->t, last_position_1, position) && index > last_position_1%line_length ) ||
                                (direction == 0 && gsl_spmatrix_get(graph_game->t, last_position_0, position) && index < last_position_0%line_length)) ) || 
            (color == BLACK && ((direction == 1 && gsl_spmatrix_get(graph_game->t, last_position_1, position) && y_index > last_position_1/line_length && index == last_position_1%line_length) ||
                         (direction == 0 && gsl_spmatrix_get(graph_game->t, last_position_0, position) && y_index < last_position_0/line_length && index == last_position_0%line_length)) ) )
            return 120;
    return 110; 
    }

    else if (is_one_bridge(position, color)){
        if ( (color == BLACK && ((direction == 1 && y_index >= last_position_1/line_length) || (direction == 0 && y_index <= last_position_0/line_length) ) ) || 
            (color == WHITE && ((direction == 1 && index >= last_position_1%line_length) || (direction == 0 && index <= last_position_0%line_length) ) ) ){
            if ((color == BLACK &&  ((direction == 1 && y_index > last_position_1/line_length) || (direction == 0 && y_index < last_position_0/line_length) ) ) || 
                (color == WHITE && ((direction == 1 && index >= last_position_1%line_length) || (direction == 0 && index <= last_position_0%line_length)) ) )
                return 128;
            return 125;
        }
        return 10;
    }

    else if (is_vital_point(position))
        return 5;

    else if (! is_owned(position))
        return 1;
        
    return -1;
}


/* The MiniMax Algorithm is explained in the report
 * Returns a position
 * PARAM:
 * -position : a position, between 0 and num_vertices, whre we begin computing
 * -depth : depth of computing
 * -alpha && beta : two integers, alpha-beta pruning is explained in the report
 * -maximizing : a bool 
 */

size_t minimax(size_t position, size_t depth, int alpha, int beta, int maximizing){
     if (depth == 0 ){ // || is_winning()
            return position;
     }

     if (maximizing){
        int besteval = - 1000;
        size_t bestpos = position;
        for (size_t i=0; i < graph_game->num_vertices; i++){
            if (!is_owned(i)){
                size_t pos = minimax(i, depth - 1, alpha, beta, FALSE);
                int old_besteval = besteval;
                besteval = max(besteval, evaluation(pos, direction));
                if (besteval != old_besteval)
                    bestpos = pos;
                alpha = max(alpha, pos);
                if (beta <= alpha)
                    break;   
            }
        }
        return bestpos;
     }
     else{
        int leasteval = 1000;
        size_t leastpos = position;
        for (size_t i = 0; i < graph_game->num_vertices; i++){
            if (!is_owned(i)){
                int pos = minimax(i, depth - 1, alpha, beta, TRUE);
                int old_leasteval = leasteval;
                leasteval = min(leasteval, evaluation(pos,direction)); 
                if (leasteval != old_leasteval)
                    leastpos = pos; 
                beta = min(beta, pos);
                if (beta <= alpha)
                    break;  
            }
        }
        return leastpos;
     }
 }

// --------------------------------- End of strategy Functions -------------------------------




// ---------------------------------- Necessary client Functions -----------------------------

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name() {
    return "Minimax";
}

/* Returns a single move for an opening
 * PRECOND:
 * - a call to initialize_graph has been made
 * RETURNS:
 * - a correct move inside the graph
 */
struct move_t propose_opening() {
    struct move_t move = {
        .m = (m%2 == 0)? graph_game->num_vertices/2: (graph_game->num_vertices)/2 + (m+1)/2 ,
        .c = BLACK
    };
    last_position_0 = move.m;
    last_position_1 = move.m;
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
    size_t mid = (m%2 == 0)? graph_game->num_vertices/2: (graph_game->num_vertices)/2 + (m+1)/2; 
    last_position_0 = mid;
    last_position_1 = mid;
    if(  opening.m == mid){
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
        printf("ERROR : %s has its graph initialized twice \n", get_player_name());
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
        return;
    }
    printf("ERROR : %s has its color initialized twice", get_player_name());
}


/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player
 * RETURNS:
 * - the next move for the player.
 */
struct move_t play(struct move_t previous_move) {
    gsl_spmatrix_set(graph_game->o, previous_move.c, previous_move.m, TRUE);
    size_t line_length = m+1;
    size_t op_x = previous_move.m%line_length;
    // Adjusting Direction on how the ennemy played, but only if we're the second to play
    if (color == WHITE){
        if (op_x > last_position_1%line_length)
            direction = 1;
        else if (op_x < last_position_0%line_length)
            direction = 0;
    }
    struct move_t move = {.m = 0, .c = color};
    size_t only_link = 0;

    // Linking my two bridges if there is only one valid link
    for(size_t i = 0; i<graph_game->num_vertices; i++){
        if (!is_owned(i) && is_only_link_two_bridge(i, color)){
            move.m = i;
            only_link = 1;
            break;
        }
    }
    // Else Using minimax to find an optimized move
    if (!only_link){
        move.m = minimax(previous_move.m, 1, -1000, 1000, TRUE );
        int eva = evaluation(move.m, direction);
        if (eva >= 100 && eva != 250){
            if (direction)
                last_position_1 = move.m;
            else{ last_position_0 = move.m;}
        }
        if (eva != 150)
            direction = !direction;
    }
    // Proposing valid moves is necessary, no matter what
    if (is_owned(move.m)){
        printf("Minimax : Chosing unoptimized valid move\n");
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
}