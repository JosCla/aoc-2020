// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

struct InputLine {
	int min, max;
	char letter;
	string password;
};

int partOne(const vector<InputLine> &input);
int partTwo(const vector<InputLine> &input);

int main() {
	vector<string> inputLines = readFileToLines("input");
	if (inputLines.empty()) {
		return 1;
	}
	inputLines.pop_back();

	vector<InputLine> input;
	regex splitExpr("(?= [a-z]) |-|: ");
	for (unsigned int i = 0; i < inputLines.size(); ++i) {
		vector<string> currLine = splitReg(inputLines.at(i), splitExpr);
		InputLine line;

		line.min = stoi(currLine.at(0));
		line.max = stoi(currLine.at(1));
		line.letter = currLine.at(2).at(0);
		line.password = currLine.at(3);

		input.push_back(line);
	}

	cout << "Part 1: " << partOne(input) << endl;
	cout << "Part 2: " << partTwo(input) << endl;

	return 0;
}

int partOne(const vector<InputLine> &input) {
	int numGood = 0;

	for (auto currLine : input) {
		int letterCount = 0;

		for (auto currLetter : currLine.password) {
			if (currLetter == currLine.letter) {
				++letterCount;
			}
		}

		if (letterCount >= currLine.min && letterCount <= currLine.max) {
			++numGood;
		}
	}

	return numGood;
}

int partTwo(const vector<InputLine> &input) {
	int numGood = 0;

	for (auto currLine : input) {
		int numLet = 0;
		if (currLine.password.at(currLine.min-1) == currLine.letter) {
			++numLet;
		}
		if (currLine.password.at(currLine.max-1) == currLine.letter) {
			++numLet;
		}

		if (numLet == 1) {
			++numGood;
		}
	}

	return numGood;
}
