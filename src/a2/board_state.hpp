#ifndef BOARD_STATE_HPP
#define BOARD_STATE_HPP

#include <vector>
#include <iostream>
#include <algorithm>
//#include <queue>

//A2
#include "game_constants.hpp"
#include "calibration.hpp"

#include "math/point.hpp"

class BoardState {
	
private:

	calibration_t cali;
	
	std::vector < eecs467::Point <int> > availBalls; 

public:

	BoardState();

	/*
	expects coordinates of the red and green balls in the img coordinate frame
	and determines the state of the board	
	*/	
	std::vector <char> determineStateofBoard(std::vector <int>& greenBalls, std::vector 
				<int>& redBalls, std::vector <int>& cyanSquares, int imgWidth);	

	// returns num of balls left to place
	int ballsLeft();

	eecs467::Point <int> nextFreeBall();
	
};

#endif
