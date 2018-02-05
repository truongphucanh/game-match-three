#pragma once
#include "Global/Marcos.h"
#include <vector>
#include <CCPlatformMacros.h>

using namespace std;

class Level
{
private:
	vector<vector<bool>> _hasCookie;
	CC_SYNTHESIZE(int, _num, Num);
	CC_SYNTHESIZE(int, _targetScore, TargetScore)
	CC_SYNTHESIZE(int, _maxMove, MaxMove)
public:
	Level();
	explicit Level(const int hasCookie[][NUM_COLS], int targetScore, int maxMove);
	bool hasCookieAt(int row, int col) const;
};
