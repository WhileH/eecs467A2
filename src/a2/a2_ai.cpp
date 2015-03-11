#include "a2_ai.hpp"

bool A2AI::win(const BoardState& boardstate) {

	char winner = 'N'; //n - none	
	
	for (unsigned int i = 0; i < 3; i++) {

		//across
		if ( boardstate.board[i][0] == boardstate.board[i][1] && boardstate.board[i][1] == boardstate.board[i][2] )
			winner = boardstate.board[i][0];
	
		//down
		else if ( boardstate.board[0][i] == boardstate.board[1][i] && boardstate.board[1][i] == boardstate.board[2][i] ) 
			winner = boardstate.board[0][i];
	
		//forward diagnol	
		else if ( boardstate.board[0][0] == boardstate.board[1][1] && boardstate.board[1][1] == boardstate.board[2][2] ) 
			winner = board.board[0][0];
	
		//backward diagnol	
		else if ( boardstate.board[0][2] == boardstate.board[1][1] && boardstate.board[1][1] == boardstate.board[2][0] ) 
			winner = boardstate.board[0][2];
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

bool A2AI::draw(const BoardState& boardstate) {

	for (unsigned int  i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {		
			if (boardstate.board[i][j] != 'R' && boardstate.board[i][j] != 'G')
				return false;	
		}
	}
	
	cout << "It is a draw!" << endl;
	return true;
}

int A2AI::AI(const BoardState& board, const PlayerID& id) {

	int nextMove;
	
	boardstate.displayBoard();

	if(win() || draw())
		exit(0);
	
	//simple AI for now, put ball in the next available cell
	//going from position 0 to position 8
	for (unsigned int  i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {	
			if (boardstate.board[i][j] == '.') {
				nextMove = (i * 3) + j;			
			}
		}
	}	
	
	cout << id << "moves to position " << nextMove << endl;
	
	return nextMove;

std::vector <int> A2AI::nextMove(const BoardState& boardstate, const PlayerID& id) {

	int move = AI(boardstate, id);

	std::vector <int> nextMoveCoord (2);
		
	switch (move)

		case 0:
			nextMoveCoord[0] = 1;
			nextMoveCoord[1] = 1;
			break;
		case 1:
			nextMoveCoord[0] = 1;
			nextMoveCoord[1] = 2;
			break;
		case 2:
			nextMoveCoord[0] = 1;
			nextMoveCoord[1] = 3;
			break;
		case 3:
			nextMoveCoord[0] = 2;
			nextMoveCoord[1] = 1;
			break;
		case 4:
			nextMoveCoord[0] = 2;
			nextMoveCoord[1] = 2;
			break;
		case 5:
			nextMoveCoord[0] = 2;
			nextMoveCoord[1] = 3;
			break;
		case 6:
			nextMoveCoord[0] = 3;
			nextMoveCoord[1] = 1;
			break;
		case 7:
			nextMoveCoord[0] = 3;
			nextMoveCoord[1] = 2;
			break;
		case 8:
			nextMoveCoord[0] = 3;
			nextMoveCoord[1] = 3;
			break;
		default:
			std::cout << "invalid move position" << endl;
			exit(1);
			break;

	nextMoveCoord = boardstate.BoardtoArmCoord(nextMoveCoord);

	return nextMoveCoord;
}

