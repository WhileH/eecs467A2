#ifndef BOARD_STATE_HPP
#define BOARD_STATE_HPP

#include <vector>


namespace eecs467 {

class BoardState {

private: 

	/*
		uses blob detection and converted coordinates to determine
		the state of the board	
	*/	
	void determineState();

public: 
	
	std::vector <std::vector <char> > grid;
	
	BoardState();
	
	/*
		updates the grid to match the real world tictactoe board
	*/
	void getBoard();
	
	/* 
		outputs the grid
	*/
	void displayGrid();

};

}

#endif
