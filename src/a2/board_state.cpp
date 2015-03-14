#include "board_state.hpp"

BoardState::BoardState() {	}

bool BoardState::ballsLeft() {
	if(availBalls.size() == 0)
		return false;
	else
		return true;
}

eecs467::Point <int> BoardState::nextFreeBall() {
		eecs467::Point <int> point = availBalls[0];
		availBalls.erase(availBalls.begin());
		return point;
}
	
std::vector <char> BoardState::determineStateofBoard(std::vector <int>& greenBalls, 
     std::vector <int>& redBalls, std::vector <int>& cyanSquares, int imgWidth) {
	
	//clear vectors
	availBalls.clear();
	
	//initilize board
	std::vector<char> camBoard;
	 for(unsigned int i = 0; i < 9; i++) 
        camBoard.push_back(' ');

	//get distance between registration squares (pixels to meters)
	eecs467::Point<double> sq1Point;
	eecs467::Point<double> sq2Point;

	std::sort(cyanSquares.begin(), cyanSquares.end());
	
	sq1Point.x = cyanSquares[0]%imgWidth;
	sq1Point.y = cyanSquares[0]/imgWidth;

	sq2Point.x = cyanSquares[1]%imgWidth;
	sq2Point.y = cyanSquares[1]/imgWidth;

	std::vector <double> sq1Vec = cali.translate(sq1Point);
	std::vector <double> sq2Vec = cali.translate(sq2Point);

	//parameters used to calulate board position
	double x_o = sq1Vec[0];
	double y_o = sq1Vec[1];
	double distBetSquares = abs(sq1Vec[1] - sq2Vec[0]);

	//getting board coordinates for the balls

	eecs467::Point <double> point;
	std::vector <double> coord;

	for (unsigned int i = 0; i < redBalls.size(); i++) {

		point.x = redBalls[i]%imgWidth; 
		point.y = redBalls[i]/imgWidth; 
			
		coord = cali.translate(point);

		double x = coord[0];
		double y = coord[1];

		double tmp = x_o + ((distBetSquares - gridSize)/2);		
		int xBoard = (x - tmp)/gridCellSize;

		tmp = -y_o + ((distBetSquares - gridSize)/2);
		int yBoard = (-y - tmp)/gridCellSize;

		if ( (xBoard < 0) || (yBoard < 0) ) {
			point.x = xBoard;
			point.y = yBoard;
			availBalls.push_back(point);
		}				
		else 
			camBoard[3 * xBoard + yBoard] = 'R';
	}
		

	for (unsigned int i = 0; i < greenBalls.size(); i++) {
			
		point.x = greenBalls[i]%imgWidth; 
		point.y = greenBalls[i]/imgWidth; 
			
		coord = cali.translate(point);

		double x = coord[0];
		double y = coord[1];

		double tmp = x_o + ((distBetSquares - gridSize)/2);		
		int xBoard = (x - tmp)/gridCellSize;

		tmp = -y_o + ((distBetSquares - gridSize)/2);
		int yBoard = (-y - tmp)/gridCellSize;

		if ( (xBoard < 0) || (yBoard < 0) ) {
			point.x = xBoard;
			point.y = yBoard;
			availBalls.push_back(point);
		}				
		else 
			camBoard[3 * xBoard + yBoard] = 'G';
	}	
	
	return camBoard;			
}
