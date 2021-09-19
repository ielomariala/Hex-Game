#ifndef _HEX_MOVE_H_
#define _HEX_MOVE_H_

#include <stdlib.h>

/* Players */
enum color_t { BLACK=0, WHITE=1, NO_COLOR=2 };

/* Positions */
struct move_t {
  size_t m;       // An index between 0 and the number of vertices of - 1
  enum color_t c; // The color of the player
};

#endif // _HEX_MOVE_H_

