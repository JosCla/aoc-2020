// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
using namespace std;

// Our Files
#include "inputparse.h"

struct Recipe {
	vector<string> ingredients;
	vector<string> allergens;
};

int partOne(const vector<string> &input);
string partTwo(const vector<string> &input);

void inputParse(const vector<string> &input, vector<Recipe> &recipes);

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
	vector<Recipe> recipes;
	inputParse(input, recipes);

	// finding all possible allergen ingredients
	map< string, vector<string> > allergenIngreds;

	for (Recipe recipe : recipes) {
		for (string allergen : recipe.allergens) {
			auto allerPos = allergenIngreds.find(allergen);

			if (allerPos == allergenIngreds.end()) {
				// new allergen
				allergenIngreds[allergen] = recipe.ingredients;

			} else {
				// compare with old ingredients to see which are possible
				vector<string> possible = allerPos->second;

				for (unsigned int i = 0; i < possible.size(); ++i) {
					if (find(recipe.ingredients.begin(), recipe.ingredients.end(), possible.at(i)) == recipe.ingredients.end()) {
						// since this ingredient can't be found in both recipes with the allergen, it can't be the allergen
						possible.erase(possible.begin() + i);
						--i;
					}
				}

				allerPos->second = possible;
			}
		}
	}

	// using logic stuffs to find which allergen is which
	map<string, string> allergens;

	while (allergenIngreds.size() > 0) {
		vector<string> toErase;

		for (auto it = allergenIngreds.begin(); it != allergenIngreds.end(); ++it) {
			if ((it->second).size() == 1) {
				string currAller = (it->second).at(0);

				for (auto it2 = allergenIngreds.begin(); it2 != allergenIngreds.end(); ++it2) {
					for (unsigned int i = 0; i < (it2->second).size(); ++i) {
						if ((it2->second).at(i) == currAller) {
							(it2->second).erase((it2->second).begin() + i);
						}
					}
				}

				allergens[currAller] = it->first;
				toErase.push_back(it->first);
			}
		}

		for (string key : toErase) {
			allergenIngreds.erase(key);
		}
	}

	// counting all words that aren't allergens
	int total = 0;
	for (Recipe recipe : recipes) {
		for (string word : recipe.ingredients) {
			if (allergens.find(word) == allergens.end()) {
				++total;
			}
		}
	}

	return total;
}

string partTwo(const vector<string> &input) {
	vector<Recipe> recipes;
	inputParse(input, recipes);

	// finding all possible allergen ingredients
	map< string, vector<string> > allergenIngreds;

	for (Recipe recipe : recipes) {
		for (string allergen : recipe.allergens) {
			auto allerPos = allergenIngreds.find(allergen);

			if (allerPos == allergenIngreds.end()) {
				// new allergen
				allergenIngreds[allergen] = recipe.ingredients;

			} else {
				// compare with old ingredients to see which are possible
				vector<string> possible = allerPos->second;

				for (unsigned int i = 0; i < possible.size(); ++i) {
					if (find(recipe.ingredients.begin(), recipe.ingredients.end(), possible.at(i)) == recipe.ingredients.end()) {
						// since this ingredient can't be found in both recipes with the allergen, it can't be the allergen
						possible.erase(possible.begin() + i);
						--i;
					}
				}

				allerPos->second = possible;
			}
		}
	}

	// using logic stuffs to find which allergen is which
	map<string, string> allergens;

	while (allergenIngreds.size() > 0) {
		vector<string> toErase;

		for (auto it = allergenIngreds.begin(); it != allergenIngreds.end(); ++it) {
			if ((it->second).size() == 1) {
				string currAller = (it->second).at(0);

				for (auto it2 = allergenIngreds.begin(); it2 != allergenIngreds.end(); ++it2) {
					for (unsigned int i = 0; i < (it2->second).size(); ++i) {
						if ((it2->second).at(i) == currAller) {
							(it2->second).erase((it2->second).begin() + i);
						}
					}
				}

				allergens[it->first] = currAller;
				toErase.push_back(it->first);
			}
		}

		for (string key : toErase) {
			allergenIngreds.erase(key);
		}
	}

	// creating the danger list
	string dangerList;
	for (auto it = allergens.begin(); it != allergens.end(); ++it) {
		dangerList += it->second;
		dangerList += ',';
	}
	dangerList = dangerList.substr(0, dangerList.size() - 1);

	return dangerList;
}

void inputParse(const vector<string> &input, vector<Recipe> &recipes) {
	for (string line : input) {
		Recipe recipe;
		
		vector<string> lineSpl = split(line, " ");
		for (unsigned int i = 0; i < lineSpl.size(); ++i) {
			lineSpl.at(i) = ignoreChars(lineSpl.at(i), "(,)");
		}

		int readMode = 0;
		for (string word : lineSpl) {
			if (word == "contains") {
				readMode = 1;
				continue;
			}

			switch (readMode) {
			case 0:
				// ingredient
				recipe.ingredients.push_back(word);
				break;
			case 1:
				// allergen
				recipe.allergens.push_back(word);
				break;
			}
		}

		recipes.push_back(recipe);
	}
}
