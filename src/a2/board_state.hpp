#ifndef BOARD_STATE_HPP
#define BOARD_STATE_HPP

#include <vector>
#include <iostream>
#include <algorithm>

//A2
#include "game_constants.hpp"

#include "math/point.hpp"

class BoardState {
	
private: 

public: 

	BoardState();
	
	std::vector <std::vector <char> > std::vector < vector <char> >(3, vector <char> 
		(3,'.'); //tic tac toe baord
	std::vector < eecs467::Point<int> > availBalls; //out of play balls

	/*
	expects coordinates of the red and green balls in the img coordinate frame
	and determines the state of the board	
	*/	
	void determineStateofBoard(std::vector <int>& greenBalls, std::vector 
				<int>& redBalls, std::vector <int>& cyanSquares, int imgWidth);	

	/* 
		outputs the board
	*/
	void displayBoard();

};

#endif
