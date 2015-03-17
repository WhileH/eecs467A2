#include "board_state.hpp"

BoardState::BoardState() {	}

int BoardState::ballsLeft() {
	return availBalls.size();
}

eecs467::Point <double> BoardState::nextFreeBall() {
		eecs467::Point <double> point = availBalls[0];
		
		availBalls.erase(availBalls.begin());
		std::cout << '\t' << point.x << ' ' << point.y << std::endl;
		return point;
}
	
std::vector <char> BoardState::determineStateofBoard(std::vector <int>& greenBalls, 
						     std::vector <int>& redBalls, std::vector <int>& cyanSquares, int imgWidth, int imgHeight,calibration_t &cali, char color) {
	
	//clear vectors
	availBalls.clear();
	
	//initilize board
	std::vector<char> camBoard;
	 for(unsigned int i = 0; i < 9; i++) 
        camBoard.push_back(' ');

	//get distance between registration squares (pixels to meters)
	 //not needed annymore, will keep temporarily for sanity checks
	eecs467::Point<double> sq1Point;
	eecs467::Point<double> sq2Point;
	 
	std::sort(cyanSquares.begin(), cyanSquares.end());

	std::cout<<"sorted cyanSquares here"<<std::endl;
	
	sq1Point.x = cyanSquares[0]%imgWidth;
	sq1Point.y = imgHeight - (cyanSquares[0]/imgWidth);

	sq2Point.x = cyanSquares[1]%imgWidth;
	sq2Point.y = imgHeight - (cyanSquares[1]/imgWidth);
	std::cout<< "sq1Point: "<<sq1Point.x<<" "<< sq1Point.y<<std::endl;
	std::cout<< "sq2Point: "<<sq2Point.x<<" "<< sq2Point.y<<std::endl;
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

		double d = fabs(point.x) + fabs(point.y);
		std::cout << "redBall \t" << point.x << "," << point.y << std::endl;

		if( (point.x > 0) || (point.y >= fabs(sq1Point.y)) || (point.y <= -1*fabs(sq1Point.y)) ){
		  if(color == 'R') {
		  std::cout << "Pushing back: " << point.x << ' ' << point.y << std::endl;
			availBalls.push_back(point);
		  }
		}
		else {
		  point = convert(point, d); //converts to board coordinates
		  std::cout << "red_convert: "<< point.x << ' '<< point.y <<std::endl;
		  if(point.x != 99)
		    camBoard[3 * point.x + point.y] = 'R';
		}

	}
		

	for (unsigned int i = 0; i < greenBalls.size(); i++) {
			
		point.x = greenBalls[i]%imgWidth; 
		point.y = imgHeight - (greenBalls[i]/imgWidth); 
			
		point = cali.translate(point);

		double d = fabs(point.x) + fabs(point.y);
		std::cout << "greenBall \t" << point.x << "," << point.y << std::endl;

		if( (point.x > 0) || (point.y >= fabs(sq1Point.y)) || (point.y <= -1*fabs(sq1Point.y))){
		  if(color == 'G')
			availBalls.push_back(point);
		}

		else {
		  point = convert(point, d); //converts to board coordinates
		  std::cout << "green_convert: "<< point.x << ' '<< point.y <<std::endl;
		  if(point.x != 99)
		    camBoard[3 * point.x + point.y] = 'G';
		}

	}	

	return camBoard;			
}

eecs467::Point<int> BoardState::convert(eecs467::Point<double> p, double d){
	

  //d = round(d * 20.0) / 20.0;
  //std::cout << '\t' << d << std::endl;
  eecs467::Point<int> point;
  double l_thres1 = 0.05;
  double thres1 = 0.075;
  double thres2 = 0.14;
  double thres3 = 0.19;
  double thres4 = 0.25;

  if ( d>= l_thres1 && d <= thres1 ) {
    point.x = 0;
    point.y = 1;
  }
  else if ( d>thres1 && d<=thres2 && p.y>0 && fabs(p.x)<=0.08) {
    point.x = 0;
    point.y = 0;
  }
  else if ( d>thres1 && d<=thres2 && p.y<0 && fabs(p.x)<=0.08) {
    point.x = 0;
    point.y = 2;
  }
  else if ( d>thres1 && d<=thres2 && fabs(p.x)>=0.09 ) {
    point.x = 1;
    point.y = 1;
  }
  else if ( d>thres2 && d<=thres3 && p.y>0 && fabs(p.x)<=0.14 ) {
    point.x = 1;
    point.y = 0;
  }
  else if ( d>thres2 && d<=thres3 && p.y<0 && fabs(p.x)<=0.14 ) {
    point.x = 1;
    point.y = 2;
  }
  else if ( d>thres2 && d<=thres3 && fabs(p.x)>0.14 ) {
    point.x = 2;
    point.y = 1;
  }
  else if ( d>thres3 && d<=thres4 && p.y>0 ) {
    point.x = 2;
    point.y = 0;
  }
  else if ( d>thres3 && d<=thres4 && p.y<0 ) {
    point.x = 2;
    point.y = 2;
  }
  else {
    point.x = 99;
    point.y = 99;
  }
		
  return point;
}

