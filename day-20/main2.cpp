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

#define llint long long int

struct Tile {
	int id;
	vector<string> pic;
	vector<unsigned int> edgeVals;
};

llint partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

void parseInputs(const vector<string> &input, vector<Tile> &tiles);
void getEdgeVals(Tile &tile);

void flipVert(Tile &tile, int &dir);
void rotCW(Tile &tile, int &dir);
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

llint partOne(const vector<string> &input) {
	vector<Tile> tiles;
	parseInputs(input, tiles);
	for (unsigned int i = 0; i < tiles.size(); ++i) {
		getEdgeVals(tiles.at(i));
	}

	llint prod = 1;
	for (unsigned int i = 0; i < tiles.size(); ++i) {
		Tile tile1 = tiles.at(i);
		vector<int> connectVec;
		for (unsigned int j = 0; j < tiles.size(); ++j) {
			if (i == j) {continue;}
			Tile tile2 = tiles.at(j);
			for (unsigned int val1 : tile1.edgeVals) {
				for (unsigned int val2 : tile2.edgeVals) {
					if (val1 == val2) {
						connectVec.push_back(tile2.id);
					}
				}
			}
		}
		if (connectVec.size() == 4) {prod *= tile1.id;}
	}

	return prod;
}

int partTwo(const vector<string> &input) {
	vector<Tile> tiles;
	parseInputs(input, tiles);
	for (unsigned int i = 0; i < tiles.size(); ++i) {
		getEdgeVals(tiles.at(i));
	}

	int width = sqrt(tiles.size());
	int height = width;
	vector< vector<Tile> > grid;
	for (int i = 0; i < height; ++i) {
		vector<Tile> row;
		for (int j = 0; j < width; ++j) {
			row.push_back(Tile());
		}
		grid.push_back(row);
	}
	vector<int> inserted;
	map< int, vector<int> > connections;

	// finding all connections, inserting the upper-left corner
	for (Tile tile1 : tiles) {
		vector<int> connectionNums;

		for (Tile tile2 : tiles) {
			if (tile1.id == tile2.id) {continue;}

			pair<int, int> connection = findConnection(tile1, tile2);
			if (connection.first != -1) {
				connections[tile1.id].push_back(tile2.id);
				connectionNums.push_back(connection.first);
			}
		}

		if (connections[tile1.id].size() == 2 && inserted.size() == 0) {
			// reorienting the tile
			int connectMin = min(connectionNums.at(0), connectionNums.at(1));
			int connectMax = max(connectionNums.at(0), connectionNums.at(1));
			if (connectMin % 2 != 0) {--connectMin;}
			if (connectMax % 2 != 0) {--connectMax;}
			while (connectMin != 2) {
				rotCW(tile1, connectMin);
			}
			if (connectMax == 6) {rotCW(tile1, connectMax);}

			// inserting the tile
			grid.at(0).at(0) = tile1;
			inserted.push_back(tile1.id);
		}
	}

	
	// completing the puzzle
	vector<int> queue = {inserted.at(0)};
	while (queue.size() > 0) {
		int currID = queue.at(0);
		//cout << currID << endl;
		vector<int> currConnections = connections[currID];
		//cout << "con to: " << currConnections.size() << endl;
		
		Tile currTile;
		currTile.id = -1;
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				if (grid.at(i).at(j).id == currID) {
					currTile = grid.at(i).at(j);
				}
				if (currTile.id != -1) {break;}
			}
			if (currTile.id != -1) {break;}
		}
		
		int currTileRow(-1), currTileCol(-1);
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				if (grid.at(i).at(j).id == currID) {
					currTileRow = i;
					currTileCol = j;
				}
				if (currTileRow != -1) {break;}
			}
			if (currTileRow != -1) {break;}
		}

		for (int id : currConnections) {
			if (find(inserted.begin(), inserted.end(), id) != inserted.end()) {continue;}
			//cout << "(" << id << ")" << endl;
			Tile targetTile;
			for (unsigned int i = 0; i < tiles.size(); ++i) {
				if (tiles.at(i).id == id) {
					targetTile = tiles.at(i);
					break;
				}
			}

			pair<int, int> connection = findConnection(currTile, targetTile);
			//cout << "con: " << connection.first << ", " << connection.second << endl;
			orientToFit(targetTile, connection.second, connection.first);

			switch (connection.first) {
				case 0:
				case 1:
					grid.at(currTileRow - 1).at(currTileCol) = targetTile;
					break;
				case 2:
				case 3:
					grid.at(currTileRow).at(currTileCol + 1) = targetTile;
					break;
				case 4:
				case 5:
					grid.at(currTileRow + 1).at(currTileCol) = targetTile;
					break;
				case 6:
				case 7:
					grid.at(currTileRow).at(currTileCol - 1) = targetTile;
					break;
			}
			inserted.push_back(id);
			queue.push_back(id);
		}
		queue.erase(queue.begin());
	}

	/*
	for (auto row : grid) {
		for (auto col : row) {
			cout << col.id << " ";
		}
		cout << endl;
	}
	*/

	// culling the grid, assembling the larger picture
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			cullEdges(grid.at(i).at(j));
		}
	}

	vector<string> largePic;
	for (auto row : grid) {
		for (unsigned int i = 0; i < row.at(0).pic.size(); ++i) {
			string currLine;
			for (unsigned int j = 0; j < row.size(); ++j) {
				currLine += row.at(j).pic.at(i);
			}
			largePic.push_back(currLine);
		}
	}
	Tile full;
	full.pic = largePic;
	full.edgeVals = vector<unsigned int>(8, 0);

	vector<string> monsterStr = {
		"                  1 ",
		"1    11    11    111",
		" 1  1  1  1  1  1   "
	};

	int maxIgnored = 0;
	for (int i = 0; i < 4; ++i) {
		int curr = ignorePattern(monsterStr, full.pic);
		//cout << "ignored: " << curr << endl;
		if (curr > maxIgnored) {maxIgnored = curr;}
		int dummy = 0;
		rotCW(full, dummy);
	}
	int dummy2 = 0;
	flipVert(full, dummy2);
	for (int i = 0; i < 4; ++i) {
		int curr = ignorePattern(monsterStr, full.pic);
		//cout << "ignored: " << curr << endl;
		if (curr > maxIgnored) {maxIgnored = curr;}
		int dummy = 0;
		rotCW(full, dummy);
	}

	int total = 0;
	for (auto row : full.pic) {
		for (auto col : row) {
			if (col == '1') {
				++total;
			}
		}
	}

	return total - maxIgnored;
}

void parseInputs(const vector<string> &input, vector<Tile> &tiles) {
	Tile tile;
	for (string line : input) {
		if (line.empty()) {
			// new tile
			tiles.push_back(tile);

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
	reverse(tile.pic.begin(), tile.pic.end());

	dir = ((8 - dir) + 5) % 8;
	vector<unsigned int> newEdgeVals(8);
	for (unsigned int i = 0; i < tile.edgeVals.size(); ++i) {
		newEdgeVals.at(((8 - i) + 5) % 8) = tile.edgeVals.at(i);
	}
	tile.edgeVals = newEdgeVals;
}

void rotCW(Tile &tile, int &dir) {
	int width = tile.pic.at(0).size();
	int height = tile.pic.size();

	vector<string> newPic;
	for (int i = 0; i < height; ++i) {
		string currRow;
		for (int j = 0; j < width; ++j) {
			currRow += tile.pic.at(height-1-j).at(i);
		}
		newPic.push_back(currRow);
	}
	tile.pic = newPic;

	dir = (dir + 2) % 8;

	vector<unsigned int> newEdgeVals(8);
	for (unsigned int i = 0; i < tile.edgeVals.size(); ++i) {
		newEdgeVals.at((i + 2) % 8) = tile.edgeVals.at(i);
	}
	tile.edgeVals = newEdgeVals;
}

void orientToFit(Tile &tile, int start, const int target) {
	int opTarget = (target + 4) % 8;
	opTarget = (opTarget ^ 1);
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
