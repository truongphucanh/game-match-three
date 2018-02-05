#pragma once

#include "Model/GameModel.h"
#include "cocos2d.h"
#include "RowCol.h"

USING_NS_CC;

class Converter
{
public:
	static Vec2 toLocation(int row, int col);
	static RowCol toRowCol(const Vec2& location);
};