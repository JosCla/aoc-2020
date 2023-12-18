// C++ Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// Our Files
#include "inputparse.h"

struct Command {
	string cmd;
	vector<string> args;
};

int partOne(const vector<string> &input);
int partTwo(const vector<string> &input);

Command lineToCommand(const string &line);
void parseCommand(const Command &command, int &accumulator, int &ip);
int doesTerminate(const vector<Command> &commands, int &accumulator, int &ip);

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
	int accumulator(0), ip(0);
	vector<bool> visited(input.size(), false);

	bool revisited = false;	
	while (!revisited) {
		visited.at(ip) = true;

		Command command = lineToCommand(input.at(ip));
		parseCommand(command, accumulator, ip);
		++ip;

		if (visited.at(ip) == true) {
			revisited = true;
		}
	}

	return accumulator;
}

int partTwo(const vector<string> &input) {
	int accumulator, ip;
	vector<Command> commands;
	for (string line : input) {
		commands.push_back(lineToCommand(line));
	}

	for (unsigned int i = 0; i < commands.size(); ++i) {
		if (commands.at(i).cmd == "acc") {continue;}
		accumulator = 0;
		ip = 0;
		vector<Command> commandsCopy = commands;

		if (commandsCopy.at(i).cmd == "jmp") {
			commandsCopy.at(i).cmd = "nop";
		} else if (commandsCopy.at(i).cmd == "nop") {
			commandsCopy.at(i).cmd = "jmp";
		}

		int status = doesTerminate(commandsCopy, accumulator, ip);
		if (status == 1) {
			return accumulator;
		}
	}

	return 0;
}

Command lineToCommand(const string &line) {
	Command command;
	command.cmd = line.substr(0, 3);
	command.args.push_back(line.substr(4));
	return command;
}

void parseCommand(const Command &command, int &accumulator, int &ip) {
	if (command.cmd == "acc") {
		accumulator += stoi(command.args.at(0));
	} else if (command.cmd == "jmp") {
		ip += stoi(command.args.at(0));
		--ip;
	}
	// do nothing for nop
}

// returns -1 for no terminate, -2 for bad instruction, 1 for successful termination
int doesTerminate(const vector<Command> &commands, int &accumulator, int &ip) {
	vector<bool> visited(commands.size(), false);

	while (true) {
		visited.at(ip) = true;

		parseCommand(commands.at(ip), accumulator, ip);
		++ip;

		if (ip > commands.size()) {
			return -2;
		} else if (ip == commands.size()) {
			return 1;
		} else if (visited.at(ip) == true) {
			return -1;
		}
	}

	return -1; // (control should never reach here)
}
