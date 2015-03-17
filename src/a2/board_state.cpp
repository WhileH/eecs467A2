#include "board_state.hpp"
using namespace std;

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
	eecs467::Point<double> sq4Point;
	 
	std::sort(cyanSquares.begin(), cyanSquares.end());

	std::cout<<"sorted cyanSquares here"<<std::endl;
	if(cyanSquares.size() != 4)
	  std::cout << "Too many or too few cyan squares" << endl;

	sq1Point.x = cyanSquares[0]%imgWidth;
	sq1Point.y = imgHeight - (cyanSquares[0]/imgWidth);

	sq4Point.x = cyanSquares[3]%imgWidth;
	sq4Point.y = imgHeight - (cyanSquares[3]/imgWidth);
	std::cout<< "sq1Point: "<<sq1Point.x<<" "<< sq1Point.y<<std::endl;
	std::cout<< "sq2Point: "<<sq4Point.x<<" "<< sq4Point.y<<std::endl;
	sq1Point = cali.translate(sq1Point);
	sq4Point = cali.translate(sq4Point);

	std::cout << "cyan " << sq1Point.x << "," << sq1Point.y << std::endl;
	std::cout << "cyan " << sq4Point.x << "," << sq4Point.y << std::endl;

	//parameters used to calulate board position
	double x_o = sq1Point.x;
	double y_o = sq1Point.y;
	double distBetSquares = sqrt(sq(sq4Point.y - sq1Point.y)+sq(sq4Point.x-sq1Point.x));
	std::cout <<"distBetSquares:"<< distBetSquares << std::endl;

	//getting board coordinates for the balls
 
	eecs467::Point <double> point;

	for (unsigned int i = 0; i < redBalls.size(); i++) {

		point.x = redBalls[i]%imgWidth; 
		point.y = imgHeight - (redBalls[i]/imgWidth); 
			
		point = cali.translate(point);

		double d = fabs(point.x) + fabs(point.y);
		std::cout << "redBall \t" << point.x << "," << point.y << std::endl;
		int index;
		if( (point.x > 0) || (point.y >= fabs(sq1Point.y)) || (point.y <= -1*fabs(sq1Point.y)) ){
		  if(color == 'R') {
		  std::cout << "Pushing back: " << point.x << ' ' << point.y << std::endl;
			availBalls.push_back(point);
		  }
		}
		else {
		  index = convert(point, sq1Point, sq4Point); //converts to board coordinates
		  if(index != -1)
		    camBoard[index] = 'R';
		  std::cout << "Red Pos: " << index << std::endl;
		}

	}
		

	for (unsigned int i = 0; i < greenBalls.size(); i++) {
			
		point.x = greenBalls[i]%imgWidth; 
		point.y = imgHeight - (greenBalls[i]/imgWidth); 
			
		point = cali.translate(point);

		std::cout << "greenBall \t" << point.x << "," << point.y << std::endl;
		int index;
		if( (point.x > 0) || (point.y >= fabs(sq1Point.y)) || (point.y <= -1*fabs(sq1Point.y))){
		  if(color == 'G')
			availBalls.push_back(point);
		}

		else {
		  index = convert(point, sq1Point, sq4Point); //converts to board coordinates
		  if(index != -1)
		    camBoard[index] = 'G';
		  std::cout << "Green Pos: " << index << std::endl;
		}

	}	

	return camBoard;			
}

int BoardState::convert(eecs467::Point<double> p, eecs467::Point<double> sq1, eecs467::Point<double> sq4){
	
  /*
  //d = round(d * 20.0) / 20.0;
  //std::cout << '\t' << d << std::endl;
  eecs467::Point<int> point;
  double l_thres1 = 0.04;
  double thres1 = 0.095;
  double thres2 = 0.16;
  double thres3 = 0.21;
  double thres4 = 0.27;

  if ( d>= l_thres1 && d <= thres1 ) {
    point.x = 0;
    point.y = 1;
  }
  else if ( d>thres1 && d<=thres2 && p.y>0 && fabs(p.x)<=thres1) {
    point.x = 0;
    point.y = 0;
  }
  else if ( d>thres1 && d<=thres2 && p.y<0 && fabs(p.x)<=thres1) {
    point.x = 0;
    point.y = 2;
  }
  else if ( d>thres1 && d<=thres2 && fabs(p.x)>=thres1 ) {
    point.x = 1;
    point.y = 1;
  }
  else if ( d>thres2 && d<=thres3 && p.y>0 && fabs(p.x)<=thres2 ) {
    point.x = 1;
    point.y = 0;
  }
  else if ( d>thres2 && d<=thres3 && p.y<0 && fabs(p.x)<=thres2 ) {
    point.x = 1;
    point.y = 2;
  }
  else if ( d>thres2 && d<=thres3 && fabs(p.x)>thres2 ) {
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
  */

  //double len = sqrt(sq(sq1.x + sq4.x) + sq(sq1.y + sq4.y));
  double sqSize = gridCellSize;
  eecs467::Point<double> center;
  center.x = (sq1.x + sq4.x)/2.0;
  center.y = (sq1.y + sq4.y)/2.0;
  
  vector<eecs467::Point<double>> b(9);

  /*
   *  b0   b1   b2
   *  b3   b4   b5
   *  b6   b7   b8
   */
  b[0].x = center.x + sqSize;
  b[0].y = center.y + sqSize;

  b[1].x = center.x + sqSize;
  b[1].y = center.y;

  b[2].x = center.x + sqSize;
  b[2].y = center.y - sqSize;

  b[3].x = center.x;
  b[3].y = center.y + sqSize;

  b[4].x = center.x;
  b[4].y = center.y;
  
  b[5].x = center.x;
  b[5].y = center.y - sqSize;

  b[6].x = center.x - sqSize;
  b[6].y = center.y + sqSize;

  b[7].x = center.x - sqSize;
  b[7].y = center.y;
  
  b[8].x = center.x - sqSize;
  b[8].y = center.y - sqSize;


  double minDist = 1.0;
  int closest = -1;
  for(int i=0; i<b.size(); ++i){
    double pD = sqrt(sq(p.x - b[i].x) + sq(p.y - b[i].y));
    if(pD < minDist){
      minDist = pD;
      closest = i;
    }
  }
  
  
  return closest;
}

