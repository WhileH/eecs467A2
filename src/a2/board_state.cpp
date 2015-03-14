#include "board_state.hpp"

BoardState::BoardState() {	}

int BoardState::ballsLeft() {
	return availBalls.size();
}

eecs467::Point <int> BoardState::nextFreeBall() {
		eecs467::Point <int> point = availBalls[0];
		availBalls.erase(availBalls.begin());
		return point;
}
	
std::vector <char> BoardState::determineStateofBoard(std::vector <int>& greenBalls, 
     std::vector <int>& redBalls, std::vector <int>& cyanSquares, int imgWidth, int imgHeight,calibration_t &cali) {
	
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

	std::cout<<"sorted cyanSquares here"<<std::endl;
 
	sq1Point.x = cyanSquares[0]%imgWidth;
	sq1Point.y = imgHeight - (cyanSquares[0]/imgWidth);

	sq2Point.x = cyanSquares[1]%imgWidth;
	sq2Point.y = imgHeight - (cyanSquares[1]/imgWidth);
 
	sq1Point = cali.translate(sq1Point);
	sq2Point = cali.translate(sq2Point);

	std::cout << "cyan " << sq1Point.x << "," << sq1Point.y << std::endl;
	std::cout << "cyan " << sq2Point.x << "," << sq2Point.y << std::endl;

	//parameters used to calulate board position
	double x_o = sq1Point.x;
	double y_o = sq1Point.y;
	double distBetSquares = sqrt(sq(sq2Point.y - sq1Point.y)+sq(sq2Point.x-sq1Point.x));
	std::cout <<"distBetSquares:"<< distBetSquares << std::endl;
	//getting board coordinates for the balls
 
	eecs467::Point <double> point;

	for (unsigned int i = 0; i < redBalls.size(); i++) {

		point.x = redBalls[i]%imgWidth; 
		point.y = imgHeight - (redBalls[i]/imgWidth); 
			
		point = cali.translate(point);

		double tmp = x_o + (distBetSquares - gridSize);		
		int xBoard = (point.x - tmp)/(gridCellSize);

		tmp = -y_o + (distBetSquares - gridSize);
		int yBoard = (-point.y - tmp)/(gridCellSize);

		std::cout << "red: "<< xBoard << ' '<< yBoard <<std::endl;

		if( xBoard > -2 || yBoard > -2 || xBoard < -4 || yBoard < -4)
			availBalls.push_back(point);
		else {
			point = convert(xBoard, yBoard); //converts to board coordinates
			std::cout << "red_convert: "<< point.x << ' '<< point.y <<std::endl;
			camBoard[3 * point.x + point.y] = 'R';
		}

	}
		

	for (unsigned int i = 0; i < greenBalls.size(); i++) {
			
		point.x = greenBalls[i]%imgWidth; 
		point.y = imgHeight - (greenBalls[i]/imgWidth); 
			
		point = cali.translate(point);

		double tmp = x_o + (distBetSquares - gridSize);		
		int xBoard = (point.x - tmp)/gridCellSize;

		tmp = -y_o + (distBetSquares - gridSize);
		int yBoard = (-point.y - tmp)/gridCellSize;

		std::cout << "Green: "<< xBoard << ' '<< yBoard <<std::endl;

		if( xBoard > -2 || yBoard > -2 || xBoard < -4 || yBoard < -4)
			availBalls.push_back(point);
		else {
			point = convert(xBoard, yBoard); //converts to board coordinates
			std::cout << "green_convert: "<< point.x << ' '<< point.y <<std::endl;
			camBoard[3 * point.x + point.y] = 'G';
		}

	}	

	return camBoard;			
}

eecs467::Point<int> BoardState::convert(int x, int y){
	
	eecs467::Point<int> point;

	if ( x==-2 and y==-2 ) {
		point.x = 0;
		point.y = 0;
	}
	else if ( x==-3 and y==-2 ) {
		point.x = 0;
		point.y = 1;
	}
	else if ( x==-4 and y==-2 ) {
		point.x = 0;
		point.y = 2;
	}
	else if ( x==-2 and y==-3 ) {
		point.x = 1;
		point.y = 0;
	}
	else if ( x==-3 and y==-3 ) {
		point.x = 1;
		point.y = 1;
	}
	else if ( x==-4 and y==-3 ) {
		point.x = 1;
		point.y = 2;
	}
	else if ( x==-2 and y==-4 ) {
		point.x = 2;
		point.y = 0;
	}
	else if ( x==-3 and y==-4 ) {
		point.x = 2;
		point.y = 1;
	}
	else if ( x==-4 and y==-4 ) {
		point.x = 2;
		point.y = 2;
	}
	//else {
	//	point.x = x;
	//	point.y = y;
	//	availBalls.push_back(point);
	//} 
		
	return point;
}

