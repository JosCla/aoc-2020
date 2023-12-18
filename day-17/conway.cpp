// conway.cpp
// Implements conway.h.

// C++ Standard Library
#include <iostream>
using std::cout; using std::endl; using std::ostream;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <map>
using std::map;

// Our Files
#include "conway.h"

ConwayND::ConwayND(const vector<string> &input, const int nDim) {
	for (unsigned int y = 0; y < input.size(); ++y) {
		string row = input.at(y);

		for (unsigned int x = 0; x < row.size(); ++x) {
			if (row.at(x) == '#') {
				vector<int> currCoords(nDim, 0);
				currCoords.at(0) = x;
				currCoords.at(1) = y;

				set(currCoords, 1);
			}
		}
	}
}

void ConwayND::step() {
	map<vector<int>, int> changeMap;
	vector< vector<int> > delBoard;
	vector< vector<int> > delAdj;

	for (map<vector<int>, int>::iterator itr = adj.begin(); itr != adj.end(); ++itr) {
		vector<int> currCoords = itr->first;

		int boardVal = board[currCoords];
		if (boardVal == 0) {
			if (itr->second == 3) {
				changeMap[currCoords] = 1;
			} else {
				delBoard.push_back(currCoords);
			}
		} else {
			if (itr->second < 2 || itr->second > 3) {
				changeMap[currCoords] = 0;
			}
		}

		if (itr->second == 0 && board[itr->first] == 0) {
			delAdj.push_back(itr->first);
			delBoard.push_back(itr->first);
		}
	}

	for (vector<int> coord : delBoard) {
		board.erase(coord);
	}
	for (vector<int> coord : delAdj) {
		adj.erase(coord);
	}

	for (map<vector<int>, int>::const_iterator itr = changeMap.begin(); itr != changeMap.end(); ++itr) {
		set(itr->first, itr->second);
	}
}

int ConwayND::count() {
	int total = 0;
	for (map<vector<int>, int>::const_iterator itr = board.begin(); itr != board.end(); ++itr) {
		total += itr->second;
	}
	return total;
}

int ConwayND::get(const std::vector<int> &coords) {
	return board[coords];
}

void ConwayND::set(const std::vector<int> &coords, const int val) {
	int diff = val - board[coords];
	board[coords] = val;
	updateAdj(coords, diff);

	if (val == 0) {
		board.erase(coords);
	}
}

void ConwayND::updateAdj(std::vector<int> coords, const int val, const unsigned int ind) {
	if (ind == coords.size()) {
		adj[coords] += val;
	} else {
		--coords.at(ind);
		updateAdj(coords, val, ind + 1);
		++coords.at(ind);
		updateAdj(coords, val, ind + 1);
		++coords.at(ind);
		updateAdj(coords, val, ind + 1);
	}

	if (ind == 0) {
		--coords.at(ind);
		adj[coords] -= val;
	}
}

/*
Conway3D::Conway3D(const vector<string> &input) {
	vector< vector<bool> > currLayer;

	for (string row : input) {
		vector<bool> currRow;

		for (char col : row) {
			if (col == '#') {
				currRow.push_back(true);
			} else {
				currRow.push_back(false);
			}
		}
		currLayer.push_back(currRow);
	}

	board = {currLayer};

	dimZ = board.size();
	dimY = board.at(0).size();
	dimX = board.at(0).at(0).size();
}

void Conway3D::step() {
	vector< vector< vector<bool> > > changeVec;
	for (int i = 0; i < dimZ + 2; ++i) {
		vector< vector<bool> > currLayer;
		for (int j = 0; j < dimY + 2; ++j) {
			currLayer.push_back(vector<bool>(dimX + 2, false));
		}
		changeVec.push_back(currLayer);
	}

	bool expandX(false), expandY(false), expandZ(false);
	for (int z = -1; z <= dimZ; ++z) {
		for (int y = -1; y <= dimY; ++y) {
			for (int x = -1; x <= dimX; ++x) {
				bool startState = get(x, y, z);
				int surrounding = countSurrounding(x, y, z);

				if (startState == false && surrounding == 3) {
					changeVec.at(z+1).at(y+1).at(x+1) = true;
					if (x == -1) {expandX = true;}
					if (y == -1) {expandY = true;}
					if (z == -1) {expandZ = true;}
				} else if (startState == true && (surrounding < 2 || surrounding > 3)) {
					changeVec.at(z+1).at(y+1).at(x+1) = true;
				}
			}
		}
	}

	if (expandX) {expand(1);}
	if (expandY) {expand(3);}
	if (expandZ) {expand(5);}

	for (unsigned int z = 0; z < changeVec.size(); ++z) {
		for (unsigned int y = 0; y < changeVec.at(0).size(); ++y) {
			for (unsigned int x = 0; x < changeVec.at(0).at(0).size(); ++x) {
				if (changeVec.at(z).at(y).at(x) == true) {
					int boardX = x + (int)expandX - 1;
					int boardY = y + (int)expandY - 1;
					int boardZ = z + (int)expandZ - 1;

					bool newVal = !get(boardX, boardY, boardZ);
					set(boardX, boardY, boardZ, newVal);
				}
			}
		}
	}
}

int Conway3D::count() {
	int total = 0;

	for (auto layer : board) {
		for (auto row : layer) {
			for (auto col : row) {
				if (col) {++total;}
			}
		}
	}

	return total;
}

vector< vector< vector<bool> > > Conway3D::getBoard() {
	return board;
}

void Conway3D::printBoard(ostream &out) {
	for (auto layer : board) {
		for (auto row : layer) {
			for (auto col : row) {
				cout << col;
			}
			cout << endl;
		}
		cout << endl;
	}
}

bool Conway3D::get(const int x, const int y, const int z) {
	if (onGrid(x, y, z)) {
		return board.at(z).at(y).at(x);
	} else {
		return false;
	}
}

void Conway3D::set(int x, int y, int z, const bool val) {
	if (x >= dimX) {
		expand(0, (x-dimX) + 1);
	} else if (x < 0) {
		expand(1, -x);
		x = 0;
	}

	if (y >= dimY) {
		expand(2, (y-dimY) + 1);
	} else if (y < 0) {
		expand(3, -y);
		y = 0;
	}

	if (z >= dimZ) {
		expand(4, (z-dimZ) + 1);
	} else if (z < 0) {
		expand(5, -z);
		z = 0;
	}

	board.at(z).at(y).at(x) = val;
}

// 0:+x 1:-x 2:+y 3:-y 4:+z 5:-z
void Conway3D::expand(const int dir, const int amount) {
	switch (dir) {
		case 0:
			// +x
			for (int i = 0; i < amount; ++i) {
				for (int layer = 0; layer < dimZ; ++layer) {
					for (int row = 0; row < dimY; ++row) {
						board.at(layer).at(row).push_back(false);
					}
				}
			}
			++dimX;
			break;
		case 1:
			// -x
			for (int i = 0; i < amount; ++i) {
				for (int layer = 0; layer < dimZ; ++layer) {
					for (int row = 0; row < dimY; ++row) {
						board.at(layer).at(row).insert(board.at(layer).at(row).begin(), false);
					}
				}
			}
			++dimX;
			break;

		case 2:
			// +y
			for (int i = 0; i < amount; ++i) {
				for (int layer = 0; layer < dimZ; ++layer) {
					board.at(layer).push_back(vector<bool>(dimX, false));
				}
			}
			++dimY;
			break;
		case 3:
			// -y
			for (int i = 0; i < amount; ++i) {
				for (int layer = 0; layer < dimZ; ++layer) {
					board.at(layer).insert(board.at(layer).begin(), vector<bool>(dimX, false));
				}
			}
			++dimY;
			break;

		case 4:
			// +z
			for (int i = 0; i < amount; ++i) {
				vector< vector<bool> > newLayer;
				for (int row = 0; row < dimY; ++row) {
					newLayer.push_back(vector<bool>(dimX, false));
				}
				board.push_back(newLayer);
			}
			++dimZ;
			break;
		case 5:
			// -z
			for (int i = 0; i < amount; ++i) {
				vector< vector<bool> > newLayer;
				for (int row = 0; row < dimY; ++row) {
					newLayer.push_back(vector<bool>(dimX, false));
				}
				board.insert(board.begin(), newLayer);
			}
			++dimZ;
			break;

		default:
			break;
	}
}

bool Conway3D::onGrid(const int x, const int y, const int z) {
	bool oobX = (x < 0 || x >= dimX);
	bool oobY = (y < 0 || y >= dimY);
	bool oobZ = (z < 0 || z >= dimZ);
	return !(oobX || oobY || oobZ);
}

int Conway3D::countSurrounding(const int x, const int y, const int z) {
	int total = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			for (int k = -1; k <= 1; ++k) {
				if (i == 0 && j == 0 && k == 0) {continue;}
				if (get(x+k, y+j, z+i)) {
					++total;
				}
			}
		}
	}
	return total;
}

Conway4D::Conway4D(const vector<string> &input) {
	vector< vector<bool> > currLayer;

	for (string row : input) {
		vector<bool> currRow;

		for (char col : row) {
			if (col == '#') {
				currRow.push_back(true);
			} else {
				currRow.push_back(false);
			}
		}
		currLayer.push_back(currRow);
	}

	auto box = {currLayer};
	board = {box};

	dimW = board.size();
	dimZ = board.at(0).size();
	dimY = board.at(0).at(0).size();
	dimX = board.at(0).at(0).at(0).size();
}

void Conway4D::step() {
	vector< vector< vector< vector<bool> > > > changeVec;
	for (int n = 0; n < dimW + 2; ++n) {
		vector< vector< vector<bool> > > currBox;
		for (int i = 0; i < dimZ + 2; ++i) {
			vector< vector<bool> > currLayer;
			for (int j = 0; j < dimY + 2; ++j) {
				currLayer.push_back(vector<bool>(dimX + 2, false));
			}
			currBox.push_back(currLayer);
		}
		changeVec.push_back(currBox);
	}

	bool expandX(false), expandY(false), expandZ(false), expandW(false);
	for (int w = -1; w <= dimW; ++w) {
		for (int z = -1; z <= dimZ; ++z) {
			for (int y = -1; y <= dimY; ++y) {
				for (int x = -1; x <= dimX; ++x) {
					bool startState = get(x, y, z, w);
					int surrounding = countSurrounding(x, y, z, w);

					if (startState == false && surrounding == 3) {
						changeVec.at(w+1).at(z+1).at(y+1).at(x+1) = true;
						if (x == -1) {expandX = true;}
						if (y == -1) {expandY = true;}
						if (z == -1) {expandZ = true;}
						if (w == -1) {expandW = true;}
					} else if (startState == true && (surrounding < 2 || surrounding > 3)) {
						changeVec.at(w+1).at(z+1).at(y+1).at(x+1) = true;
					}
				}
			}
		}
	}

	if (expandX) {expand(1);}
	if (expandY) {expand(3);}
	if (expandZ) {expand(5);}
	if (expandW) {expand(7);}

	for (unsigned int w = 0; w < changeVec.size(); ++w) {
		for (unsigned int z = 0; z < changeVec.at(0).size(); ++z) {
			for (unsigned int y = 0; y < changeVec.at(0).at(0).size(); ++y) {
				for (unsigned int x = 0; x < changeVec.at(0).at(0).at(0).size(); ++x) {
					if (changeVec.at(w).at(z).at(y).at(x) == true) {
						int boardX = x + (int)expandX - 1;
						int boardY = y + (int)expandY - 1;
						int boardZ = z + (int)expandZ - 1;
						int boardW = w + (int)expandW - 1;

						bool newVal = !get(boardX, boardY, boardZ, boardW);
						set(boardX, boardY, boardZ, boardW, newVal);
					}
				}
			}
		}
	}
}

int Conway4D::count() {
	int total = 0;

	for (auto box : board) {
		for (auto layer : box) {
			for (auto row : layer) {
				for (auto col : row) {
					if (col) {++total;}
				}
			}
		}
	}

	return total;
}

vector< vector< vector< vector<bool> > > > Conway4D::getBoard() {
	return board;
}

void Conway4D::printBoard(ostream &out) {
	for (auto box : board) {
		for (auto layer : box) {
			for (auto row : layer) {
				for (auto col : row) {
					cout << col;
				}
				cout << endl;
			}
			cout << endl;
		}
		cout << endl;
	}
}

bool Conway4D::get(const int x, const int y, const int z, const int w) {
	if (onGrid(x, y, z, w)) {
		return board.at(w).at(z).at(y).at(x);
	} else {
		return false;
	}
}

void Conway4D::set(int x, int y, int z, int w, const bool val) {
	if (x >= dimX) {
		expand(0, (x-dimX) + 1);
	} else if (x < 0) {
		expand(1, -x);
		x = 0;
	}

	if (y >= dimY) {
		expand(2, (y-dimY) + 1);
	} else if (y < 0) {
		expand(3, -y);
		y = 0;
	}

	if (z >= dimZ) {
		expand(4, (z-dimZ) + 1);
	} else if (z < 0) {
		expand(5, -z);
		z = 0;
	}

	if (w >= dimW) {
		expand(6, (w-dimW) + 1);
	} else if (w < 0) {
		expand(7, -w);
		w = 0;
	}

	board.at(w).at(z).at(y).at(x) = val;
}

// 0:+x 1:-x 2:+y 3:-y 4:+z 5:-z 6:+w 7:-w
void Conway4D::expand(const int dir, const int amount) {
	switch (dir) {
		case 0:
			// +x
			for (int i = 0; i < amount; ++i) {
				for (int box = 0; box < dimW; ++box) {
					for (int layer = 0; layer < dimZ; ++layer) {
						for (int row = 0; row < dimY; ++row) {
							board.at(box).at(layer).at(row).push_back(false);
						}
					}
				}
			}
			++dimX;
			break;
		case 1:
			// -x
			for (int i = 0; i < amount; ++i) {
				for (int box = 0; box < dimW; ++box) {
					for (int layer = 0; layer < dimZ; ++layer) {
						for (int row = 0; row < dimY; ++row) {
							board.at(box).at(layer).at(row).insert(board.at(box).at(layer).at(row).begin(), false);
						}
					}
				}
			}
			++dimX;
			break;

		case 2:
			// +y
			for (int i = 0; i < amount; ++i) {
				for (int box = 0; box < dimW; ++box) {
					for (int layer = 0; layer < dimZ; ++layer) {
						board.at(box).at(layer).push_back(vector<bool>(dimX, false));
					}
				}
			}
			++dimY;
			break;
		case 3:
			// -y
			for (int i = 0; i < amount; ++i) {
				for (int box = 0; box < dimW; ++box) {
					for (int layer = 0; layer < dimZ; ++layer) {
						board.at(box).at(layer).insert(board.at(box).at(layer).begin(), vector<bool>(dimX, false));
					}
				}
			}
			++dimY;
			break;

		case 4:
			// +z
			for (int i = 0; i < amount; ++i) {
				for (int box = 0; box < dimW; ++box) {
					vector< vector<bool> > newLayer;
					for (int row = 0; row < dimY; ++row) {
						newLayer.push_back(vector<bool>(dimX, false));
					}
					board.at(box).push_back(newLayer);
				}
			}
			++dimZ;
			break;
		case 5:
			// -z
			for (int i = 0; i < amount; ++i) {
				for (int box = 0; box < dimW; ++box) {
					vector< vector<bool> > newLayer;
					for (int row = 0; row < dimY; ++row) {
						newLayer.push_back(vector<bool>(dimX, false));
					}
					board.at(box).insert(board.at(box).begin(), newLayer);
				}
			}
			++dimZ;
			break;

		case 6:
			// +w
			for (int i = 0; i < amount; ++i) {
				vector< vector< vector<bool> > > newBox;
				for (int layer = 0; layer < dimZ; ++layer) {
					vector< vector<bool> > newLayer;
					for (int row = 0; row < dimY; ++row) {
						newLayer.push_back(vector<bool>(dimX, false));
					}
					newBox.push_back(newLayer);
				}
				board.push_back(newBox);
			}
			++dimW;
			break;
		case 7:
			// -w
			for (int i = 0; i < amount; ++i) {
				vector< vector< vector<bool> > > newBox;
				for (int layer = 0; layer < dimZ; ++layer) {
					vector< vector<bool> > newLayer;
					for (int row = 0; row < dimY; ++row) {
						newLayer.push_back(vector<bool>(dimX, false));
					}
					newBox.push_back(newLayer);
				}
				board.insert(board.begin(), newBox);
			}
			++dimW;
			break;

		default:
			break;
	}
}

bool Conway4D::onGrid(const int x, const int y, const int z, const int w) {
	bool oobX = (x < 0 || x >= dimX);
	bool oobY = (y < 0 || y >= dimY);
	bool oobZ = (z < 0 || z >= dimZ);
	bool oobW = (w < 0 || w >= dimW);
	return !(oobX || oobY || oobZ || oobW);
}

int Conway4D::countSurrounding(const int x, const int y, const int z, const int w) {
	int total = 0;
	for (int n = -1; n <= 1; ++n) {
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				for (int k = -1; k <= 1; ++k) {
					if (n == 0 && i == 0 && j == 0 && k == 0) {continue;}
					if (get(x+k, y+j, z+i, w+n)) {
						++total;
					}
				}
			}
		}
	}
	return total;
}
*/
