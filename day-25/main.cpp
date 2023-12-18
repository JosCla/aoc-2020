// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <bitset>
using namespace std;

// Our Files
#include "inputparse.h"

long long int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

int findLoops(const int &pubkey);
void performLoop(long long int &num, const long long int &subject);

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
	int loops = findLoops(stoi(input.at(0)));
	long long int curr = 1;
	for (int i = 0; i < loops; ++i) {
		performLoop(curr, stoll(input.at(1)));
	}

	return curr;
}

int partTwo(const vector<string> &input) {
	return 0;
}

int findLoops(const int &pubkey) {
	long long int num = 1;
	int loops = 0;
	while (num != pubkey) {
		performLoop(num, 7);
		++loops;
	}

	return loops;
}

void performLoop(long long int &num, const long long int &subject) {
	num = (num * subject) % 20201227;
}

	
