// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <cmath>
using namespace std;

// Our Files
#include "inputparse.h"

struct Tile {
	int id;
	vector<string> pic;
	vector<unsigned int> edgeVals;
};

long long int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

void parseInputs(const vector<string> &input, map<int, Tile> &tiles);
void getEdgeVals(Tile &tile);

void flipVert(Tile &tile, int &dir);
void flipVertText(vector<string> &text);
void rotCW(Tile &tile, int &dir);
void rotCWText(vector<string> &text);
void orientToFit(Tile &tile, int start, const int target);

pair<int, int> findConnection(const Tile &tile1, const Tile &tile2);
void cullEdges(Tile &tile);

int ignorePattern(const vector<string> &pattern, const vector<string> &pic);

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
	// parsing input
	map<int, Tile> tiles;
	parseInputs(input, tiles);
	for (auto it = tiles.begin(); it != tiles.end(); ++it) {
		getEdgeVals(it->second);
	}

	// finding the product of the corner ids
	long long int prod = 1;
	for (auto it1 = tiles.begin(); it1 != tiles.end(); ++it1) {
		// finding any connections to a given tile
		vector<int> connections;
		for (auto it2 = tiles.begin(); it2 != tiles.end(); ++it2) {
			if (it1->first == it2->first) {continue;}

			if (findConnection(it1->second, it2->second).first != -1) {
				connections.push_back(it2->first);
			}
		}

		// if the tile has exactly 2 connections, it's a corner
		if (connections.size() == 2) {
			prod *= it1->first;
		}
	}

	return prod;
}

int partTwo(const vector<string> &input) {
	// parsing input
	map<int, Tile> tiles;
	parseInputs(input, tiles);
	for (auto it = tiles.begin(); it != tiles.end(); ++it) {
		getEdgeVals(it->second);
	}

	// finding all connections, along with a corner
	map< int, vector<int> > connections;
	int cornerID = -1;
	for (auto it1 = tiles.begin(); it1 != tiles.end(); ++it1) {
		// finding any connections to a given tile
		vector<int> currConnections;
		vector<int> connectionDirs;
		for (auto it2 = tiles.begin(); it2 != tiles.end(); ++it2) {
			if (it1->first == it2->first) {continue;}

			pair<int, int> sidePairing = findConnection(it1->second, it2->second);
			if (sidePairing.first != -1) {
				currConnections.push_back(it2->first);
				connectionDirs.push_back(sidePairing.first);
			}
		}

		// adding connections to overall connections map
		connections[it1->first] = currConnections;

		// looking for corners
		if (currConnections.size() == 2 && cornerID == -1) {
			cornerID = it1->first;

			// rotating as needed before inserting the corner
			int dir1 = (connectionDirs.at(0) & 254);
			int dir2 = (connectionDirs.at(1) & 254);
			while (!((dir1 == 2 && dir2 == 4) || (dir1 == 4 && dir2 == 2))) {
				rotCW(tiles[cornerID], dir1);
				dir2 = (dir2 + 2) % 8;
			}
		}
	}

	// getting the puzzle grid ready
	map<int, bool> inserted;
	map< int, pair<int, int> > insertedPos;
	vector< vector<int> > grid;
	int gridSize = sqrt(tiles.size());
	for (int i = 0; i < gridSize; ++i) {
		grid.push_back(vector<int>(gridSize, -1));
	}
	grid.at(0).at(0) = cornerID;
	inserted[cornerID] = true;
	insertedPos[cornerID] = pair<int, int>(0, 0);

	// putting the puzzle together
	vector<int> queue = {cornerID};
	while (queue.size() > 0) {
		int currID = queue.at(0);
		vector<int> currConnections = connections[currID];
		
		pair<int, int> tileCoords = insertedPos[currID];

		// finding which connected tiles to insert
		for (int targetID : currConnections) {
			if (inserted[targetID]) {continue;}

			// reorienting the new tile
			pair<int, int> connection = findConnection(tiles[currID], tiles[targetID]);
			orientToFit(tiles[targetID], connection.second, connection.first);

			// finding where to put the new tile
			int newRow(tileCoords.first), newCol(tileCoords.second);
			switch (connection.first) {
				case 0:
				case 1:
					--newRow;
					break;
				case 2:
				case 3:
					++newCol;
					break;
				case 4:
				case 5:
					++newRow;
					break;
				case 6:
				case 7:
					--newCol;
					break;
			}
			
			// inserting the new tile
			grid.at(newRow).at(newCol) = targetID;
			insertedPos[targetID] = pair<int, int>(newRow, newCol);
			inserted[targetID] = true;
			queue.push_back(targetID);
		}
		
		// removing the current queue item
		queue.erase(queue.begin());
	}

	// culling the edges of each tile
	for (auto it = tiles.begin(); it != tiles.end(); ++it) {
		cullEdges(it->second);
	}

	// assembling the larger picture
	vector<string> full;
	int rowsPerTile = (tiles.begin()->second).pic.size();
	for (auto row : grid) {
		for (int i = 0; i < rowsPerTile; ++i) {
			string currLine;
			for (int id : row) {
				currLine += tiles[id].pic.at(i);
			}
			full.push_back(currLine);
		}
	}

	// finding which pictures have monsters
	vector<string> monsterStr = {
		"                  1 ",
		"1    11    11    111",
		" 1  1  1  1  1  1   "
	};

	int maxIgnored = 0;
	for (int i = 0; i < 4; ++i) {
		int curr = ignorePattern(monsterStr, full);
		//cout << "ignored: " << curr << endl;
		if (curr > maxIgnored) {maxIgnored = curr;}
		rotCWText(full);
	}
	flipVertText(full);
	for (int i = 0; i < 4; ++i) {
		int curr = ignorePattern(monsterStr, full);
		//cout << "ignored: " << curr << endl;
		if (curr > maxIgnored) {maxIgnored = curr;}
		rotCWText(full);
	}

	int total = 0;
	for (auto row : full) {
		for (auto col : row) {
			if (col == '1') {
				++total;
			}
		}
	}

	int roughWater = total - maxIgnored;

	return roughWater;
}

void parseInputs(const vector<string> &input, map<int, Tile> &tiles) {
	Tile tile;
	for (string line : input) {
		if (line.empty()) {
			// new tile
			tiles[tile.id] = tile;

			tile.id = 0;
			tile.pic.clear();
		} else if (line.at(0) == 'T') {
			// new id
			tile.id = stoi(line.substr(5, 4));
		} else {
			// new line of picture
			string currLine = line;
			replace(currLine.begin(), currLine.end(), '.', '0');
			replace(currLine.begin(), currLine.end(), '#', '1');
			tile.pic.push_back(currLine);
		}
	}
}

void getEdgeVals(Tile &tile) {
	// Top
	string topStr = tile.pic.at(0);
	tile.edgeVals.push_back(stoi(topStr, nullptr, 2));
	reverse(topStr.begin(), topStr.end());
	tile.edgeVals.push_back(stoi(topStr, nullptr, 2));

	// Right
	string rightStr;
	for (string line : tile.pic) {
		rightStr += line.at(line.size() - 1);
	}
	tile.edgeVals.push_back(stoi(rightStr, nullptr, 2));
	reverse(rightStr.begin(), rightStr.end());
	tile.edgeVals.push_back(stoi(rightStr, nullptr, 2));

	// Bottom
	string bottomStr = tile.pic.at(tile.pic.size() - 1);
	reverse(bottomStr.begin(), bottomStr.end());
	tile.edgeVals.push_back(stoi(bottomStr, nullptr, 2));
	reverse(bottomStr.begin(), bottomStr.end());
	tile.edgeVals.push_back(stoi(bottomStr, nullptr, 2));

	// Left
	string leftStr;
	for (string line : tile.pic) {
		leftStr += line.at(0);
	}
	reverse(leftStr.begin(), leftStr.end());
	tile.edgeVals.push_back(stoi(leftStr, nullptr, 2));
	reverse(leftStr.begin(), leftStr.end());
	tile.edgeVals.push_back(stoi(leftStr, nullptr, 2));
}

void flipVert(Tile &tile, int &dir) {
	flipVertText(tile.pic);

	dir = ((8 - dir) + 5) % 8;
	vector<unsigned int> newEdgeVals(8);
	for (unsigned int i = 0; i < tile.edgeVals.size(); ++i) {
		newEdgeVals.at(((8 - i) + 5) % 8) = tile.edgeVals.at(i);
	}
	tile.edgeVals = newEdgeVals;
}

void flipVertText(vector<string> &text) {
	reverse(text.begin(), text.end());
}

void rotCW(Tile &tile, int &dir) {
	rotCWText(tile.pic);

	dir = (dir + 2) % 8;

	vector<unsigned int> newEdgeVals(8);
	for (unsigned int i = 0; i < tile.edgeVals.size(); ++i) {
		newEdgeVals.at((i + 2) % 8) = tile.edgeVals.at(i);
	}
	tile.edgeVals = newEdgeVals;
}

void rotCWText(vector<string> &text) {
	int width = text.at(0).size();
	int height = text.size();

	vector<string> newPic;
	for (int i = 0; i < height; ++i) {
		string newRow;
		for (int j = 0; j < width; ++j) {
			newRow += text.at(height - 1 - j).at(i);
		}
		newPic.push_back(newRow);
	}

	text = newPic;
}

void orientToFit(Tile &tile, int start, const int target) {
	int opTarget = (((target + 4) % 8) ^ 1);
	if ((start % 2) != (opTarget % 2)) {
		flipVert(tile, start);
	}
	while (start != opTarget) {
		rotCW(tile, start);
	}
}

pair<int, int> findConnection(const Tile &tile1, const Tile &tile2) {
	for (unsigned int i = 0; i < tile1.edgeVals.size(); ++i) {
		for (unsigned int j = 0; j < tile2.edgeVals.size(); ++j) {
			if (tile1.edgeVals.at(i) == tile2.edgeVals.at(j)) {
				return pair<int, int>(i, j);
			}
		}
	}
	return pair<int, int>(-1, -1);
}

void cullEdges(Tile &tile) {
	tile.pic.erase(tile.pic.begin());
	tile.pic.erase(tile.pic.end() - 1);

	for (unsigned int i = 0; i < tile.pic.size(); ++i) {
		tile.pic.at(i) = tile.pic.at(i).substr(1, tile.pic.at(i).size() - 2);
	}
}

int ignorePattern(const vector<string> &pattern, const vector<string> &pic) {
	unsigned int patHeight(pattern.size()), patWidth(pattern.at(0).size());
	vector< pair<int, int> > offsets;
	for (unsigned int i = 0; i < patHeight; ++i) {
		for (unsigned int j = 0; j < patWidth; ++j) {
			if (pattern.at(i).at(j) == '1') {
				offsets.push_back(pair<int, int>(i, j));
			}
		}
	}

	int totalRemoved = 0;
	unsigned int picHeight(pic.size()), picWidth(pic.at(0).size());
	for (unsigned int i = 0; i < (picHeight - patHeight); ++i) {
		for (unsigned int j = 0; j < (picWidth - patWidth); ++j) {
			bool couldWork = true;
			for (auto offset : offsets) {
				if (pic.at(i + offset.first).at(j + offset.second) != '1') {
					couldWork = false;
				}
			}
			if (couldWork) {totalRemoved += offsets.size();}
		}
	}

	return totalRemoved;
}
