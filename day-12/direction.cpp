// direction.cpp
// Implements features declared in direction.h

// C++ Standard Library
#include <string>
using std::string;

// Our files
#include "direction.h"

// Returns the opposite of a certain direction
Direction opposite(const Direction &dir) {
	switch (dir) {
		case DIR_UP:
			return DIR_DOWN;
			break;
		case DIR_RIGHT:
			return DIR_LEFT;
			break;
		case DIR_DOWN:
			return DIR_UP;
			break;
		case DIR_LEFT:
			return DIR_RIGHT;
			break;
	}
	return DIR_UP;
}

// Returns the given direction rotated once clockwise
Direction turnRight(const Direction &dir) {
	switch (dir) {
		case DIR_UP:
			return DIR_RIGHT;
			break;
		case DIR_RIGHT:
			return DIR_DOWN;
			break;
		case DIR_DOWN:
			return DIR_LEFT;
			break;
		case DIR_LEFT:
			return DIR_UP;
			break;
	}
	return DIR_UP;
}

// Returns the given direction rotated once counter-clockwise
Direction turnLeft(const Direction &dir) {
	switch (dir) {
		case DIR_UP:
			return DIR_LEFT;
			break;
		case DIR_RIGHT:
			return DIR_UP;
			break;
		case DIR_DOWN:
			return DIR_RIGHT;
			break;
		case DIR_LEFT:
			return DIR_DOWN;
			break;
	}
	return DIR_UP;
}

// Returns a string corresponding to the given direction
string dirStr(const Direction &dir) {
	switch (dir) {
		case DIR_UP:
			return "up";
			break;
		case DIR_RIGHT:
			return "right";
			break;
		case DIR_DOWN:
			return "down";
			break;
		case DIR_LEFT:
			return "left";
			break;
	}
	return "up";
}

// Translates the given contents towards a given direction
void translateBy(int &x, int &y, const Direction &dir) {
	switch (dir) {
		case DIR_UP:
			--y;
			break;
		case DIR_RIGHT:
			++x;
			break;
		case DIR_DOWN:
			++y;
			break;
		case DIR_LEFT:
			--x;
			break;
	}
}

// Translates the given contents towards a given direction
void translateBy(int &x, int &y, const Direction &dir, const int amount) {
	switch (dir) {
		case DIR_UP:
			y -= amount;
			break;
		case DIR_RIGHT:
			x += amount;
			break;
		case DIR_DOWN:
			y += amount;
			break;
		case DIR_LEFT:
			x -= amount;
			break;
	}
}


char dirToArrow(const Direction &dir) {
	switch (dir) {
		case DIR_UP:
			return '^';
			break;
		case DIR_RIGHT:
			return '>';
			break;
		case DIR_DOWN:
			return 'v';
			break;
		case DIR_LEFT:
			return '<';
			break;
	}
	return '^';
}
