// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
using namespace std;

// Our Files
#include "inputparse.h"

int partOne(const vector<string> &input);
long long int partTwo(const vector<string> &input);

long long int numValidPathsFrom(int index, const vector<int> &jolts, const vector<long long int> &validPathsCache);

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
	vector<int> jolts = {0};
	for (string line : input) {
		jolts.push_back(stoi(line));
	}

	sort(jolts.begin(), jolts.end());

	jolts.push_back(jolts.at(jolts.size() - 1) + 3);

	int numOne(0), numThree(0);

	for (unsigned int i = 0; i < jolts.size()-1; ++i) {
		if (jolts.at(i+1) - jolts.at(i) == 1) {
			++numOne;
		} else {
			++numThree;
		}
	}

	return numOne * numThree;
}

long long int partTwo(const vector<string> &input) {
	vector<int> jolts = {0};
	for (string line : input) {
		jolts.push_back(stoi(line));
	}
	sort(jolts.begin(), jolts.end());
	jolts.push_back(jolts.at(jolts.size() - 1) + 3);

	vector<long long int> validPathsCache(jolts.size());
	for (int i = jolts.size()-1; i >= 0; --i) {
		validPathsCache.at(i) = numValidPathsFrom(i, jolts, validPathsCache);
	}
	return validPathsCache.at(0);
}

long long int numValidPathsFrom(int index, const vector<int> &jolts, const vector<long long int> &validPathsCache) {
	if (index == jolts.size() - 1) {
		return 1;
	}

	long long int numValidPaths = 0;
	for (unsigned int i = index+1; i < jolts.size(); ++i) {
		if (jolts.at(i) - jolts.at(index) > 3) {
			break;
		} else {
			numValidPaths += validPathsCache.at(i);
		}
	}

	return numValidPaths;
}
