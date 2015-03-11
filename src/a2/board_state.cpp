#include "board_state.hpp"

namespace eecs467 {

	BoardState::BoardState() {	
		//create board		
		std::vector <char> row (3,'.');
		
		for (unsigned int i; i < 3; i++)
		board.push_back(row);	
	}
	
	void BoardState::getBoard() {
		determineState();
	}

		
	void BoardState::determineState(std::vector <int> greenBalls, std::vector <int>  	
									redBalls,std::vector <int> cyanSquares) {
			
				
		
	}

	void BoardState::displayGrid() {
		
		std::cout << "---------------------" << std::endl;
    	std::cout << board[0][0] << board[0][1] << board[0][2] << std::endl;
    	std::cout << board[1][0] << board[1][1] << board[1][2] << std::endl;
    	std::cout << board[2][0] << board[2][1] << board[2][2] << std::endl;
    	std::cout << "---------------------" << std::endl;  		
	}
	

}

