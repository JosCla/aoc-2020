#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main() {
	ifstream fileIn;
	fileIn.open("input.txt");
	if (fileIn.fail()) {
		cerr << "Failed to read input file. Aborting." << endl;
	}

	vector<int> input;
	while (!fileIn.eof()) {
		int curr;
		fileIn >> curr;
		input.push_back(curr);
	}
	input.pop_back();

	fileIn.close();

	// Part 1
	int resultOne = 0;
	for (unsigned int i = 0; i < input.size(); ++i) {
		int currInput = input.at(i);
		for (unsigned int j = i+1; j < input.size(); ++j) {
			if (currInput + input.at(j) == 2020) {
				resultOne = currInput * input.at(j);
				break;
			}
		}
		if (resultOne) {break;}
	}
	cout << "Part One: " << resultOne << endl;

	// Part 2
	int resultTwo = 0;
	for (unsigned int i = 0; i < input.size(); ++i) {
		for (unsigned int j = i+1; j < input.size(); ++j) {
			for (unsigned int k = j+1; k < input.size(); ++k) {
				if (input.at(i) + input.at(j) + input.at(k) == 2020) {
					resultTwo = input.at(i) * input.at(j) * input.at(k);
					break;
				}
			}
			if (resultTwo) {break;}
		}
		if (resultTwo) {break;}
	}
	cout << "Part Two: " << resultTwo << endl;

	return 0;
}
