// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cmath>
using namespace std;

// Our Files
#include "inputparse.h"

struct Planet {
	int pos[3];
	int vel[3] = {0, 0, 0};

	bool equals(const Planet &planet) {
		bool isEq = true;
		for (unsigned int i = 0; i < 3; ++i) {
			if (planet.pos[i] != pos[i]) {isEq = false;}
			if (planet.vel[i] != vel[i]) {isEq = false;}
		}
		return isEq;
	}
};

int partOne(const vector<string> &input);
long long int partTwo(const vector<string> &input);

vector<Planet> getPlanets(const vector<string> &input);
int getEnergy(const Planet &planet);
vector<Planet> selectDim(const vector<Planet> &planetVec, unsigned int dim);
long long int GCF(long long int a, long long int b);

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
	vector<Planet> planetVec = getPlanets(input);

	// (for 1000 steps)
	for (unsigned int n = 0; n < 1000; ++n) {
		// finding velocities
		for (unsigned int i = 0; i < planetVec.size(); ++i) {
			for (unsigned int j = i+1; j < planetVec.size(); ++j) {
				// going through each velocity component
				for (unsigned int k = 0; k < 3; ++k) {
					if (planetVec.at(i).pos[k] > planetVec.at(j).pos[k]) {
						--planetVec.at(i).vel[k];
						++planetVec.at(j).vel[k];
					} else if (planetVec.at(i).pos[k] < planetVec.at(j).pos[k]) {
						++planetVec.at(i).vel[k];
						--planetVec.at(j).vel[k];
					}
				}
			}
		}

		// applying velocities
		for (unsigned int i = 0; i < planetVec.size(); ++i) {
			for (unsigned int k = 0; k < 3; ++k) {
				planetVec.at(i).pos[k] += planetVec.at(i).vel[k];
			}
		}
	}

	// finding final energy
	int totalEnergy = 0;
	for (Planet planet : planetVec) {
		totalEnergy += getEnergy(planet);
	}
	
	return totalEnergy;
}

long long int partTwo(const vector<string> &input) {
	vector<Planet> planets = getPlanets(input);
	
	// finding how many steps it takes to repeat in each dimension separately
	int repeat[3];
	int offset[3];

	// for each dimension
	for (unsigned int d = 0; d < 3; ++d) {
		vector<Planet> planetVec = selectDim(planets, d);
		vector< vector<Planet> > prevStates = {planetVec};
		bool doRepeat = true;
		int currTwoPow = 0;

		while (doRepeat) {
			// finding velocities
			for (unsigned int i = 0; i < planetVec.size(); ++i) {
				for (unsigned int j = i+1; j < planetVec.size(); ++j) {
					if (planetVec.at(i).pos[d] > planetVec.at(j).pos[d]) {
						--planetVec.at(i).vel[d];
						++planetVec.at(j).vel[d];
					} else if (planetVec.at(i).pos[d] < planetVec.at(j).pos[d]) {
						++planetVec.at(i).vel[d];
						--planetVec.at(j).vel[d];
					}
				}
			}

			// applying velocities
			for (unsigned int i = 0; i < planetVec.size(); ++i) {
				for (unsigned int k = 0; k < 3; ++k) {
					planetVec.at(i).pos[k] += planetVec.at(i).vel[k];
				}
			}

			// seeing if this is a repeated position
			// also, since this is a very intensive check - O(n^2) - it's only done at increasing powers of two
			if (prevStates.size() == pow(2, currTwoPow)) {
				for (unsigned int n = 0; n < prevStates.size(); ++n) {
					bool isSame = true;
					for (unsigned int i = 0; i < planetVec.size(); ++i) {
						if (!planetVec.at(i).equals(prevStates.at(n).at(i))) {
							isSame = false;
						}
					}

					if (isSame) {
						// we got a match! now that we know the offset we just need to step backwards until we find the earliest match
						doRepeat = false;

						repeat[d] = prevStates.size() - n;
						prevStates.push_back(planetVec);

						int currOffset;
						for (currOffset = n; currOffset > 0; --currOffset) {
							vector<Planet> initialPlans = prevStates.at(currOffset);
							vector<Planet> repeatPlans = prevStates.at(currOffset + repeat[d]);

							bool equal = true;
							for (unsigned int i = 0; i < initialPlans.size(); ++i) {
								if (!initialPlans.at(i).equals(repeatPlans.at(i))) {
									equal = false;
									break;
								}
							}

							if (!equal) {
								++currOffset;
								break;
							}
						}
						offset[d] = currOffset;

						break;
					}
				}

				++currTwoPow;
			}

			// pushing the current position to the prevStates vector
			prevStates.push_back(planetVec);
		}
	}

	// now we just need to find where they all intersect
	if (offset[0] == 0 && offset[1] == 0 && offset[2] == 0) {
		// if the offset is zero, we can find the least common multiple
		long long int product = 1;
		for (unsigned int i = 0; i < 3; ++i) {
			long long int curr = repeat[i];
			long long int factor = GCF(product, curr);
			product = product * curr / factor;
		}

		return product;
	} else {
		// if the offset isn't zero, we have to do it more manually
		long long int intersection = 0;
		long long int currSteps[3];
		for (unsigned int i = 0; i < 3; ++i) {
			currSteps[i] = offset[i];
		}

		while (intersection == 0) {
			// finding which dimension is the most behind
			unsigned int minPos = 0;
			unsigned int minStep = currSteps[0];
			for (unsigned int i = 1; i < 3; ++i) {
				if (currSteps[i] < minStep) {
					minPos = i;
					minStep = currSteps[i];
				}
			}

			// taking a step in that dimension
			currSteps[minPos] += repeat[minPos];

			// checking if they're equal
			if (currSteps[0] == currSteps[1] && currSteps[1] == currSteps[2]) {
				intersection = currSteps[0];
			}
		}

		return intersection;
	}

	return 0; // (control should never reach here)
}

vector<Planet> getPlanets(const vector<string> &input) {
	vector<Planet> planetVec;

	regex regexp("<x=|, y=|, z=|>");
	for (string line : input) {
		vector<string> currProps = splitReg(line, regexp);

		Planet currPlanet;
		currPlanet.pos[0] = stoi(currProps.at(1));
		currPlanet.pos[1] = stoi(currProps.at(2));
		currPlanet.pos[2] = stoi(currProps.at(3));
		planetVec.push_back(currPlanet);
	}

	return planetVec;
}

int getEnergy(const Planet &planet) {
	int pot = 0;
	int kin = 0;
	for (unsigned int i = 0; i < 3; ++i) {
		pot += abs(planet.pos[i]);
		kin += abs(planet.vel[i]);
	}

	return pot * kin;
}

vector<Planet> selectDim(const vector<Planet> &planetVec, unsigned int dim) {
	vector<Planet> retVec = planetVec;

	for (unsigned int i = 0; i < retVec.size(); ++i) {
		for (unsigned int d = 0; d < 3; ++d) {
			if (d == dim) {continue;}
			retVec.at(i).pos[d] = 0;
		}
	}

	return retVec;
}

long long int GCF(long long int a, long long int b) {
	while (true) {
		a = a % b;
		if (a == 0) {return b;}
		b = b % a;
		if (b == 0) {return a;}
	}
}
