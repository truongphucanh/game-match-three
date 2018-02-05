#include "Level.h"

Level::Level()
{
	_hasCookie = vector<vector<bool>>(NUM_ROWS, vector<bool>(NUM_COLS, false));
	_num = 0;
	_targetScore = 0;
	_maxMove = 0;
}

Level::Level(const int hasCookie[][NUM_COLS], int targetScore, int maxMove)
{
	_hasCookie = vector<vector<bool>>(NUM_ROWS, vector<bool>(NUM_COLS, false));
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (hasCookie[row][col] == 1)
			{
				_hasCookie[row][col] = true;
			}
		}
	}
	_targetScore = targetScore;
	_maxMove = maxMove;
}

bool Level::hasCookieAt(int row, int col) const
{
	if (row < 0 || row >= NUM_ROWS) return false;
	if (col < 0 || col >= NUM_COLS) return false;
	return _hasCookie[row][col];
}
