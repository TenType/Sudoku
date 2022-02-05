#include <iostream>
#include <vector>
using namespace std;

#include "boards.hpp"
#include "sudoku.hpp"

void Sudoku::load(const int difficultyArg, const int levelArg) {
	difficulty = difficultyArg;
	level = levelArg;

	// copy board from boards.cpp to class instance
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			board[i][j] = boards[difficulty][level][i][j];
			original[i][j] = boards[difficulty][level][i][j];
		}
	}
	cout << "✅ Successfully loaded level " + to_string((level + 1) + difficulty * 10) + "\n\n";
}

bool Sudoku::fill(const int row, const int col, const int n) {
	// fill cell with value
	board[row][col] = n;

	// check sudoku
	validate();

	// return true if there are errors, false if none
	if (errors) {
		history.push_back(createLog(row, col, n) + "?");
		return true;
	} else if (complete) {
		history.push_back(createLog(row, col, n) + "#");
		victory = true;
		return false;
	}
	history.push_back(createLog(row, col, n));
	return false;
}

string Sudoku::format() {
	string output = "🎯 Level " + to_string(getLevel()) + "\n\n";

	// 9 x 9 sudoku board
	for (int i = 0; i < 9; i++) {
		// add horizontal lines to middle of board
		if (i == 3 || i == 6) {
			for (int j = 0; j < 47; j++) output += "-";
			output += "\n";
		}

		for (int j = 0; j < 9; j++) {
			// add vertical lines to middle of board
			if (j == 3 || j == 6) {
				output += "|";
			}
			if (board[i][j] == 0) {
				// cell is blank
				output += "  .  ";
			} else if (board[i][j] == original[i][j]) {
				// cell can't be changed
				output += " [" + to_string(board[i][j]) + "] ";
			} else {
				// cell was added by player
				output += "  " + to_string(board[i][j]) + "  ";
			}
		}
		output += "\n";
	}
	return output;
}

void Sudoku::validate() {
	errors = false;
	complete = true;

	// check rows
	for (int row = 0; row < 9; row++) {
		checkRows(board, row);
		if (errors) return;
	}

	// check columns
	for (int col = 0; col < 9; col++) {
		checkColumns(board, col);
		if (errors) return;
	}

	// check boxes
	for (int row = 0; row < 7; row += 3) {
		for (int col = 0; col < 7; col += 3) {
			checkBoxes(board, row, col);
			if (errors) return;
		}
	}
}

void Sudoku::checkRows(const int grid[9][9], const int row) {
	int seen[9] = {};

	// iterate through all 9 columns (checking one row)
	for (int col = 0; col < 9; col++) {
		int cell = grid[row][col];

		// check if sudoku is incomplete
		if (cell == 0) {
			complete = false;
			continue;
		}

		// check for duplicates
		for (int i = 0; i < 9; i++) {
			if (seen[i] == cell) {
				errors = true;
				return;
			}
		}

		seen[col] = cell;
	}
}

void Sudoku::checkColumns(const int grid[9][9], const int col) {
	int seen[9] = {};

	// iterate through all 9 rows (checking one column)
	for (int row = 0; row < 9; row++) {
		int cell = grid[row][col];

		// check if sudoku is incomplete
		if (cell == 0) {
			complete = false;
			continue;
		}

		// check for duplicates
		for (int i = 0; i < 9; i++) {
			if (seen[i] == cell) {
				errors = true;
				return;
			}
		}

		seen[row] = cell;
	}
}

void Sudoku::checkBoxes(const int grid[9][9], const int startRow, const int startCol) {
	int seen[9] = {};
	int count = -1;

	// iterate through 3x3 boxes
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			int cell = grid[row + startRow][col + startCol];
			count++;

			// check if sudoku is incomplete
			if (cell == 0) {
				complete = false;
				continue;
			}
		
			// check for duplicates
			for (int i = 0; i < 9; i++) {
				if (seen[i] == cell) {
					errors = true;
					return;
				}
			}

			seen[col] = cell;
		}
	}
}

string Sudoku::createLog(const int row, const int col, const int n) {
	// log entry for move history
	return to_string(row) + "," + to_string(col) + ":" + to_string(n);
}

string Sudoku::formatHistory() {
	// convert history into formatted string
	string output = "";
	for (int i = 0; i < history.size(); i++) {
		output += history[i] + ", ";
	}

	// remove comma and space at end
	output.pop_back();
	output.pop_back();

	return output;
}

int Sudoku::getLevel() {
	return (level + 1) + difficulty * 10;
}

string Sudoku::getDifficulty() {
	if (difficulty == 0) {
		return "⛳ Easy";
	}
	if (difficulty == 1) {
		return "🏔 Medium";
	}
	if (difficulty == 2) {
		return "🏔 Hard";
	}
	return "? Unknown";
}