// Patrick Li - pli50
// Iason Mihalopoulos - imihalo1
// Taran Agarwal - tagarwa4

#include "Pawn.h"

namespace Chess
{
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
    
    // Check that both positions are on the board
    if (!(start.first >= 'A' && start.first <= 'H' && start.second >= '1' && start.second <= '8'))
      return false;
    if (!(end.first >= 'A' && end.first <= 'H' && end.second >= '1' && end.second <= '8'))
      return false;

    int diff_x = end.first - start.first;
    int diff_y = end.second - start.second;
    if (diff_x != 0)  // Pawn should not move in the x-direction
      return false;
    if (diff_y == 0)
      return false;
    // Check that pawn either moves one or two spaces in the y-direction
    if (to_ascii() == 'P') {
        if (!(diff_y == 2 || diff_y == 1))
            return false;
        else if ((diff_y == 2) && (start.second != '0'+2)) {
            return false;
        }
    }
    else if (to_ascii() == 'p') {
        if (!(diff_y == -2 || diff_y == -1))
            return false;
        else if ((diff_y == -2) && (start.second != '0' + 7)) {
            return false;
        }
    }
    return true;
  }

  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const {

    // Check that both positions are on the board
    if (!(start.first >= 'A' && start.first <= 'H' && start.second >= '1' && start.second <= '8'))
      return false;
    if (!(end.first >= 'A' && end.first <= 'H' && end.second >= '1' && end.second <= '8'))
      return false;

    int diff_y = end.second - start.second;
    int diff_x = end.first - start.first;

    // Check that pawn only moves one space diagonally
    if ((to_ascii() == 'P') && ((diff_y == 1) && (diff_x == 1 || diff_x == -1))) {
      return true;
    }
    else if ((to_ascii() == 'p') && (diff_y == -1 && (diff_x == 1 || diff_x == -1)))
      return true;
    
    return false;
  }
}