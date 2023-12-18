// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cmath>
using namespace std;

// Our Files
#include "inputparse.h"

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

void parseInput(const vector<string> &input, vector<int> &deck1, vector<int> &deck2);
int getScore(const vector<int> &deck);

int combatRecu(vector<int> &deck1, vector<int> &deck2);

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
	vector<int> deck1, deck2;
	parseInput(input, deck1, deck2);

	// finding the winner
	int winner = 1;
	while (deck1.size() > 0 && deck2.size() > 0) {
		if (deck1.at(0) > deck2.at(0)) {
			winner = 1;
			deck1.push_back(deck1.at(0));
			deck1.push_back(deck2.at(0));
			deck1.erase(deck1.begin());
			deck2.erase(deck2.begin());
		} else {
			winner = 2;
			deck2.push_back(deck2.at(0));
			deck2.push_back(deck1.at(0));
			deck1.erase(deck1.begin());
			deck2.erase(deck2.begin());
		}
	}

	int score = 0;
	if (winner == 1) {
		score = getScore(deck1);
	} else if (winner == 2) {
		score = getScore(deck2);
	}

	return score;
}

int partTwo(const vector<string> &input) {
	vector<int> deck1, deck2;
	parseInput(input, deck1, deck2);

	// finding the winner
	int winner = combatRecu(deck1, deck2);

	int score = 0;
	if (winner == 1) {
		score = getScore(deck1);
	} else if (winner == 2) {
		score = getScore(deck2);
	}

	return score;
}

void parseInput(const vector<string> &input, vector<int> &deck1, vector<int> &deck2) {
	int readState = 1;
	for (string line : input) {
		if (line.empty()) {
			readState = 2;
			continue;
		} else if (line.at(0) == 'P') {
			continue;
		} else {
			int curr = stoi(line);
			if (readState == 1) {
				deck1.push_back(curr);
			} else if (readState == 2) {
				deck2.push_back(curr);
			}
		}
	}
}

int getScore(const vector<int> &deck) {
	int score = 0;
	for (unsigned int i = 0; i < deck.size(); ++i) {
		score += deck.at(i) * (deck.size() - i);
	}
	return score;
}

int combatRecu(vector<int> &deck1, vector<int> &deck2) {
	// initializing previous state vectors
	vector< vector<int> > prevDeck1 = {deck1};
	vector< vector<int> > prevDeck2 = {deck2};

	// doing combat
	int winner = 1;
	int round = 1;
	int curr2Pow = 0;
	while (deck1.size() > 0 && deck2.size() > 0) {
		// finding the winner
		if (deck1.at(0) <= (deck1.size() - 1) && deck2.at(0) <= (deck2.size() - 1)) {
			// recursion time
			vector<int> newDeck1(deck1.at(0)), newDeck2(deck2.at(0));
			copy_n(deck1.begin()+1, deck1.at(0), newDeck1.begin());
			copy_n(deck2.begin()+1, deck2.at(0), newDeck2.begin());

			winner = combatRecu(newDeck1, newDeck2);
		} else {
			// deciding winner normally
			if (deck1.at(0) > deck2.at(0)) {
				winner = 1;
			} else {
				winner = 2;
			}
		}

		// applying the win
		if (winner == 1) {
			deck1.push_back(deck1.at(0));
			deck1.push_back(deck2.at(0));
			deck1.erase(deck1.begin());
			deck2.erase(deck2.begin());
		} else {
			deck2.push_back(deck2.at(0));
			deck2.push_back(deck1.at(0));
			deck1.erase(deck1.begin());
			deck2.erase(deck2.begin());
		}

		// checking against previous states
		bool repeat = false;
		if (pow(2, curr2Pow) == round) { // (note: only checking for a repeat at every (2^n)th step really saves on computation)
			for (int i = 0; i < round; ++i) {
				if (prevDeck1.at(i) == deck1 && prevDeck2.at(i) == deck2) {
					// we've got a repeat!
					repeat = true;
					break;
				}
			}
			
			++curr2Pow;
		}
		if (repeat) {
			winner = 1;
			break;
		} else {
			prevDeck1.push_back(deck1);
			prevDeck2.push_back(deck2);
			++round;
		}
	}

	return winner;
}
