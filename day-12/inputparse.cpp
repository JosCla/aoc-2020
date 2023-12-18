// inputparse.cpp
// Implements inputparse.h.

// C++ Standard Library
#include <iostream>
using std::cout; using std::cerr; using std::endl;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <regex>
using std::regex; using std::smatch; using std::regex_search;

vector<string> readFileToLines(const string filepath) {
	vector<string> fileLines;

	// Opening the file
	ifstream fileIn;
	fileIn.open(filepath);
	if (fileIn.fail()) {
		cerr << "Failed to open file. Aborting." << endl;
		return fileLines;
	}

	// Reading the file's contents
	while (!fileIn.eof()) {
		string currLine;
		getline(fileIn, currLine);
		fileLines.push_back(currLine);
	}

	// Closing the file
	fileIn.close();

	return fileLines;
}

vector<string> splitReg(string input, const regex &regexp) {
	// Creating match_result object and vector for the split string
	smatch matchRes;
	vector<string> splitVec;

	// While the regex is able to find its pattern, push the substring through the matched sequence into the split vector
	while (regex_search(input, matchRes, regexp)) {
		splitVec.push_back(input.substr(0, matchRes.position(0)));
		input = matchRes.suffix();
	}
	splitVec.push_back(input);

	return splitVec;
}
