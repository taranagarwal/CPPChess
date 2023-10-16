// Patrick Li - pli50
// Iason Mihalopoulos - imihalo1
// Taran Agarwal - tagarwa4

#include "Knight.h"

namespace Chess
{
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {

    // Check that both positions are on the board
    if (!(start.first >= 'A' && start.first <= 'H' && start.second >= '1' && start.second <= '8'))
      return false;
    if (!(end.first >= 'A' && end.first <= 'H' && end.second >= '1' && end.second <= '8'))
      return false;

    int diff_x = end.first - start.first;
    int diff_y = end.second - start.second;

    // Check that knght moves in an L-shape
    if ((diff_x == 1 || diff_x == -1) && (diff_y == 2 || diff_y == -2))
      return true;
    if ((diff_x == 2 || diff_x == -2) && (diff_y == 1 || diff_y == -1)) 
      return true;
    
    return false;
  }
}