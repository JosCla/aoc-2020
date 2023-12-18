// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

int findChanges(vector<string> &seats, const int part);
int numSurroundingOne(const vector<string> &seats, const int row, const int col);
int numSurroundingTwo(const vector<string> &seats, const int row, const int col);

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
	vector<string> seats = input;
	while (findChanges(seats, 1) > 0) {}

	int occupied = 0;
	for (string line : seats) {
		for (char seat : line) {
			if (seat == '#') {++occupied;}
		}
	}

	return occupied;
}

int partTwo(const vector<string> &input) {
	vector<string> seats = input;
	while (findChanges(seats, 2) > 0) {}

	int occupied = 0;
	for (string line : seats) {
		for (char seat : line) {
			if (seat == '#') {++occupied;}
		}
	}

	return occupied;
}

// Finds and applies any necessary changes to the seat vector, returning total number of changes
int findChanges(vector<string> &seats, const int part) {
	vector< vector<int> > changeVec;
	int totalChanges = 0;

	int minToLeave = 0;
	if (part == 1) {
		minToLeave = 4;
	} else if (part == 2) {
		minToLeave = 5;
	}

	// Finding changes
	for (unsigned int row = 0; row < seats.size(); ++row) {
		changeVec.push_back(vector<int>(seats.at(row).size(), 0));

		for (unsigned int col = 0; col < seats.at(row).size(); ++col) {
			char curr = seats.at(row).at(col);
			if (curr == '.') {continue;}

			int surround = 0;
			if (part == 1) {
				surround = numSurroundingOne(seats, row, col);
			} else if (part == 2) {
				surround = numSurroundingTwo(seats, row, col);
			}

			if (curr == 'L' && surround == 0) {
				changeVec.at(row).at(col) = 1;
				++totalChanges;
			} else if (curr == '#' && surround >= minToLeave) {
				changeVec.at(row).at(col) = -1;
				++totalChanges;
			}
		}
	}

	// Applying changes
	for (unsigned int row = 0; row < seats.size(); ++row) {
		for (unsigned int col = 0; col < seats.at(row).size(); ++col) {
			int curr = changeVec.at(row).at(col);

			if (curr == 1) {
				seats.at(row).at(col) = '#';
			} else if (curr == -1) {
				seats.at(row).at(col) = 'L';
			}
		}
	}

	return totalChanges;
}

// Returns the number of occupied seats surrounding a particular coordinate (part one)
int numSurroundingOne(const vector<string> &seats, const int row, const int col) {
	int total = 0;

	for (int i = row-1; i <= row+1; ++i) {
		if (i < 0 || i >= seats.size()) {continue;}

		for (int j = col-1; j <= col+1; ++j) {
			if (j < 0 || j >= seats.at(i).size()) {continue;}
			if (i == row && j == col) {continue;}

			if (seats.at(i).at(j) == '#') {
				++total;
			}
		}
	}

	return total;
}

// Returns the number of occupied seats surrounding a particular coordinate (part two)
int numSurroundingTwo(const vector<string> &seats, const int row, const int col) {
	int total = 0;

	vector< vector<int> > displacements;

	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (i == 0 && j == 0) {continue;}

			displacements.push_back(vector<int>({i, j}));
		}
	}

	for (vector<int> displacement : displacements) {
		bool occupiedSeen = false;
		int rowCurr(row), colCurr(col);

		while (true) {
			rowCurr += displacement.at(0);
			colCurr += displacement.at(1);

			if (rowCurr < 0 || rowCurr >= seats.size() || colCurr < 0 || colCurr >= seats.at(rowCurr).size()) {break;}
			if (seats.at(rowCurr).at(colCurr) == 'L') {break;}
			if (seats.at(rowCurr).at(colCurr) == '#') {
				occupiedSeen = true;
				break;
			}
		}

		if (occupiedSeen) {
			++total;
		}
	}

	return total;
}

