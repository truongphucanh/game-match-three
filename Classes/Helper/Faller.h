#pragma once
#include <CCPlatformMacros.h>

/**
 * \brief Store infomations a about cookies need to fall down
 */
class Faller
{
private:
	CC_SYNTHESIZE(int, _row, Row); // row index of cookie need to fall
	CC_SYNTHESIZE(int, _col, Col); // col index of cookie need to fall
	CC_SYNTHESIZE(int, _step, Step); // fall down step, cookies need to fall down "step" tile from row, col
public:
	Faller(int row, int col, int step);
	friend bool operator==(const Faller& lhs, const Faller& rhs)
	{
		return lhs.getRow() == rhs.getRow() && lhs.getCol() == rhs.getCol();
	}
	friend bool operator<(const Faller& lhs, const Faller& rhs)
	{
		return (lhs.getRow() < rhs.getRow()) || (lhs.getRow() == rhs.getRow() && lhs.getCol() < rhs.getCol());
	}
	friend bool operator>(const Faller& lhs, const Faller& rhs)
	{
		return !(lhs < rhs) && !(lhs == rhs);
	}
};
