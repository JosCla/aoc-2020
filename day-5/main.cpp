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

int getSeatID(const string &pass);

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
	int highestID = 0;
	for (string pass : input) {
		int currID = getSeatID(pass);
		if (currID > highestID) {
			highestID = currID;
		}
	}
	return highestID;
}

int partTwo(const vector<string> &input) {
	vector<bool> seatsPresent(partOne(input) + 1, false);
	for (string pass : input) {
		seatsPresent.at(getSeatID(pass)) = true;
	}

	bool seenTrue = false;
	for (unsigned int i = 0; i < seatsPresent.size(); ++i) {
		if (seatsPresent.at(i) == true) {
			seenTrue = true;
		} else if (seenTrue) {
			return i;
		}
	}
	return 0; // (control should never reach here)
}
	
int getSeatID(const string &pass) {
	string binStr = "";
	for (unsigned int i = 0; i < pass.length(); ++i) {
		char digit = pass.at(i);
		if (digit == 'B' || digit == 'R') {
			binStr += "1";
		} else {
			binStr += "0";
		}
	}
	return stoi(binStr, nullptr, 2);
}
