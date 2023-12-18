// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
using namespace std;

// Our Files
#include "inputparse.h"
#include "conway.h"

int partZero(const vector<string> &input);
int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);
int partThree(const vector<string> &input);
int partFour(const vector<string> &input);

int main() {
	vector<string> inputLines = readFileToLines("input");
	if (inputLines.empty()) {
		return 1;
	}
	inputLines.pop_back();

	//cout << "Part Zero: " << partZero(inputLines) << endl;
	cout << "Part One: " << partOne(inputLines) << endl;
	cout << "Part Two: " << partTwo(inputLines) << endl;
	//cout << "Part Three: " << partThree(inputLines) << endl;
	//cout << "Part Four: " << partFour(inputLines) << endl;

	return 0;
}

int partZero(const vector<string> &input) {
	ConwayND conway(input, 2);
	for (int i = 0; i < 6; ++i) {
		conway.step();
	}
	
	return conway.count();
}

int partOne(const vector<string> &input) {
	ConwayND conway(input, 3);
	for (int i = 0; i < 6; ++i) {
		conway.step();
	}
	
	return conway.count();
}

int partTwo(const vector<string> &input) {
	ConwayND conway(input, 4);
	for (int i = 0; i < 6; ++i) {
		conway.step();
	}
	
	return conway.count();
}

int partThree(const vector<string> &input) {
	ConwayND conway(input, 5);
	for (int i = 0; i < 6; ++i) {
		conway.step();
	}
	
	return conway.count();
}

int partFour(const vector<string> &input) {
	ConwayND conway(input, 6);
	for (int i = 0; i < 6; ++i) {
		conway.step();
	}
	
	return conway.count();
}

