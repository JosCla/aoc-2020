// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
using namespace std;

// Our Files
#include "inputparse.h"

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

int nthNum(const vector<int> &startingNums, const unsigned int n);

int main() {
	vector<string> inputLines = readFileToLines("input");
	if (inputLines.empty()) {
		return 1;
	}
	inputLines.pop_back();

	cout << "Part One: " << partOne(inputLines) << endl;
	cout << "(don't worry, this one just takes a while...)" << endl;
	cout << "Part Two: " << partTwo(inputLines) << endl;

	return 0;
}

int partOne(const vector<string> &input) {
	vector<int> inputNums = vecStrToInt(split(input.at(0), ","));
	return nthNum(inputNums, 2020);
}

int partTwo(const vector<string> &input) {
	vector<int> inputNums = vecStrToInt(split(input.at(0), ","));
	return nthNum(inputNums, 30000000);
}

int nthNum(const vector<int> &startingNums, const unsigned int n) {
	map<int, int> lastInd;
	for (unsigned int i = 0; i < startingNums.size(); ++i) {
		lastInd[startingNums.at(i)] = i;
	}

	int prev = startingNums.at(startingNums.size()-1);
	for (unsigned int i = startingNums.size()-1; i < (n-1); ++i) {
		int next;
		if (lastInd.find(prev) != lastInd.end()) {
			next = i - lastInd[prev];
		} else {
			next = 0;
		}

		lastInd[prev] = i;
		prev = next;
	}

	return prev;
}
