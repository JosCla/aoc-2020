// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
using namespace std;

// Our Files
#include "inputparse.h"

long long int partOne(const vector<string> &input);
long long int partTwo(const vector<string> &input);

bool numSumIn(const long long int num, const vector<long long int> &numVec);

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

long long int partOne(const vector<string> &input) {
	// Creating the initial vector of previous values
	int prevLen = 25;
	vector<long long int> prevVec;
	for (int i = 0; i < prevLen; ++i) {
		prevVec.push_back(stoll(input.at(i)));
	}
	sort(prevVec.begin(), prevVec.end());

	for (unsigned int i = prevLen; i < input.size(); ++i) {
		// Checking if the current number can be expressed as the sum of the previous ones
		long long int currNum = stoll(input.at(i));
		if (!numSumIn(currNum, prevVec)) {
			return currNum;
		}

		// Removing the oldest number in the previous vector, and adding the current one
		prevVec.erase(find(prevVec.begin(), prevVec.end(), stoll(input.at(i - prevLen))));
		prevVec.push_back(currNum);
		sort(prevVec.begin(), prevVec.end());
	}

	return 0;
}

long long int partTwo(const vector<string> &input) {
	long long int invalidNum = partOne(input);

	for (unsigned int i = 0; i < input.size(); ++i) {
		long long int currSum = 0;
		long long int smallest = stoll(input.at(i));
		long long int largest = smallest;

		for (unsigned int j = i; j < input.size(); ++j) {
			long long int currInput = stoll(input.at(j));
			currSum += currInput;

			if (currInput < smallest) {
				smallest = currInput;
			}
			if (currInput > largest) {
				largest = currInput;
			}

			if (currSum == invalidNum) {
				return smallest + largest;
			} else if (currSum > invalidNum) {
				break;
			}
		}
	}

	return 0;
}

// Returns whether a number can be expressed as the sum of two numbers in a given sorted vector
bool numSumIn(const long long int num, const vector<long long int> &numVec) {
	int firstIndex = 0;
	int lastIndex = numVec.size() - 1;

	while (firstIndex < lastIndex) {
		long long int sum = numVec.at(firstIndex) + numVec.at(lastIndex);
		if (sum == num) {
			return true;
		} else if (sum < num) {
			++firstIndex;
		} else if (sum > num) {
			--lastIndex;
		}
	}

	return false;
}
