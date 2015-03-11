#ifndef A2_AI_HPP
#define A2_AI_HPP

#include <vector>
#include <iostream>

//A2
#include "board_state.hpp"
#include "game_constants.hpp"

namespace eecs467 {

class A2AI {

private:  	

	/*
		is there a win. exits if so
	*/

	bool win(const BoardState& board);

	/*
		is there a draw, exits if so
	*/
	bool draw(const BoardState& board);
	
	/*
		chooses next position to place ball
	*/
	int AI(const BoardState& board, const PlayerID& id);

public:

	A2AI();
	
	/*
		returns arm frame coordinate where to put next ball
	*/
	std::vector <int> nextMove(const BoardState& boardstate, const PlayerID& id);
};

}

#endif
