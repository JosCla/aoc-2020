// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

void findCoords(int &x, int &y, const string &dirs);
void flipTile(const pair<int, int> &coords, map< pair<int, int>, bool > &tiles, map< pair<int, int>, int > &neighbors);

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
	map< pair<int, int>, bool > tiles;
	for (string line : input) {
		int x(0), y(0);
		findCoords(x, y, line);
		pair<int, int> coords(x, y);

		tiles[coords] = !tiles[coords];
	}

	int total = 0;
	for (auto it = tiles.begin(); it != tiles.end(); ++it) {
		if (it->second) {
			++total;
		}
	}

	return total;
}

int partTwo(const vector<string> &input) {
	map< pair<int, int>, bool > tiles;
	map< pair<int, int>, int > neighbors;
	for (string line : input) {
		int x(0), y(0);
		findCoords(x, y, line);
		pair<int, int> coords(x, y);

		flipTile(coords, tiles, neighbors);
	}

	for (int i = 0; i < 100; ++i) {
		vector< pair<int, int> > changeVec;

		for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
			bool tile = tiles[it->first];
			int neighs = it->second;

			if (tile && (neighs == 0 || neighs > 2)) {
				changeVec.push_back(it->first);
			} else if (!tile && (neighs == 2)) {
				changeVec.push_back(it->first);
			}
		}

		for (pair<int, int> coords : changeVec) {
			flipTile(coords, tiles, neighbors);
		}
	}

	int total = 0;
	for (auto it = tiles.begin(); it != tiles.end(); ++it) {
		if (it->second) {
			++total;
		}
	}

	return total;
}

void findCoords(int &x, int &y, const string &dirs) {
	for (unsigned int i = 0; i < dirs.size(); ++i) {
		char curr = dirs.at(i);
		char next = '0';
		if (i < (dirs.size() - 1)) {
			next = dirs.at(i+1);
		}
		
		switch (curr) {
			case 'e':
				x += 2;
				break;
			case 'w':
				x -= 2;
				break;
			
			case 'n':
				y -= 2;

				switch (next) {
					case 'e':
						x += 1;
						break;
					case 'w':
						x -= 1;
						break;
				}

				++i;
				break;
			case 's':
				y += 2;

				switch (next) {
					case 'e':
						x += 1;
						break;
					case 'w':
						x -= 1;
						break;
				}

				++i;
				break;
		}
	}
}

void flipTile(const pair<int, int> &coords, map< pair<int, int>, bool > &tiles, map< pair<int, int>, int > &neighbors) {
	// flipping tile
	tiles[coords] = !tiles[coords];
	int change = (tiles[coords] ? 1 : -1);
	neighbors[coords] += 0; // making sure neighbors[coords] exists

	// changing neighbor count
	vector< vector<int> > displacements = {
		{-1, -2},
		{1, -2},
		{-2, 0},
		{2, 0},
		{-1, 2},
		{1, 2}
	};
	for (auto disp : displacements) {
		pair<int, int> newCoords(coords.first + disp.at(0), coords.second + disp.at(1));
		neighbors[newCoords] += change;
	}
}
