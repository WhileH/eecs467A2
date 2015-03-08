#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <lcm/lcm-cpp.hpp>
#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include "math/point.hpp"

//common
#include "common/getopt.h"
#include "common/timestamp.h"
#include "common/zarray.h"

//A2
#include "board_state.hpp"
#include "game_constants.hpp"

bool win(const BoardState& board) {

	char winner = 'N'; //n - none	
	
	for (unsigned int i = 0; i < 3; i++) {

		//across
		if ( board.grid[i][0] == board.grid[i][1] && board.grid[i][1] == board.grid[i][2] )
			winner = board.board[i][0];
	
		//down
		else if ( board.grid[0][i] == board.grid[1][i] && board.grid[1][i] == board.grid[2][i] ) 
			winner = board.board[0][i];
	
		//forward diagnol	
		else if ( board.grid[0][0] == board.grid[1][1] && board.grid[1][1] == board.grid[2][2] ) 
			winner = board.board[0][0];
	
		//backward diagnol	
		else if ( board.grid[0][2] == board.grid[1][1] && board.grid[1][1] == board.grid[2][0] ) 
			winner = board.board[0][2];
	}
	
	if (winner == 'R') {
		cout << "Red wins." << endl;
		return true;
	}
	else if (winner == 'G') {
		cout << "Green wins." << endl;
		return true;
	}
	else 
		return false;
}

bool draw(const BoardState& board) {

	for (unsigned int  i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {		
			if (board.grid[i][j] != 'R' && board.grid[i][j] != 'G')
				return false;	
		}
	}
	
	cout << "It is a draw!" << endl;
	return true;
}

int AI(const BoardState& board, cosnt PlayerID& id) {

	int nextMove;
	
	board.displayGrid();

	if(win() || draw())
		exit(0);
	
	//simple AI for now, put ball in the next available cell
	//going from position 0 to position 8
	for (unsigned int  i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {	
			if (board.grid[i][j] == '.') {
				nextMove = (i * 3) + j;			
			}
		}
	}	
	
	cout << id << "moves to position " << nextMove << endl;
	return nexMove;	
}


int main (int argc, char *argv[])
{
 	
 	BoardState board;
	PlayerID id = "GREEN"; //this should be given by main program
	
	board.getBoard();
	
	AI(board, id);
    
   
}
