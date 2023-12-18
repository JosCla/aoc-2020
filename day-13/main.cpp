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
	int startTime = stoi(input.at(0));
	vector<string> busTimeStrs = split(input.at(1), ",");
	int minWait = 50000;
	int minWaitProd = 0;

	// finding the minimum time to wait before a bus arrives
	for (string time : busTimeStrs) {
		if (time == "x") {continue;}
		
		int busTime = stoi(time);
		int waitTime = busTime - (startTime % busTime);
		if (waitTime < minWait) {
			minWait = waitTime;
			minWaitProd = busTime * waitTime;
		}
	}

	return minWaitProd;
}

long long int partTwo(const vector<string> &input) {
	// finding how often each bus arrives, and the necessary offset for each arrival
	vector<string> busTimeStrs = split(input.at(1), ",");
	vector<int> busTimes;
	vector<int> busOffsets;

	for (unsigned int i = 0; i < busTimeStrs.size(); ++i) {
		if (busTimeStrs.at(i) == "x") {continue;}

		busTimes.push_back(stoi(busTimeStrs.at(i)));
		busOffsets.push_back(i);
	}

	// finding when each bus arrives at the correct position
	// effectively, if we can find the interval at which the first two buses
	// arrive, then we only need to search those until we find one where
	// the third bus also arrives at the right time, so on and so forth
	long long int firstTime = busTimes.at(0);
	long long int prodPrevs = firstTime;
	for (unsigned int i = 1; i < busTimes.size(); ++i) {
		int currTime = busTimes.at(i);
		int currOffset = busOffsets.at(i);

		// to find the next time that all previous buses arrive at the right
		// time, we can just add the product of the interval length
		// of each bus
		for (unsigned int j = 1; j < currTime; ++j) {
			if ((firstTime + (prodPrevs * j) + currOffset) % currTime == 0) {
				firstTime = firstTime + (prodPrevs * j);
				prodPrevs *= currTime;
				break;
			}
		}
	}

	return firstTime;
}
