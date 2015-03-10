#ifndef GAME_CONSTANTS_HPP
#define GAME_CONSTANTS_HPP

#include <cmath>

// game constants

namespace eecs467 {

	enum PlayerID : char { RED, GREEN, UNKOWN };

	static const float grid_cell = 0.06; //m
	static const float ball_radius = 0.021; //m
	static const float ballDistfromArm = 0.165; //m
	
	static const int hz = 20; //lcm msg freq

}

#endif /* GAME_CONSTANTS_HPP */
