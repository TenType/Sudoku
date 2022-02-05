#include <iostream>
#include <ctime>
using namespace std;

#include "sudoku.hpp"

void command(string input);
void setup();
void play();
int prompt(string prompt, string error, int min, int max);
bool isNumber(string n);

bool tutorial[] = {
	true, // hints for cells that can't be changed
	true, // hints for erasing cells
};

Sudoku sudoku;

int main() {
	// clear terminal
	system("clear");

	string input;
	cout << "Welcome to Sudoku\n\nSudoku is a game where the objective is to fill in a grid to make it so that there are no duplicate numbers in every row, column, and box!\n\nType 'play' to start a new Sudoku game.\n\n";
	while (true) {
		cout << "> ";
		cin >> input;
		command(input);
	}

	return 0;
}

void command(string input) {
	if (input == "play") {
		setup();
		play();
	} else {
		cout << "🛑 error: Unrecognized command '" + input + "'\n\n";
	}
	// scalable: more commands can be added...
}

void setup() {
	sudoku.victory = false;

	// ask user if they want to see visible errors
	cout << "🔍 Would you like the program to automatically check for visible errors in your game? (y/n)\n\n";
	bool askInput = true;
	while (askInput) {
		string input;
		cout << "> ";
		cin >> input;

		askInput = false;
		if (input == "y" || input == "yes" || input == "true") {
			sudoku.showErrors = true;
		} else if (input == "n" || input == "no" || input == "false") {
			sudoku.showErrors = false;
		} else {
			cout << "🛑 error: Please input either yes (y) or no (n).\n\n";
			askInput = true;
		}
	}

	// prepare level menu
	string easy;
	string medium;
	string hard;

	for (int i = 1; i <= 10; i++) easy += to_string(i) + "  ";
	for (int i = 11; i <= 20; i++) medium += to_string(i) + " ";
	for (int i = 21; i <= 30; i++) hard += to_string(i) + " ";

	cout << "🧠 Choose a level to play on:\n\nEasy    | " + easy + "\nMedium  | " + medium + "\nHard    | " + hard + "\n...or, type 'random' for a random level.\n\n";

	// choose a level
	askInput = true;
	while (askInput) {
		string input;
		cout << "> ";
		cin >> input;

		askInput = false;

		if (isNumber(input)) {
			// load level number
			int level = stoi(input);
			if (level > 0 && level < 11) {
				sudoku.load(0, level - 1);
			} else if (level > 10 && level < 21) {
				sudoku.load(1, level - 11);
			} else if (level > 20 && level < 31) {
				sudoku.load(2, level - 21);
			} else {
				// bad input
				cout << "🛑 error: Please include a level number from 1-30 or 'random' for a random level.\n\n";
				askInput = true;
			}
		} else {
			// handle when input is string
			if (input == "random") {
				// random level
				srand(time(NULL));
				sudoku.load(rand() % 3, rand() % 10);
			} else {
				// bad input
				cout << "🛑 error: Please include a level number from 1-30 or 'random' for a random level.\n\n";
				askInput = true;
			}
		}
	}
}

void play() {
	// clear terminal
	system("clear");

	cout << sudoku.format() + "\n\n"; 
	cout << "🏁 Your game has now started!\n\nTo fill in a cell, input the X and Y of a cell and the number you wish to fill in.\n\n";
	while (!sudoku.victory) {
		// coordinate inputs
		int x = prompt("Input X (1-9) > ", "🛑 error: You must input a valid X coordinate from 1 to 9.\n\n", 1, 9);
		int y = prompt("Input Y (1-9) > ", "🛑 error: You must input a valid Y coordinate from 1 to 9.\n\n", 1, 9);

		// disallow user from filling number that is part of the puzzle
		if (sudoku.original[y - 1][x - 1] != 0) {
			cout << "🛑 error: The number at " + to_string(x) + ", " + to_string(y) + " has a number (" + to_string(sudoku.original[y - 1][x - 1]) + ") that is part of the puzzle and can't be changed.\n";
			if (tutorial[0]) {
				cout << "💡 hint: Numbers that are part of the puzzle are enclosed by brackets []\n\n";
				tutorial[0] = false;
			}
			continue;
		}

		// number to fill
		int value = prompt("Input number to fill > ", "🛑 error: You must input a number from 1 to 9 or 0 to erase a cell.\n\n", 0, 9);

		// clear terminal
		system("clear");

		// fill cell
		bool errors = sudoku.fill(y - 1, x - 1, value);
		cout << sudoku.format() + "\n\n";
		cout << "📝 Filled " + to_string(x) + ", " + to_string(y) + " with value " + to_string(value) + "\n\n";

		// if display error is on, show if there are any errors
		if (errors && sudoku.showErrors) {
			cout << "🔍 Sudoku checker detected visible error(s), make sure your board does not have any duplicate rows, columns, or boxes!\n\n";
			if (tutorial[1]) {
				cout << "💡 hint: To erase a cell, fill it with the number '0'\n\n";
				tutorial[1] = false;
			}
		}
	}
	// victory
	cout << "🏆 Congratulations, you've completed this Sudoku puzzle!\n\n";
	cout << "Move History:\n" + sudoku.formatHistory() + "\n\n";
	cout << "Completed level " + to_string(sudoku.getLevel()) + " (" + sudoku.getDifficulty() + ")\n\n";
	exit(0);
}

int prompt(string prompt, string error, int min, int max) {
	while (true) {
		cout << prompt;
		string input;
		cin >> input;

		// prevent terminal from crashing when input is a string
		if (isNumber(input)) {
			// check if in range
			if (stoi(input) >= min && stoi(input) <= max) {
				return stoi(input);
			} else {
				cout << error;
			}
		} else {
			cout << error;
		}
	}
}

bool isNumber(string n) {
	for (int i = 0; i < n.length(); i++) {
		if (isdigit(n[i]) == false) return false;
	}
	return true;
}