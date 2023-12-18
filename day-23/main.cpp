// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <list>
#include <algorithm>
using namespace std;

// Our Files
#include "inputparse.h"

string partOne(const vector<string> &input);
long long int partTwo(const vector<string> &input);

void doMove(list<int> &cups, const list<int>::iterator &currentCup, const int maxVal);
void doMoveTwo(vector<int> &nextVal, const int currCup);

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

string partOne(const vector<string> &input) {
	list<int> cups;
	for (char cup : input.at(0)) {
		cups.push_back(cup - '0');
	}
	list<int>::iterator currCup = cups.begin();

	for (int i = 0; i < 100; ++i) {
		doMove(cups, currCup, 9);
		++currCup;
		if (currCup == cups.end()) {currCup = cups.begin();}
	}

	string afterOne;
	list<int>::const_iterator onePos = find(cups.begin(), cups.end(), 1);
	list<int>::const_iterator currPos = onePos;
	++currPos;
	while (currPos != onePos) {
		afterOne += to_string(*currPos);
		++currPos;
		if (currPos == cups.end()) {currPos = cups.begin();}
	}

	return afterOne;
}

long long int partTwo(const vector<string> &input) {
	const int MAX_VAL = 1000000;

	vector<int> cups;
	for (char cup : input.at(0)) {
		cups.push_back(cup - '0');
	}
	cups.push_back(10);

	vector<int> nextVal(MAX_VAL + 1);
	for (int i = 0; i < 9; ++i) {
		nextVal.at(cups.at(i)) = cups.at(i+1);
	}
	for (int i = 10; i < MAX_VAL; ++i) {
		nextVal.at(i) = i+1;
	}
	nextVal.at(MAX_VAL) = cups.at(0);

	int currPos = cups.at(0);
	for (int i = 0; i < 10000000; ++i) {
		doMoveTwo(nextVal, currPos);
		currPos = nextVal.at(currPos);
	}

	long long int oneAfter = nextVal.at(1);
	long long int twoAfter = nextVal.at(oneAfter);
	long long int prod = oneAfter * twoAfter;

	return prod;
}

void doMove(list<int> &cups, const list<int>::iterator &currentCup, const int maxVal) {
	// getting the three cups after the current one
	list<int> moveList;
	for (int i = 0; i < 3; ++i) {
		list<int>::iterator movePos = currentCup;
		advance(movePos, 1);
		if (movePos == cups.end()) {movePos = cups.begin();}

		moveList.push_back(*movePos);
		cups.erase(movePos);
	}

	// finding the next cup to insert at
	int insCup = (*currentCup) - 1;
	list<int>::iterator insPos = cups.end();
	while (insPos == cups.end()) {
		if (insCup < 1) {insCup = maxVal;}
		insPos = find(cups.begin(), cups.end(), insCup);
		if (insPos == cups.end()) {--insCup;}
	}
	++insPos;
	if (insPos == cups.end()) {insPos = cups.begin();}

	// inserting the cups
	cups.splice(insPos, moveList);
}

void doMoveTwo(vector<int> &nextVal, const int currCup) {
	// getting the three cups after the current one
	vector<int> moveVec;
	int curr = currCup;
	for (int i = 0; i < 3; ++i) {
		moveVec.push_back(nextVal.at(curr));
		curr = nextVal.at(curr);
	}

	// finding the next cup to insert at
	int nextCup = currCup - 1;
	if (nextCup == 0) {nextCup = nextVal.size() - 1;}
	while (find(moveVec.begin(), moveVec.end(), nextCup) != moveVec.end()) {
		--nextCup;
		if (nextCup == 0) {nextCup = nextVal.size() - 1;}
	}

	// inserting the cups
	nextVal.at(currCup) = nextVal.at(moveVec.at(2));
	nextVal.at(moveVec.at(2)) = nextVal.at(nextCup);
	nextVal.at(nextCup) = moveVec.at(0);
}
