// Patrick Li - pli50
// Iason Mihalopoulos - imihalo1
// Taran Agarwal - tagarwa4

#include "King.h"

namespace Chess
{
  bool King::legal_move_shape(const Position& start, const Position& end) const {

    // Check that both positions are on the board
    if (!(start.first >= 'A' && start.first <= 'H' && start.second >= '1' && start.second <= '8'))
      return false;
    if (!(end.first >= 'A' && end.first <= 'H' && end.second >= '1' && end.second <= '8'))
      return false;

    int diff_x = end.first - start.first;
    int diff_y = end.second - start.second;

    // Check that king only moves one space
    if(diff_x == 1 || diff_x == -1){
         if(diff_y == 0 || diff_y == 1 || diff_y == -1)
             return true;
     }
     if(diff_y == 1 || diff_y == -1){
         if(diff_x == 0 || diff_x == 1 || diff_x == -1)
             return true;
     }
    return false;
  }
}
