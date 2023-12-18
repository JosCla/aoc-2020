// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

long long int partOne(const vector<string> &input);
long long int partTwo(const vector<string> &input);

long long int eval(const string &expr);
long long int advEval(const string &expr);

int findMatching(const string &expr, const int openParen);
void applyOp(long long int &left, const long long int right, const char op);

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
	long long int sum = 0;
	for (string line : input) {
		long long int curr = eval(line);
		sum += curr;
	}
	return sum;
}

long long int partTwo(const vector<string> &input) {
	long long int sum = 0;
	for (string line : input) {
		long long int curr = advEval(line);
		sum += curr;
	}
	return sum;
}

long long int eval(const string &expr) {
	vector<long long int> nums;
	vector<char> ops;
	
	// getting numbers and operands
	for (unsigned int i = 0; i < expr.size(); i += 2) {
		if (expr.at(i) == '(') {
			// new inner expression
			int closeParen = findMatching(expr, i);

			nums.push_back(eval(expr.substr(i + 1, closeParen - i - 1)));
			i = closeParen;
		} else if (expr.at(i) == '+' || expr.at(i) == '*') {
			// new operand
			ops.push_back(expr.at(i));
		} else {
			// new normal value
			nums.push_back(expr.at(i) - '0');
		}
	}

	// applying all operands
	while (ops.size()) {
		applyOp(nums.at(1), nums.at(0), ops.at(0));
		nums.erase(nums.begin());
		ops.erase(ops.begin());
	}

	return nums.at(0);
}

long long int advEval(const string &expr) {
	vector<long long int> nums;
	vector<char> ops;
	
	// getting all numbers and operands
	for (unsigned int i = 0; i < expr.size(); i += 2) {
		if (expr.at(i) == '(') {
			// new inner expression
			int closeParen = findMatching(expr, i);

			nums.push_back(advEval(expr.substr(i + 1, closeParen - i - 1)));
			i = closeParen;
		} else if (expr.at(i) == '+' || expr.at(i) == '*') {
			// new operand
			ops.push_back(expr.at(i));
		} else {
			// new normal value
			nums.push_back(expr.at(i) - '0');
		}
	}

	// addition
	for (unsigned int i = 0; i < ops.size(); ++i) {
		if (ops.at(i) != '+') {continue;}

		applyOp(nums.at(i+1), nums.at(i), '+');
		nums.erase(nums.begin() + i);
		ops.erase(ops.begin() + i);
		--i;
	}
	
	// multiplication
	for (unsigned int i = 0; i < ops.size(); ++i) {
		if (ops.at(i) != '*') {continue;}

		applyOp(nums.at(i+1), nums.at(i), '*');
		nums.erase(nums.begin() + i);
		ops.erase(ops.begin() + i);
		--i;
	}

	return nums.at(0);
}

int findMatching(const string &expr, const int openParen) {
	int depth = 0;
	for (unsigned int i = openParen; i < expr.size(); ++i) {
		if (expr.at(i) == '(') {++depth;}
		if (expr.at(i) == ')') {--depth;}
		if (depth == 0) {return i;}
	}
	return -1;
}

void applyOp(long long int &left, const long long int right, const char op) {
	switch (op) {
		case '+':
			left += right;
			break;
		case '*':
			left *= right;
			break;
	}
}
