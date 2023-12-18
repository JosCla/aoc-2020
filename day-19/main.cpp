// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
using namespace std;

// Our Files
#include "inputparse.h"

struct Rule {
	int type;
	vector< vector<int> > seqs;
	char letter;
};

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

void parseInputs(const vector<string> &lines, map<int, Rule> &rules, vector<string> &mesgs);
string getRegStr(const map<int, Rule> &rules, int id);
bool doesMatch(const string &msg, const regex &reg42, const regex &reg31, int num42 = 0, int num31 = 0);

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
	// getting rules and messages
	map<int, Rule> rules;
	vector<string> mesgs;
	parseInputs(input, rules, mesgs);

	// finding the regex for rule 0
	string regStr0 = getRegStr(rules, 0);
	regex reg0(regStr0);

	// checking to see which lines fit the regex
	int total = 0;
	for (string line : input) {
		smatch matchRes;
		regex_match(line, matchRes, reg0);
		if (matchRes.size() > 0 && matchRes.str(0) == line) {
			++total;
		}
	}

	return total;
}

int partTwo(const vector<string> &input) {
	// getting rules and messages
	map<int, Rule> rules;
	vector<string> msgs;
	parseInputs(input, rules, msgs);

	// finding the regex for rules 42 and 31
	string regStr42 = getRegStr(rules, 42);
	string regStr31 = getRegStr(rules, 31);
	regex reg42(regStr42);
	regex reg31(regStr31);

	// checking to see which messages fit the requirements
	int total = 0;
	for (string msg : msgs) {
		if (doesMatch(msg, reg42, reg31)) {
			++total;
		}
	}

	return total;
}

// parses inputs into a map of rules and a vector of messages
void parseInputs(const vector<string> &lines, map<int, Rule> &rules, vector<string> &mesgs) {
	int readMode = 0;
	for (string line : lines) {
		switch (readMode) {
		case 0:
		// reading rules
		{
			if (line.empty()) {
				readMode = 1;
				continue;
			}

			regex regSpl(": | \\\| ");
			vector<string> lineSpl = splitReg(line, regSpl);

			Rule rule;
			int id = stoi(lineSpl.at(0));
			if (lineSpl.at(1).at(0) == '"') {
				// letter rule
				rule.type = 1;
				rule.letter = lineSpl.at(1).at(1);
			} else {
				// sequence rule
				rule.type = 0;
				for (unsigned int i = 1; i < lineSpl.size(); ++i) {
					string seq = lineSpl.at(i);
					vector<int> currSeq;
					for (string num : split(seq, " ")) {
						currSeq.push_back(stoi(num));
					}
					rule.seqs.push_back(currSeq);
				}
			}

			rules[id] = rule;
			break;
		}

		case 1:
		// reading messages
		{
			mesgs.push_back(line);
			break;
		}
		}
	}
}

// recursively finds the regex string for a particular rule
string getRegStr(const map<int, Rule> &rules, int id) {
	Rule rule = rules.at(id);
	string regStr = "";

	if (rule.type == 0) {
		regStr += "(?:";
		for (unsigned int i = 0; i < rule.seqs.size(); ++i) {
			vector<int> currSeq = rule.seqs.at(i);
			for (int num : currSeq) {
				regStr += getRegStr(rules, num);
			}
			if (i != rule.seqs.size() - 1) {
				regStr += '|';
			}
		}
		regStr += ")";
	} else {
		regStr += rule.letter;
	}

	return regStr;
}

// finds whether or not a particular message can be expressed as m 42 patterns followed by n 31 patterns, such that both m and n are nonzero and such that m > n
bool doesMatch(const string &msg, const regex &reg42, const regex &reg31, int num42, int num31) {
	if (msg.empty()) {
		// end
		if (num42 > num31 && num31 != 0) {
			return true;
		} else {
			return false;
		}
	} else if (num31 == 0) {
		// searching for 42 or 31
		bool canWork = false;

		smatch res42, res31;
		bool found42 = regex_search(msg, res42, reg42);
		bool found31 = regex_search(msg, res31, reg31);

		if (found42 && res42.prefix() == "") {
			if (doesMatch(res42.suffix(), reg42, reg31, num42 + 1, num31)) {
				canWork = true;
			}
		}
		if (found31 && res31.prefix() == "") {
			if (doesMatch(res31.suffix(), reg42, reg31, num42, num31 + 1)) {
				canWork = true;
			}
		}

		return canWork;
	} else {
		// searching only for 31 (done with 42)
		bool canWork = false;

		smatch res31;
		bool found31 = regex_search(msg, res31, reg31);

		if (found31 && res31.prefix() == "") {
			if (doesMatch(res31.suffix(), reg42, reg31, num42, num31 + 1)) {
				canWork = true;
			}
		}

		return canWork;
	}
}
