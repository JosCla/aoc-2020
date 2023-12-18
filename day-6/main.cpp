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

int sumGroupOne(const string &group);
int sumGroupTwo(const vector<bool> &group);

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
	int sumQuestions = 0;
	string currGroup = "";

	for (string line : input) {
		if (line == "") {
			// new group
			sumQuestions += sumGroupOne(currGroup);

			currGroup.clear();
		} else {
			// adding the current person to the current group
			currGroup += line;
		}
	}
	sumQuestions += sumGroupOne(currGroup); // (last group)

	return sumQuestions;
}

int partTwo(const vector<string> &input) {
	int sumQuestions = 0;
	vector<bool> groupAgree(26, true);
	
	for (string line : input) {
		if (line == "") {
			// new group
			sumQuestions += sumGroupTwo(groupAgree);

			groupAgree = vector<bool>(26, true);
		} else {
			// finding which questions the current person agrees with
			vector<bool> currAgree(26, false);
			for (char c : line) {
				currAgree.at((int)c - (int)'a') = true;
			}

			// 'and-ing' the current person with the whole group
			for (unsigned int i = 0; i < 26; ++i) {
				if (currAgree.at(i) == false) {
					groupAgree.at(i) = false;
				}
			}
		}
	}
	sumQuestions += sumGroupTwo(groupAgree); // (last group)

	return sumQuestions;
}

int sumGroupOne(const string &group) {
	int sum = 0;
	for (int i = group.length()-1; i >= 0; --i) {
		if (group.find(group.at(i), i+1) == string::npos) {
			++sum;
		}
	}
	return sum;
}

int sumGroupTwo(const vector<bool> &group) {
	int sum = 0;
	for (bool agree : group) {
		sum += (int)agree;
	}
	return sum;
}
