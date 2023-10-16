// Patrick Li - pli50
// Iason Mihalopoulos - imihalo1
// Taran Agarwal - tagarwa4

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "Piece.h"
#include "Board.h"
#include "Exceptions.h"

namespace Chess
{

	class Game {

	public:
		// This default constructor initializes a board with the standard
		// piece positions, and sets the state to white's turn
		Game();

		Game(const Game& g);

		~Game();

		// Returns true if it is white's turn
		/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		bool turn_white() const { return is_white_turn; }
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    // Displays the game by printing it to stdout
		void display() const { board.display(); }
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    // Checks if the game is valid
		bool is_valid_game() const { return board.has_valid_kings(); }

		// Attempts to make a move. If successful, the move is made and
		// the turn is switched white <-> black. Otherwise, an exception is thrown
		void make_move(const Position& start, const Position& end);

		// Returns true if the designated player is in check
		bool in_check(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_mate(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_stalemate(const bool& white) const;

		// Return the total material point value of the designated player
		int point_value(const bool& white) const;

		// Return the new points from a capture
		int capture(const Position& end);

	private:
		// The board
		Board board;

		// Is it white's turn?
		bool is_white_turn;

		//Point value variables
		int whitePoints;
		int blackPoints;

		bool bishop_is_clear(const Position& start, const Position& end) const;
		bool rook_is_clear(const Position& start, const Position& end) const;
		bool queen_is_clear(const Position& start, const Position& end) const;
		bool pawn_is_clear(const Position& start, const Position& end) const;
		bool mystery_is_clear(const Position& start, const Position& end) const;
		const Position find_k(const bool& white) const;

		// Writes the board out to a stream
		friend std::ostream& operator<< (std::ostream& os, const Game& game);

		// Reads the board in from a stream
		friend std::istream& operator>> (std::istream& is, Game& game);
	};
}
#endif // GAME_H
