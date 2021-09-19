#include <stdio.h>
#include <assert.h>
#include "is_winning.h"
#include "player.h"
#include "graph.h"
#include "graph_struct.h"
#include "move.h"
#include "jam_strategy.h"
#include "compo_connexes.h"

#define FALSE 0
#define TRUE 1

static enum color_t color;
static struct graph_t *graph_game;
static int color_initialized = FALSE;
static int graph_initialized = FALSE;

int cc_edge_vertex_marker_black[2];
int cc_edge_vertex_marker_white[2];
int **tab_cc_edge;

static int alternate_side = 0;

enum color_t reverse_color(enum color_t color)
{
    return !color;
}

size_t alternate_position(size_t prof_max)
{
    size_t middle = (prof_max / 2) + 1;
    alternate_side = !alternate_side;

    return alternate_side ? (middle / 2) + 1 : (middle + prof_max) / 2;
}

int absolute_value(int a) {
    return a < 0 ? -1 * a : a;
}

size_t closest_to_opponent_line(struct Queue * q, size_t opponent_move) {
    int min = dequeue(q);
    int min_distance = graph_game->num_vertices;

    while(!queue__is_empty(q)) {
        int x = dequeue(q);
        int distance = absolute_value(opponent_move - x); 
        if(distance < min_distance) {
            min = x;
            min_distance = distance; 
        }
    }
    return min;
}

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const *get_player_name()
{
    return "TrustMeRushB";
}

struct move_t get_random_move(enum color_t color)
{
    size_t i;
    do
    {
        i = rand() % (graph_game->num_vertices);
    } while (get_color_size_t(graph_game, i) != -1);
    struct move_t move = {
        .m = i,
        .c = color};
    return move;
}

/* Returns a single move for an opening
 * PRECOND:
 * - a call to initialize_graph has been made
 * RETURNS:
 * - a correct move inside the graph
 */
struct move_t propose_opening()
{
    struct move_t move = get_random_move(BLACK);

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
int accept_opening(const struct move_t opening)
{
    gsl_spmatrix_set(graph_game->o, BLACK, opening.m, TRUE);

    return FALSE; // We rush, we always want to have the first move
}

/* Player graph initialization
 * PARAM:
 * - graph : the graph where the game is played
 * PRECOND:
 * - `graph` is a heap-allocated copy of the graph where the game is
 *   played, that must be freed in the end
 * - initialize_graph has never been called before
 */
void initialize_graph(struct graph_t *graph)
{
    if (!graph_initialized)
    {
        graph_game = graph;
        graph_initialized = TRUE;
        tab_cc_edge = malloc(sizeof(int *) * 2);
        tab_cc_edge[BLACK] = cc_edge_vertex_marker_black;
        tab_cc_edge[WHITE] = cc_edge_vertex_marker_white;

        struct set *s_black = ignite_last_hope(graph, BLACK);
        struct set *s_white = ignite_last_hope(graph, WHITE);

        if (set__size(s_black) == 2 && set__size(s_white) == 2)
        {
            struct set *set_tmp = ((struct set *)set__get_element(s_white, 0));
            cc_edge_vertex_marker_white[0] = *((int *)set__get_element(set_tmp, 0));
            set_tmp = ((struct set *)set__get_element(s_white, 1));
            cc_edge_vertex_marker_white[1] = *((int *)set__get_element(set_tmp, 1));
            set_tmp = ((struct set *)set__get_element(s_black, 0));
            cc_edge_vertex_marker_black[0] = *((int *)set__get_element(set_tmp, 0));
            set_tmp = ((struct set *)set__get_element(s_black, 1));
            cc_edge_vertex_marker_black[1] = *((int *)set__get_element(set_tmp, 1));
        }

        else
        {
            printf("Error : board has not 2 edges of the same color");
        }

        set__free(s_black);
        set__free(s_white);
    }
    else
    {
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
void initialize_color(enum color_t id)
{
    if (!color_initialized)
    {
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
struct move_t play(struct move_t previous_move)
{
    assert(tab_cc_edge != NULL);
    gsl_spmatrix_set(graph_game->o, previous_move.c, previous_move.m, TRUE);

    // parcours en largeur

    struct set *cc_list = ignite_last_hope(graph_game, color);
    const struct set *cc_begin = find__cc(cc_list, tab_cc_edge[color][0]);
    assert(cc_begin != NULL);
    if (cc_begin == NULL)
    {
        printf("error : %d \n", tab_cc_edge[color][0]);
    }
    int v_end = tab_cc_edge[color][1];

    // Parcourir et comparer les longueurs
    size_t min_prof = graph_game->num_vertices;
    size_t prof;
    struct Queue *min_queue = empty__queue();
    int min_queue_init = 0;

    for (size_t i = 0; i < set__size(cc_begin); ++i)
    {
        struct Queue *q = empty__queue();
        prof = parcours_largeur(*((int *)set__get_element(cc_begin, i)), v_end, cc_list, graph_game, color, q);
        if (prof < min_prof && prof > 0)
        {
            min_prof = prof;
            min_queue_init = 1;

            while (!queue__is_empty(min_queue))
            {
                dequeue(min_queue);
            }
            while (!queue__is_empty(q))
            {
                enqueue(min_queue, dequeue(q));
            }
        }
        queue__free(q);
    }

    // Remplir move
    struct move_t move;
    if (min_queue_init && !queue__is_empty(min_queue))
    {
        move.c = color;
        move.m = closest_to_opponent_line(min_queue, previous_move.m);
        /*for (size_t i = 0; i < alternate_position(min_prof); ++i)
        {
            move.m = dequeue(min_queue);
        }*/
    }
    else
    {
        move = get_random_move(color);
    }

    set__free(cc_list);
    queue__free(min_queue);

    gsl_spmatrix_set(graph_game->o, move.c, move.m, TRUE);

    return move;
}

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize()
{
    free(tab_cc_edge);
    graph__free(graph_game);
};
