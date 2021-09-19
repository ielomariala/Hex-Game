#ifndef _HEX_PLAYER_H_
#define _HEX_PLAYER_H_

#include <stdlib.h>

#include "move.h"
#include "graph.h"

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name();

/* Returns a single move for an opening
 * PRECOND:
 * - a call to initialize_graph has been made
 * RETURNS:
 * - a correct move inside the graph
 */
struct move_t propose_opening();

/* Acceptation of the opening
 * PARAM:
 * - opening : a move inside the graph
 * PRECOND:
 * - a call to initialize_graph has been made
 * - the move is valid with regard to this graph
 * RETURNS:
 * - a boolean telling if the player accepts the opening
 */
int accept_opening(const struct move_t opening);

/* Player graph initialization
 * PARAM:
 * - graph : the graph where the game is played
 * PRECOND:
 * - `graph` is a heap-allocated copy of the graph where the game is
 *   played, that must be freed in the end
 * - initialize_graph has never been called before
 */
void initialize_graph(struct graph_t* graph);

/* Player color initialization
 * PARAM:
 * - id : the color assigned to the player
 * PRECOND:
 * - id is either BLACK or WHITE
 * - initialize_color has never been called before
 */
void initialize_color(enum color_t id);

/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player
 * RETURNS:
 * - the next move for the player.
 */
struct move_t play(struct move_t previous_move);

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize();

#endif // _HEX_PLAYER_H_

