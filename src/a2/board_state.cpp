#include "board_state.hpp"

BoardState::BoardState() {	}
		
void determineStateofBoard(std::vector <int>& greenBalls, std::vector 
				<int>& redBalls, std::vector <int>& cyanSquares, int imgWidth) {

	std::sort(cyanSquares.begin(), cyanSquares.end());

	eecs467::Point<int> point;
	//get distance between registration squares (pixels)

	//convert from pixels to meters after task 4 is done
	//what do i need to do here to get this in meters
	//the rest of this won't work until it is in meters
		
	float x_o = cyanSquares[0]%imgWidth;
	float y_o = cyanSquares[0]/imgWidth;
	float distBetSquares = abs(cyanSquares[1]%imgWidth - cyanSquares[0]%imgWidth); 

	//convert from meters to board coordinates

	for (unsigned int i = 0; i < redBalls.size(); i++) {

		float x = redBalls[i]%imgWidth; // this is wrong, needs to be changed
		float y = redBalls[i]/imgWidth; // this is wrong
			
		float tmp = x_o + ((distBetSquares - gridSize)/2);		
		int xBoard = (x - tmp)/gridCellSize;

		tmp = -y_o + ((distBetSquares - gridSize)/2);
		int yBoard = (-y - tmp)/gridCellSize;

		if ( (xBoard < 0) || (yBoard < 0) ) {
			point.x = xBoard;
			point.y = yBoard;
			availBalls.push_back(point);
		}				
		else {
			board[xBoard][yBoard] = 'R';
			std::cout << "A RED ball is at ( " << xBoard << "," << yBoard << ")" << 
			std::endl;
		}
	}
		

	for (unsigned int i = 0; i < greenBalls.size(); i++) {
			
		float x = greenBalls[i]%imgWidth; //this is wrong
		float y = greenBalls[i]/imgWidth; //this is wrong
			
		float tmp = x_o + ((distBetSquares - gridSize)/2);		
		int xBoard = (x - tmp)/gridCellSize;

		tmp = -y_o + ((distBetSquares - gridSize)/2);
		int yBoard = (-y - tmp)/gridCellSize;

		if ( (xBoard < 0) || (yBoard < 0) ) {
			point.x = xBoard;
			point.y = yBoard;
			availBalls.push_back(point);
		}
		else {
			board[xBoard][yBoard] = 'G';
			std::cout << "A GREEN ball is at ( " << xBoard << "," << yBoard << ")" << 
			std::endl;
		}
	}		

	displayBoard();
		
}

void BoardState::displayBoard() {
		
	std::cout << "---------------------" << std::endl;
    std::cout << board[0][0] << board[0][1] << board[0][2] << std::endl;
    std::cout << board[1][0] << board[1][1] << board[1][2] << std::endl;
   	std::cout << board[2][0] << board[2][1] << board[2][2] << std::endl;
   	std::cout << "---------------------" << std::endl << std::endl;  		
}
