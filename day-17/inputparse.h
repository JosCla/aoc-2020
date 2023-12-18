#ifndef INPUTPARSE_H
#define INPUTPARSE_H
// inputparse.h
// Contains functions for parsing the input for advent of code problems.

// C++ Standard Library
#include <string>
#include <vector>
#include <regex>

// Reads the contents of a file, and returns a vector containing each line of the file. Returns an empty vector if the file fails to open.
std::vector<std::string> readFileToLines(const std::string filepath);

// Splits a string across each match from a given regex.
std::vector<std::string> splitReg(std::string input, const std::regex &regexp);

// Splits a string across a given string.
std::vector<std::string> split(const std::string &input, const std::string &spl);

// Converts a vector of strings to a vector of integers with stoi.
std::vector<int> vecStrToInt(const std::vector<std::string> &strs);

// Converts a vector of strings to a vector of long long ints with stoll.
std::vector<long long int> vecStrToLLInt(const std::vector<std::string> &strs);

#endif // INPUTPARSE_H
