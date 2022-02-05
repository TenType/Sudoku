#ifndef SUDOKU
#define SUDOKU

#include <vector>

class Sudoku {
	public:
		int board[9][9];
		int original[9][9];
		bool victory;
		bool showErrors;
		void load(const int difficulty, const int level);
		int getLevel();
		bool fill(const int row, const int col, const int n);
		string format();
		string getDifficulty();
		string formatHistory();
	private:
		int difficulty;
		int level;
		bool errors;
		bool complete;
		void validate();
		void checkRows(const int grid[9][9], const int row);
		void checkColumns(const int grid[9][9], const int col);
		void checkBoxes(const int grid[9][9], const int startRow, const int startCol);
		string createLog(const int row, const int col, const int n);
		vector<string> history;
};

#endif