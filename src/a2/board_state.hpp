#ifndef BOARD_STATE_HPP
#define BOARD_STATE_HPP

#include <vector>


namespace eecs467 {

class BoardState {

private:

	std::vector <char> board (9, '.'); // init board
	
public:
	
	BoardState();
	
	void updateBoard();
	
	void decideMove();

	void outputBoard();

};

}

#endif
