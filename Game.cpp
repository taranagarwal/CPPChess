// Patrick Li - pli50
// Iason Mihalopoulos - imihalo1
// Taran Agarwal - tagarwa4

#include <cassert>
#include <cstddef>
#include "Exceptions.h"
#include "Game.h"
#include "Piece.h"

namespace Chess
{
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Game::Game() : is_white_turn(true) {
		// Add the pawns
		for (int i = 0; i < 8; i++) {
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
		board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

		// Add the knights
		board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
		board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

		// Add the bishops
		board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );

		board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
		board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

		// Add the kings and queens
		board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
		board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
		board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
		board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
	}

	// Copy constructor
	Game::Game(const Game& g) : board(g.board), is_white_turn(g.is_white_turn), whitePoints(g.whitePoints), blackPoints(g.blackPoints) {}

	// Destructor
	Game::~Game() {
	  for (int i = 8; i >= 1; i--) {
			for (char j = 'A'; j <= 'H'; j++) {
				board.remove_piece(Position(j, '0' + i));	// Remove every piece at every position
			}
		}
	}


	// Attempts to move piecce from start to end
	void Game::make_move(const Position& start, const Position& end) {

		// Check for errors
		if (!(start.first >= 'A' && start.first <= 'H' && start.second >= '1' && start.second <= '8'))	// If start position is not on board, throw exception
			throw Exception("start position is not on board");
		if (!(end.first >= 'A' && end.first <= 'H' && end.second >= '1' && end.second <= '8'))			// If end position is not on board, throw exception
			throw Exception("end position is not on board");
		if (!board(start))																				// If no piece at start position, throw exception
			throw Exception("no piece at start position");
		if (is_white_turn && !(board(start)->to_ascii() >= 'A' && board(start)->to_ascii() <= 'Z'))		// If piece color and turn do not match, throw exception
			throw Exception("piece color and turn do not match");
		if (!is_white_turn && !(board(start)->to_ascii() >= 'a' && board(start)->to_ascii() <= 'z'))
			throw Exception("piece color and turn do not match");
		if (!board(start)->legal_move_shape(start, end) && !((board(end) && board(start)->legal_capture_shape(start, end)))) 	
		// If not a legal move shape, return false. Make sure this isn't a capturing pawn
			throw Exception("illegal move shape");

		// Check that path is clear, functions will also check whether start and end constitute a legal move
		if ((board(start)->to_ascii() == 'P' || board(start)->to_ascii() == 'p') && !pawn_is_clear(start, end))	// pawn
			throw Exception("path is not clear");
		if ((board(start)->to_ascii() == 'R' || board(start)->to_ascii() == 'r') && !rook_is_clear(start, end))	// rook
			throw Exception("path is not clear");
		if ((board(start)->to_ascii() == 'B' || board(start)->to_ascii() == 'b') && !bishop_is_clear(start, end))	// bishop
			throw Exception("path is not clear");
		if ((board(start)->to_ascii() == 'Q' || board(start)->to_ascii() == 'q') && !queen_is_clear(start, end))	// queen
			throw Exception("path is not clear");
		if ((board(start)->to_ascii() == 'M' || board(start)->to_ascii() == 'm') && !mystery_is_clear(start, end))	// mystery
			throw Exception("path is not clear");
		if ((board(start)->to_ascii() == 'N' || board(start)->to_ascii() == 'n') && !board(start)->legal_move_shape(start, end))	// knight
			throw Exception("path is not clear");
		if ((board(start)->to_ascii() == 'K' || board(start)->to_ascii() == 'k') && !board(start)->legal_move_shape(start, end))	// only check legal move shape for king 
			throw Exception("illegal move shape");

		// Case if we're capturing a piece
		if (board(end)) {
			// Make sure we can capture the piece		
			if ((board(start) && board(start)->is_white() && board(end) && board(end)->is_white()) 
			|| (!board(start)->is_white() && !board(end)->is_white())) // If start and end piece are same color, throw exception
				throw Exception("cannot capture own piece");

			// Pawn captures a piece
			if ((board(start)->to_ascii() == 'P' || board(start)->to_ascii() == 'p') 
			&& !(board(start)->legal_capture_shape(start, end)))	// If pawn doesn't have legal capture shape, throw exception
				throw Exception("illegal capture shape");

			// Non-pawn captures a piece
			else if (!(board(start)->to_ascii() == 'P' || board(start)->to_ascii() == 'p')
			&& !board(start)->legal_move_shape(start, end))	// If non-pawn piece doesn't have a legal move shape, throw exception
				throw Exception("illegal move shape");
			
			board.remove_piece(end);	// Remove captured piece from the board
		}

		// Check whether this move exposes check
		bool check_initial = in_check(is_white_turn);			// Stores whether in check before the move
		Game g = *this;											// Create a copy of the game
		g.board.add_piece(end, g.board(start)->to_ascii());		// Add piece to game copy and remove old piece from copy
		g.board.remove_piece(start);
		// Promote pawn to a Queen if at at end of board in the game copy
		if ((g.board(end)->to_ascii() == 'P' && end.second == '0' + 8) || (g.board(end)->to_ascii() == 'p' && end.second == '0' + 1)) {
			g.board.remove_piece(end);
			g.board.add_piece(end, is_white_turn ? 'Q' : 'q');
		}
		bool check_after = g.in_check(is_white_turn); 			// Stores whether in check after the move
		if ((!check_initial && check_after) || (check_initial && check_after))	// If in check after move, throw exception
			throw Exception("move exposes check");		
			
		// "Move" piece by adding the piece to the end position and removing from start position (in the actual board)
		board.add_piece(end, board(start)->to_ascii());
		board.remove_piece(start);

		// Promote pawn to a Queen if at at end of board
		if ((board(end)->to_ascii() == 'P' && end.second == '0' + 8) || (board(end)->to_ascii() == 'p' && end.second == '0' + 1)) {
			board.remove_piece(end);
			board.add_piece(end, is_white_turn ? 'Q' : 'q');
		}

		// Change turns
		if (is_white_turn)
			is_white_turn = false;
		else is_white_turn = true;
	}

	// Returns whether pawn can move from start to end or capture a piece at end
	bool Game::pawn_is_clear(const Position& start, const Position& end) const {
   	 int diff_y = end.second - start.second;

		// Check if pawn is moving one or two spaces
		if (board(start)->legal_move_shape(start, end)) {				// If pawn can make the specified movement
			if (diff_y == 2) {											// If pawn moves two spaces, check whether path is clear
				Position temp = Position(start.first, start.second);	// Create a temp position in between start and end
				temp.second++;
				if (board(temp)) return false;							// If occupied, return false
			}
			else if (diff_y == -2) {									// Same process if pawn is black
				Position temp = Position(start.first, start.second);
				temp.second--;
				if (board(temp)) return false;
			}
			return true;
		}

		// Check if pawn is moving diagonally to capture
		if (board(start)->legal_capture_shape(start, end) && board(end))
			return true;

		return false;
	}

	// Returns whether the rook can make the specified movement and that the path is clear
	bool Game::rook_is_clear(const Position& start, const Position& end) const {
		if (!board(start)->legal_move_shape(start, end)) // If not a legal move shape, return false
			return false;

    	int diff_y = end.second - start.second; // Variables to store x and y movement
		int diff_x = end.first - start.first;

		// For all four directions check whether every positions in between start and end are clear
		// check along right direction
		if (diff_x > 0) {
			for (char i = start.first + 1; i < end.first; ++i) {	
				if (board(Position(i, start.second)))
					return false;
			}
		}

		// check along left direction
		else if (diff_x < 0) {
			for (char i = start.first - 1; i > end.first; --i) {	
				if (board(Position(i, start.second)))
					return false;
			}
		}

		// check along up direction
		else if (diff_y > 0) {
			for (int i = start.second + 1; i < end.second; ++i) {	
				if (board(Position(start.first, i)))
					return false;
			}
		}

		// check along down direction
		else if (diff_y < 0) {
			for (int i = start.second - 1; i > end.second; --i) {	
				if (board(Position(start.first, i)))
					return false;
			}
		}
		return true;	// If legal move shape and path is clear, return true
	}

	// Returns whether the bishop can make the specified movement and that the path is clear
	bool Game::bishop_is_clear(const Position& start, const Position& end) const {
		if (!board(start)->legal_move_shape(start, end))	// If not a legal move shape, return false
			return false;

    	int diff_y = end.second - start.second;	// Variables to store x and y movement
		int diff_x = end.first - start.first;

		// check along lower right diagonal
		if (diff_x > 0 && diff_y < 0) {		
			char j = start.second - 1;
			for (char i = start.first + 1; i < end.first; ++i) {
				if (board(Position(i, j)))
					return false;
				j--;
			}
		}

		// check along upper right diagonal
		if (diff_x > 0 && diff_y > 0) {
			char j = start.second + 1;
			for (char i = start.first + 1; i < end.first; ++i) {
				if (board(Position(i, j)))
					return false;
				j++;
			}
		}

		// check along lower left diagonal
		if (diff_x < 0 && diff_y < 0) {
			char j = start.second - 1;
			for (char i = start.first - 1; i > end.first; --i) {
				if (board(Position(i, j)))
					return false;
				j--;
			}
		}

		// check along upper left diagonal
		if (diff_x < 0 && diff_y > 0) {		
			char j = start.second + 1;
			for (char i = start.first - 1; i > end.first; --i) {
				if (board(Position(i, j)))
					return false;
				j++;
			}
		}

		return true;	// If legal move shape and path is clear, return true
	}

	// Returns whether the Queen can make the specified movement and that the path is clear
	bool Game::queen_is_clear(const Position& start, const Position& end) const {
		if (!board(start)->legal_move_shape(start, end))	// If not a legal move shape, return false
			return false;

    	int diff_y = end.second - start.second;
		int diff_x = end.first - start.first;

		if (diff_y + diff_x == 0 || diff_y - diff_x == 0)	// If queen moves in a diagonal, treat it as a bishop, otherwise a rook
			return bishop_is_clear(start, end);
		else return rook_is_clear(start, end);
	}

	// Returns whether the Mystery piece can make the specified movement and that the path is clear
	bool Game::mystery_is_clear(const Position& start, const Position& end) const {
		if (!board(start)->legal_move_shape(start, end))	// If not a legal move shape, return false
			return false;

    	int diff_y = end.second - start.second;
		int diff_x = end.first - start.first;

		if (diff_y + diff_x == 0 || diff_y - diff_x == 0)	// If moves in a diagonal, treat as a bishop
			return bishop_is_clear(start, end);
		else if (diff_y == 0 || diff_x == 0)				// If moves in straight lines, treat as a rook
			return rook_is_clear(start, end);
		else return true;									// Otherwise, no need to check whether path is clear
	}

	// Helper function that returns the position of own king
	const Position Game::find_k(const bool& white) const {
		Position kingPos;								// Initialize Position variable
		for (int i = 8; i >= 1; i--) {					// Iterate through board
			for (char j = 'A'; j <= 'H'; j++) {
				Position pos = Position(j, '0' + i);
				const Piece* piece = board(Position(j, '0' + i));
			
				if (piece && ((piece->to_ascii() == 'K' && white) || (piece->to_ascii() == 'k' && !white))) {
					kingPos = pos;
					return kingPos;
				}
			}
		}
		return kingPos;
	}

	// Checks whether current player is in check
	bool Game::in_check(const bool& white) const {		
	  const Position& kPos = find_k(white);			// Variable kPos holds king'ss position

 	  for (int i = 8; i >= 1; i--) {				// Iterate through board
			for (char j = 'A'; j <= 'H'; j++) {
				const Position& pos = Position(j, '0' + i);
				const Piece* piece = board(pos);
				// Check whether all opposing pieces can capture the king, if so, return true
				if (piece && (piece->is_white() != white)) {
					if (((white && piece->to_ascii() == 'p') || (!white && piece->to_ascii() == 'P')) 
					&& pawn_is_clear(pos, kPos) && piece->legal_capture_shape(pos, kPos))
						return true;
					if (((white && piece->to_ascii() == 'r') || (!white && piece->to_ascii() == 'R')) 
					&& rook_is_clear(pos, kPos))
						return true;
					if (((white && piece->to_ascii() == 'b') || (!white && piece->to_ascii() == 'B'))
					&& bishop_is_clear(pos, kPos))
						return true;
					if (((white && piece->to_ascii() == 'q') || (!white && piece->to_ascii() == 'Q')) 
					&& queen_is_clear(pos, kPos))
						return true;
					if (((white && piece->to_ascii() == 'm') || (!white && piece->to_ascii() == 'M')) 
					&& mystery_is_clear(pos, kPos))
						return true;
					if (((white && piece->to_ascii() == 'n') || (!white && piece->to_ascii() == 'N')) 
					&& board(pos)->legal_move_shape(pos, kPos))
						return true;
					if (((white && piece->to_ascii() == 'k') || (!white && piece->to_ascii() == 'K')) 
					&& board(pos)->legal_move_shape(pos, kPos))
						return true;
				}	
			}
	  }
	  return false;		// If no opposing piece can capture king, return false
	}
	
	
	// Check if in checkmate
	bool Game::in_mate(const bool& white) const {
		if (this->in_check(white) && this->in_stalemate(white)) return true;	// If both in check and in stalemate, return true
		else return false;
	}

	// Check if in stalemate
	bool Game::in_stalemate(const bool& white) const {
		// Check if any of our pieces can make a legal move
 		for (int i = 8; i >= 1; i--) {
			for (char j = 'A'; j <= 'H'; j++) {
				const Position& startPos = Position(j, '0' + i);

				for (int k = 8; k >= 1; k--) {
					for (char o = 'A'; o <= 'H'; o++) {
						const Position& endPos = Position(o, '0' + k);
						Game g = *this;
						if (board(startPos) && g.board(startPos)->is_white() == white) {
							try {
								g.make_move(startPos, endPos);
								return false;
							}
							catch (Chess::Exception &e) {	}
						}
					}
				}
			}
		} 
		return true;
	}

    
	// Return the total material point value of the designated player
	int Game::point_value(const bool& white) const {
		int points = 0;
		for (int i = 8; i >= 1; i--) {		// Iterate through board
			for (char j = 'A'; j <= 'H'; j++) {
				Position pos = Position(j, '0' + i);
				const Piece* piece = board(pos);
				if (piece &&((white && piece->is_white()) || (!white && !piece->is_white()))) {	
					points += piece->point_value();		// Add up the piece point values for all the pieces of the current player's color
				}
			}
		}
		return points;
	}

	// reads in board from stream
	std::istream& operator>> (std::istream& is, Game& game) {
		if (!is)
			throw Exception("Cannot load the game!");

		std::string line;
		int i = 8;

		while (std::getline(is, line)) {
			if (line.length() == 8) {
				for (char j = 'A'; j <= 'H'; j++) {
					Position pos = Position(j, '0' + i);
					char piece = line.at(j-65);
					
					game.board.remove_piece(pos);
					if (piece != '-') 	
						game.board.add_piece(pos, piece);
				}
				i--;
			}
			else if (i == 0 && line.length() == 1) {
				if (line == "w")
					game.is_white_turn = true;
				if (line == "b")
					game.is_white_turn = false;
				if (line != "w" && line != "b") 
					throw Exception("Cannot load the game!");
				return is;
			}
			else throw Exception("Cannot load the game!");
		}
		 throw Exception("Cannot load the game!");
	}


    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
	std::ostream& operator<< (std::ostream& os, const Game& game) {
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}
}
