#pragma once

#include "cocos2d.h"

USING_NS_CC;

class RowCol
{
private:
	CC_SYNTHESIZE(int, _row, Row);
	CC_SYNTHESIZE(int, _col, Col);
public:
	RowCol();
	RowCol(int row, int col);
	bool isValid() const;
	friend bool operator==(const RowCol& lhs, const RowCol& rhs)
	{
		return lhs.getRow() == rhs.getRow() && lhs.getCol() == rhs.getCol();
	}
	friend bool operator<(const RowCol& lhs, const RowCol& rhs)
	{
		return (lhs.getRow() < rhs.getRow()) || (lhs.getRow() == rhs.getRow() && lhs.getCol() < rhs.getCol());
	}
	friend bool operator>(const RowCol& lhs, const RowCol& rhs)
	{
		return !(lhs < rhs) && !(lhs == rhs);
	}
};