// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

struct Rule {
	string attr;
	int min1, max1;
	int min2, max2;
};

int partOne(const vector<string> &input);
long long int partTwo(const vector<string> &input);

void structureInputs(const vector<string> &input, vector<Rule> &rules, vector<int> &yourTicket, vector< vector<int> > &otherTickets);
void findInvalid(const vector<Rule> &rules, const vector< vector<int> > &otherTickets, vector<int> &invalidInds, vector<int> &invalidVals);

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
	vector<Rule> rules;
	vector<int> yourTicket;
	vector< vector<int> > otherTickets;
	structureInputs(input, rules, yourTicket, otherTickets);

	vector<int> invalidInds;
	vector<int> invalidVals;
	findInvalid(rules, otherTickets, invalidInds, invalidVals);

	int scanError = 0;
	for (int val : invalidVals) {
		scanError += val;
	}

	return scanError;
}

long long int partTwo(const vector<string> &input) {
	vector<Rule> rules;
	vector<int> yourTicket;
	vector< vector<int> > otherTickets;
	structureInputs(input, rules, yourTicket, otherTickets);

	vector<int> invalidInds;
	vector<int> invalidVals;
	findInvalid(rules, otherTickets, invalidInds, invalidVals);

	// erasing invalid tickets
	for (int i = invalidInds.size() - 1; i >= 0; --i) {
		otherTickets.erase(otherTickets.begin() + invalidInds.at(i));
	}

	// finding potential rule-property matches
	vector< vector<bool> > potentialMatches;

	for (unsigned int i = 0; i < rules.size(); ++i) {
		Rule rule = rules.at(i);
		vector<bool> currMatches(yourTicket.size(), true);

		// checking each ticket to see which properties could fit this rule
		for (unsigned int j = 0; j < otherTickets.size(); ++j) {
			vector<int> ticket = otherTickets.at(j);

			for (unsigned int k = 0; k < ticket.size(); ++k) {
				int val = ticket.at(k);
				if (!((val >= rule.min1 && val <= rule.max1) || (val >= rule.min2 && val <= rule.max2))) {
					currMatches.at(k) = false;
				}
			}
		}

		potentialMatches.push_back(currMatches);
	}

	vector<int> ruleInds(yourTicket.size(), -1);
	vector<bool> ruleTaken(yourTicket.size(), false);

	// finding which rule matches which index
	for (unsigned int matches = 0; matches < yourTicket.size(); ++matches) {
		for (unsigned int i = 0; i < potentialMatches.size(); ++i) {
			if (ruleInds.at(i) != -1) {continue;}

			// seeing how many indices this rule could still occupy
			int totalSeen = 0;
			int lastIndSeen = 0;
			for (unsigned int j = 0; j < yourTicket.size(); ++j) {
				if (ruleTaken.at(j) == true) {continue;}
				if (potentialMatches.at(i).at(j) == true) {
					lastIndSeen = j;
					++totalSeen;
				}
			}

			// if we've only seen one possible rule, we've found the right one
			if (totalSeen == 1) {
				ruleInds.at(i) = lastIndSeen;
				ruleTaken.at(lastIndSeen) = true;
				break;
			}
		}
	}

	// finding the product of the first six properties (the ones that start with "departure")
	long long int prod = 1;
	for (int i = 0; i < 6; ++i) {
		prod *= yourTicket.at(ruleInds.at(i));
	}

	return prod;
}

void structureInputs(const vector<string> &input, vector<Rule> &rules, vector<int> &yourTicket, vector< vector<int> > &otherTickets) {
	int readState = 0;
	for (string line : input) {
		// changing read state as needed
		if (line == "") {continue;}
		if (line.substr(0, 4) == "your") {readState = 1; continue;}
		if (line.substr(0, 6) == "nearby") {readState = 2; continue;}

		if (readState == 0) {
			// reading rules
			regex ruleReg(": | or ");
			vector<string> currLine = splitReg(line, ruleReg);

			Rule currRule;
			currRule.attr = currLine.at(0);
			vector<string> range1 = split(currLine.at(1), "-");
			currRule.min1 = stoi(range1.at(0));
			currRule.max1 = stoi(range1.at(1));
			vector<string> range2 = split(currLine.at(2), "-");
			currRule.min2 = stoi(range2.at(0));
			currRule.max2 = stoi(range2.at(1));

			rules.push_back(currRule);
		} else if (readState == 1) {
			// reading your ticket
			vector<string> yourTicketStr = split(line, ",");
			for (string num : yourTicketStr) {
				yourTicket.push_back(stoi(num));
			}
		} else if (readState == 2) {
			// reading nearby ticket
			vector<string> otherTicketStr = split(line, ",");
			vector<int> otherTicket;
			for (string num : otherTicketStr) {
				otherTicket.push_back(stoi(num));
			}

			otherTickets.push_back(otherTicket);
		}
	}
}

void findInvalid(const vector<Rule> &rules, const vector< vector<int> > &otherTickets, vector<int> &invalidInds, vector<int> &invalidVals) {
	vector<bool> validArr(1000, false);
	for (Rule rule : rules) {
		for (int i = rule.min1; i < rule.max1; ++i) {
			validArr.at(i) = true;
		}
		for (int i = rule.min2; i < rule.max2; ++i) {
			validArr.at(i) = true;
		}
	}

	for (unsigned int i = 0; i < otherTickets.size(); ++i) {
		vector<int> ticket = otherTickets.at(i);
		bool validTicket = true;

		for (unsigned int j = 0; j < ticket.size(); ++j) {
			if (validArr.at(ticket.at(j)) == false) {
				invalidVals.push_back(ticket.at(j));
				validTicket = false;
			}
		}

		if (!validTicket) {
			invalidInds.push_back(i);
		}
	}
}


