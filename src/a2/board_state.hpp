#ifndef BOARD_STATE_HPP
#define BOARD_STATE_HPP

#include <vector>
#include <iostream>

namespace eecs467 {

class BoardState {
	
	/*
	uses board coordinates where (0,0) is the regisration square at the top left 
	and (4,4) is the registration square at the bottom right.
	then returns all coordinates in the arm coordinate frame to be used for
	placment of balls.
	*/

private: 
	
	/*
	expects coordinates of the red and green balls in the board coordinate 
	frame, and determines the state of the board	
	*/	
	void determineState(const std::vector <int>& greenBalls, const std::vector <int>& 
						redBalls, cosnt std::vector <int>& cyanSquares);

public: 
	
	std::vector <std::vector <char> > board;
	
	BoardState();
	
	/*
		updates the board to match the real world tictactoe board
	*/
	void getBoard();
	
	/* 
		outputs the board
	*/
	void displayBoard();

};

}

#endif
