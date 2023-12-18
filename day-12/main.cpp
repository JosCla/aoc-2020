// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cmath>
using namespace std;

// Our Files
#include "inputparse.h"
#include "direction.h"

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

void rotate(int &x, int &y, const int &degrees);

int main() {
	vector<string> inputLines = readFileToLines("input");
	if (inputLines.empty()) {
		return 1;
	}
	inputLines.pop_back();

	cout << "Part One: " << partOne(inputLines) << endl;
	cout << "Part Two: " << partTwo(inputLines) << endl;

	return 0;
}

int partOne(const vector<string> &input) {
	Direction ourDirection = DIR_RIGHT;
	int x(0), y(0);

	for (string line : input) {
		char instr = line.at(0);
		int num = stoi(line.substr(1));

		switch (instr) {
			case 'N':
				y -= num;
				break;
			case 'S':
				y += num;
				break;
			case 'E':
				x += num;
				break;
			case 'W':
				x -= num;
				break;

			case 'L':
				{
					int numTurns = num / 90;
					for (int i = 0; i < numTurns; ++i) {
						ourDirection = turnLeft(ourDirection);
					}
				}
				break;
			case 'R':
				{
					int numTurns = num / 90;
					for (int i = 0; i < numTurns; ++i) {
						ourDirection = turnRight(ourDirection);
					}
				}
				break;
			case 'F':
				translateBy(x, y, ourDirection, num);
				break;
		}
	}

	return abs(x) + abs(y);
}

int partTwo(const vector<string> &input) {
	int shipX(0), shipY(0);
	int wayX(10), wayY(-1);

	for (string line : input) {
		char instr = line.at(0);
		int num = stoi(line.substr(1));

		switch (instr) {
			case 'N':
				wayY -= num;
				break;
			case 'S':
				wayY += num;
				break;
			case 'E':
				wayX += num;
				break;
			case 'W':
				wayX -= num;
				break;

			case 'L':
				rotate(wayX, wayY, num);
				break;
			case 'R':
				rotate(wayX, wayY, -num);
				break;
			case 'F':
				for (int i = 0; i < num; ++i) {
					shipX += wayX;
					shipY += wayY;
				}
				break;
		}
	}

	return abs(shipX) + abs(shipY);
}

void rotate(int &x, int &y, const int &degrees) {
	if (degrees > 0) {
		// counter-clockwise
		for (int i = 0; i < degrees; i += 90) {
			int temp = x;
			x = y;
			y = -temp;
		}
	} else if (degrees < 0) {
		// clockwise
		for (int i = 0; i > degrees; i -= 90) {
			int temp = x;
			x = -y;
			y = temp;
		}
	}
}
