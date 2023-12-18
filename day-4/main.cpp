// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

const vector<string> PROP_KEY = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid"};

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

int countValid(const vector<string> &passports, int validFunc);

bool validatePassportOne(const vector<string> &passport);
bool validatePassportTwo(const vector<string> &passport);

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
	return countValid(input, 1);
}

int partTwo(const vector<string> &input) {
	return countValid(input, 2);
}

int countValid(const vector<string> &passports, int validFunc) {
	int totalValid = 0;
	vector<string> currProps(8, "");

	for (string line : passports) {
		if (line == "") {
			// end of current passport
			bool valid;
			if (validFunc == 1) {
				valid = validatePassportOne(currProps);
			} else if (validFunc == 2) {
				valid = validatePassportTwo(currProps);
			}
			if (valid) {++totalValid;}

			currProps = vector<string>(8, "");
		} else {
			// properties line
			regex regexp(" ");
			vector<string> split = splitReg(line, regexp);

			// getting properties and values
			for (string prop : split) {
				int colonInd = prop.find(':');
				string propName = prop.substr(0, colonInd);
				string propVal = prop.substr(colonInd+1);

				unsigned int propInd;
				for (propInd = 0; propInd < PROP_KEY.size(); ++propInd) {
					if (PROP_KEY.at(propInd) == propName) {break;}
				}

				currProps.at(propInd) = propVal;
			}
		}
	}

	// (for last passport)
	bool valid;
	if (validFunc == 1) {
		valid = validatePassportOne(currProps);
	} else if (validFunc == 2) {
		valid = validatePassportTwo(currProps);
	}
	if (valid) {++totalValid;}

	return totalValid;
}

bool validatePassportOne(const vector<string> &passport) {
	for (unsigned int i = 0; i < PROP_KEY.size()-1; ++i) {
		if (passport.at(i) == "") {
			return false;
		}
	}
	return true;
}

bool validatePassportTwo(const vector<string> &passport) {
	// all necessary fields present
	if (!validatePassportOne(passport)) {
		return false;
	}

	// byr
	int byr = stoi(passport.at(0));
	if (byr < 1920 || byr > 2002) {
		return false;
	}

	// iyr
	int iyr = stoi(passport.at(1));
	if (iyr < 2010 || iyr > 2020) {
		return false;
	}

	// eyr
	int eyr = stoi(passport.at(2));
	if (eyr < 2020 || eyr > 2030) {
		return false;
	}

	// hgt
	string hgt = passport.at(3);
	smatch sm;
	regex regCm("(?=[0-9]+cm)[0-9]+");
	regex regIn("(?=[0-9]+in)[0-9]+");
	if (regex_search(hgt, sm, regCm)) {
		int heightCm = stoi(sm[0]);
		if (heightCm < 150 || heightCm > 193) {
			return false;
		}
	} else if (regex_search(hgt, sm, regIn)) {
		int heightIn = stoi(sm[0]);
		if (heightIn < 59 || heightIn > 76) {
			return false;
		}
	} else {
		return false;
	}

	// hcl
	string hcl = passport.at(4);
	regex regHcl("#[0-9a-f]{6}");
	if (!regex_search(hcl, sm, regHcl)) {
		return false;
	}

	// ecl
	string ecl = passport.at(5);
	if (!(ecl == "amb" || ecl == "blu" || ecl == "brn" || ecl == "gry" || ecl == "grn" || ecl == "hzl" || ecl == "oth")) {
		return false;
	}

	// pid
	string pid = passport.at(6);
	if (pid.length() != 9) {
		return false;
	}

	return true;
}
