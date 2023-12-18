#ifndef CONWAY_H
#define CONWAY_H
// conway.h
// Contains the ConwayND class, which simulates higher-dimensional Conway's Game of Life

// C++ Standard Library
#include <iostream>
#include <vector>
#include <string>
#include <map>

class ConwayND {
public:
	ConwayND(const std::vector<std::string> &input, const int nDim);

	void step();
	int count();

	int get(const std::vector<int> &coords);
	void set(const std::vector<int> &coords, const int val);

private:
	void updateAdj(std::vector<int> coords, const int val, const unsigned int ind = 0);

	std::map<std::vector<int>, int> board;
	std::map<std::vector<int>, int> adj;
};

#endif // CONWAY_H
