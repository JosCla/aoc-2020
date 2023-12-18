// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

int partOne(const vector<string> &input);
long long int partTwo(const vector<string> &input);

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
	int treesHit = 0;
	int sledX = 0;
	int width = input.at(0).length();
	for (unsigned int i = 0; i < input.size(); ++i) {
		sledX = (i * 3) % width;
		if (input.at(i).at(sledX) == '#') {
			++treesHit;
		}
	}

	return treesHit;
}

long long int partTwo(const vector<string> &input) {
	const vector< vector<int> > displacements = {
		{1, 1},
		{3, 1},
		{5, 1},
		{7, 1},
		{1, 2}
	};
	long long int treesHitProd = 1;

	for (unsigned int n = 0; n < displacements.size(); ++n) {
		int treesHit = 0;
		int sledX = 0;
		int sledY = 0;
		int width = input.at(0).length();
		while (sledY < input.size()) {
			if (input.at(sledY).at(sledX) == '#') {
				++treesHit;
			}
			sledX += displacements.at(n).at(0);
			sledX %= width;
			sledY += displacements.at(n).at(1);
		}

		cout << treesHit << endl;
		treesHitProd *= treesHit;
	}

	return treesHitProd;
}
