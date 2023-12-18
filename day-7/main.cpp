// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

struct Rule {
	string main;
	vector<string> contents;
	vector<int> contentsNums;
};

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

Rule lineToRule(const string &line);
int countInnerBags(const string &bagName, const vector<Rule> &rules);

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

// TODO: use dictionaries or something
int partOne(const vector<string> &input) {
	vector<Rule> rules;
	for (string line : input) {
		rules.push_back(lineToRule(line));
	}

	string target = "shiny gold";
	vector<string> bagsContainingTarget = {target};
	vector<bool> bagsAdded(rules.size(), false);
	bool update = true; // (denotes whether an update happened in the last search)
	while (update) {
		update = false;

		// searching for any bags that contain elements of our target vector
		for (unsigned int i = 0; i < rules.size(); ++i) {
			// seeing if the rule has already been put in the target vector
			if (bagsAdded.at(i)) {continue;}

			Rule rule = rules.at(i);
			
			// checking if the rule can contain the target
			for (string containedBag : rule.contents) {
				bool found = false;
				for (string bagName : bagsContainingTarget) {
					if (bagName == containedBag) {
						found = true;
						break;
					}
				}
				if (found) {
					bagsContainingTarget.push_back(rule.main);
					bagsAdded.at(i) = true;
					update = true;
					break;
				}
			}
		}
	}

	return (bagsContainingTarget.size() - 1);
}

int partTwo(const vector<string> &input) {
	vector<Rule> rules;
	for (string line : input) {
		rules.push_back(lineToRule(line));
	}

	string target = "shiny gold";
	return countInnerBags(target, rules);
}

// translates a line of input into a Rule struct
Rule lineToRule(const string &line) {
	Rule rule;
	regex splitMainReg(" bags contain ");
	regex splitContentsReg(" bag(s*)[\.,] *");

	vector<string> splitMain = splitReg(line, splitMainReg);
	rule.main = splitMain.at(0);

	if (splitMain.at(1) == "no other bags.") {
		return rule;
	}

	vector<string> splitContents = splitReg(splitMain.at(1), splitContentsReg);
	splitContents.pop_back();

	for (string content : splitContents) {
		rule.contentsNums.push_back((int)content.at(0) - (int)'0');
		rule.contents.push_back(content.substr(2));
	}

	return rule;
}

// returns how many bags a certain bag contains, given the rules
int countInnerBags(const string &bagName, const vector<Rule> &rules) {
	// first finding the bag's rule
	Rule rule;
	for (Rule currRule : rules) {
		if (currRule.main == bagName) {
			rule = currRule;
		}
	}

	// then returning how many bags it contains, recursing as needed
	if (rule.contents.empty()) {
		return 0;
	} else {
		int totalCount = 0;
		for (unsigned int i = 0; i < rule.contents.size(); ++i) {
			totalCount += rule.contentsNums.at(i) * (1 + countInnerBags(rule.contents.at(i), rules));
		}

		return totalCount;
	}
}
