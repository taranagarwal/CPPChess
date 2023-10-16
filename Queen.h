// Patrick Li - pli50
// Iason Mihalopoulos - imihalo1
// Taran Agarwal - tagarwa4

#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

namespace Chess
{
	class Queen : public Piece {

	public:
    bool legal_move_shape(const Position& start, const Position& end) const override;

		int point_value() const override {return 9;};

		/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		char to_ascii() const override { return is_white() ? 'Q' : 'q';	}
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		std::string to_unicode() const override { return is_white() ? "\u2655" : "\u265B"; }

	private:
		/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		Queen(bool is_white) : Piece(is_white) {}

		friend Piece* create_piece(const char& piece_designator);
	};
}

#endif // QUEEN_H
