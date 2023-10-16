// Patrick Li - pli50
// Iason Mihalopoulos - imihalo1
// Taran Agarwal - tagarwa4

#include "Bishop.h"

namespace Chess
{
  bool Bishop::legal_move_shape(const Position& start, const Position& end) const {

    // Check that both positions are on the board
    if (!(start.first >= 'A' && start.first <= 'H' && start.second >= '1' && start.second <= '8'))
      return false;
    if (!(end.first >= 'A' && end.first <= 'H' && end.second >= '1' && end.second <= '8'))
      return false;

    int diff_x, diff_y;
    diff_y = end.second - start.second;
    diff_x = end.first - start.first;

    // Check that piece is moving along diagonal
    if ((diff_x + diff_y == 0) || (diff_x - diff_y == 0)) {
      return true;
    }
    return false;
  }
}
