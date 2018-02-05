#include "RowCol.h"
#include "Marcos.h"

RowCol::RowCol()
{
	_row = _col = -1;
}

RowCol::RowCol(int row, int col)
{
	_row = row;
	_col = col;
}

bool RowCol::isValid() const
{
	return _row >= 0 && _row < NUM_ROWS
		&& _col >= 0 && _col < NUM_COLS;
}
