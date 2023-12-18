#ifndef DIRECTION_H
#define DIRECTION_H
// direction.h
// Contains functionality for Direction

// C++ Standard Library
#include <string>

enum Direction {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT
};

Direction turnRight(const Direction &dir);
Direction turnLeft(const Direction &dir);
void translateBy(int &x, int &y, const Direction &dir);
void translateBy(int &x, int &y, const Direction &dir, const int amount);

#endif // DIRECTION_H
