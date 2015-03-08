#include "board_state.hpp"
#include <iostream>

namespace eecs467 {

	BoardState::BoardState() {
	
		//create grid
		
		std::vector <char> row (3,'.');
		
		grid.push_back(row);
		grid.push_back(row);
		grid.push_back(row);
	
	}
	
	void BoardState::getBoard() {
		determineState();
	}
		
	void BoardState::determineState() {}

	void BoardState::displayGrid() {
		
		std::cout << "---------------------" << std::endl;
    	std::cout << grid[0][0] << grid[0][1] << grid[0][2] << std::endl;
    	std::cout << grid[1][0] << grid[1][1] << grid[1][2] << std::endl;
    	std::cout << grid[2][0] << grid[2][1] << grid[2][2] << std::endl;
    	std::cout << "---------------------" << std::endl;  		
	}
	

}

