// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <bitset>
using namespace std;

// Our Files
#include "inputparse.h"

const unsigned int bitLen = 36;

unsigned long long partOne(const vector<string> &input);
unsigned long long partTwo(const vector<string> &input);

vector< bitset<bitLen> > getAllAddrs(bitset<bitLen> currBits, const string &mask, const unsigned int fromInd);

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

unsigned long long partOne(const vector<string> &input) {
	map<int, unsigned long long> mem;
	string mask;

	for (string line : input) {
		if (line.substr(0, 4) == "mask") {
			// changing mask
			mask = line.substr(7);
		} else {
			// getting value to add
			vector<string> lineSpl = split(line, "] = ");
			int addr = stoi(lineSpl.at(0).substr(4));
			bitset<bitLen> valBin(stoll(lineSpl.at(1)));

			// applying mask
			for (unsigned int i = 0; i < bitLen; ++i) {
				if (mask.at(i) == '0') {
					valBin[bitLen - i - 1] = 0;
				} else if (mask.at(i) == '1') {
					valBin[bitLen - i - 1] = 1;
				}
			}

			// adding to memory
			map<int, unsigned long long>::iterator exists = mem.find(addr);
			if (exists == mem.end()) {
				mem.insert(pair<int, unsigned long long>(addr, valBin.to_ullong()));
			} else {
				exists->second = valBin.to_ullong();
			}
		}
	}

	// going through memory and adding all values
	unsigned long long total = 0;
	for (map<int, unsigned long long>::const_iterator itr = mem.begin(); itr != mem.end(); ++itr) {
		total += itr->second;
	}

	return total;
}

unsigned long long partTwo(const vector<string> &input) {
	map<unsigned long long, unsigned long long> mem;
	string mask;

	for (string line : input) {
		if (line.substr(0, 4) == "mask") {
			// changing mask
			mask = line.substr(7);
		} else {
			// getting value to add
			vector<string> lineSpl = split(line, "] = ");
			bitset<bitLen> addrPre(stoll(lineSpl.at(0).substr(4)));
			unsigned long long val = stoll(lineSpl.at(1));

			// applying mask
			vector< bitset<bitLen> > addrs = getAllAddrs(addrPre, mask, 0);

			// adding to memory
			for (bitset<bitLen> addr : addrs) {
				map<unsigned long long, unsigned long long>::iterator exists = mem.find(addr.to_ullong());
				if (exists == mem.end()) {
					mem.insert(pair<unsigned long long, unsigned long long>(addr.to_ullong(), val));
				} else {
					exists->second = val;
				}
			}
		}
	}

	// going through memory and adding all values
	unsigned long long total = 0;
	for (map<unsigned long long, unsigned long long>::const_iterator itr = mem.begin(); itr != mem.end(); ++itr) {
		total += itr->second;
	}

	return total;
}

// TODO: make this apply all of the 1s in the mask first, before recursively applying the Xs
vector< bitset<bitLen> > getAllAddrs(bitset<bitLen> currBits, const string &mask, const unsigned int fromInd) {
	vector< bitset<bitLen> > retVec;

	for (unsigned int i = fromInd; i < bitLen; ++i) {
		if (mask.at(i) == '1') {
			// if the mask has a 1, it overwrites that bit with a 1
			currBits[bitLen - i - 1] = 1;
		} else if (mask.at(i) == 'X') {
			// otherwise, it branches to two different addresses with a 0 and
			// a 1 in that particular bit
			for (int j = 0; j <= 1; ++j) {
				currBits[bitLen - i - 1] = j;
				vector< bitset<bitLen> > newAddrs = getAllAddrs(currBits, mask, i+1);
				retVec.insert(retVec.end(), newAddrs.begin(), newAddrs.end());
			}

			// breaking early, since the recursion should account for
			// all subsequent bits
			break;
		}
	}
	if (retVec.empty()) {
		// (if there was no more branching, just push the current address)
		retVec.push_back(currBits);
	}

	return retVec;
}

