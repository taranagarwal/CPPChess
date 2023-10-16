// Patrick Li - pli50
// Iason Mihalopoulos - imihalo1
// Taran Agarwal - tagarwa4

#include "Rook.h"

namespace Chess
{
  bool Rook::legal_move_shape(const Position& start, const Position& end) const {
    // Check that both positions are on the board
    if (!(start.first >= 'A' && start.first <= 'H' && start.second >= '1' && start.second <= '8'))
      return false;
    if (!(end.first >= 'A' && end.first <= 'H' && end.second >= '1' && end.second <= '8'))
      return false;
    // Check that piece is moving vertically or horizontally
    if ((start.first == end.first)||(start.second == end.second))
      return true;
    return false;
  }
}