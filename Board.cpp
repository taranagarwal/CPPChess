// Patrick Li - pli50
// Iason Mihalopoulos - imihalo1
// Taran Agarwal - tagarwa4

#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Board::Board(){}


  // Copy constructor
  Board::Board(const Board& b) {
    for (std::map<Position, Piece*>::const_iterator it = b.occ.begin(); it != b.occ.end(); ++it)
      if (it->second)
        add_piece(it->first, it->second->to_ascii());
  }


  // Return the piece at the speccified position on the board
  const Piece* Board::operator()(const Position& position) const {
    if (occ.empty())
      return nullptr;
    
    if (occ.count(position))
      return occ.at(position);
    
    return nullptr;
  }

  
  // Adds new piece with designator at specified position
  void Board::add_piece(const Position& position, const char& piece_designator) {
    Piece *temp = create_piece(piece_designator);
    if (!temp)
      throw Exception("invalid designator");
    delete temp;


    if (!(position.first >= 'A' && position.first <= 'H' && position.second >= '1' && position.second <= '8'))
      throw Exception("invalid position");
    if ((operator()(position)))
      throw Exception("position is occupied");
    occ[position] = create_piece(piece_designator);
  }


  // Attempts to remove piece at specified position
  void Board::remove_piece(const Position& position) {
    if (occ.count(position)) {
      delete occ.at(position);
      occ.at(position) = nullptr;
    }
  }


  // Prints the board
  void Board::display() const {
    std::cout << "  ABCDEFGH" << std::endl;
    for (int i = 8; i >= 1; i--) {
      std::cout << i << " ";
      for (char j = 'A'; j <= 'H'; j++) {
        if (!operator()(Position(j, '0' + i)))
          std::cout << '-';
        else
          std::cout << operator()(Position(j, '0' + i))->to_ascii();
      }
      std::cout << std::endl;
    }
  }

  bool Board::has_valid_kings() const {
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
      it != occ.end(); it++) {
        if (it->second) {
        switch (it->second->to_ascii()) {
        case 'K':
          white_king_count++;
          break;
        case 'k':
          black_king_count++;
          break;
        }
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
        const Piece* piece = board(Position(c, r));
        if (piece) {
          os << piece->to_ascii();
        } else {
          os << '-';
        }
      }
      os << std::endl;
    }
    return os;
  }

}
