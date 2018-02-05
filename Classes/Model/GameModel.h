#pragma once
#include <vector>
#include "Cookie.h"
#include "Level.h"
#include "Delegate/GameDelegate.h"
#include "Global/RowCol.h"
#include "cocos2d.h"

USING_NS_CC;

class GameModel
{
private:
	static GameModel* _sharedGameModel;
	static vector<Level> _levels;
	vector<vector<Cookie>> _cookies;
	CC_SYNTHESIZE(int, _currLevel, CurrLevel)
	CC_SYNTHESIZE(int, _movesLeft, MovesLeft)
	CC_SYNTHESIZE(int, _score, Score)
	CC_SYNTHESIZE(CookieLayerDelegate*, _cookieLayerDelegate, CookieLayerDelegate)
	CC_SYNTHESIZE(HUDLayerDelegate*, _hudLayerDelegate, HUDLayerDelegate)

public:
	static GameModel* getInstance();
	GameModel();
	void initCookies();
	void init();
	void shuffle();
	void print() const;

	Cookie cookieAt(int row, int col);
	Level currentLevel() const;

	int countSameCookies(int row, int col, int verDelta, int horDelta) const;
	bool hasChainAt(int row, int col) const;
	void decreaseMove();
	void trySwap(int rowA, int colA, int rowB, int colB);

	set<RowCol> findMatchesByDirection(int row, int col, int verDelta, int horDelta) const;
	set<RowCol> findHorizontalMatches(int row, int col);
	set<RowCol> findVerticalMatches(int row, int col);
	set<RowCol> findMatchesAt(int row, int col);
	void removeSwapMatches(int rowA, int colA, int rowB, int colB);
	void removeAllMatches();
	void fall(const set<RowCol>& matches);
	void fill(const set<Faller>& fallers);

	void loadNextLevel();
	void loadLevel(int level);
	void gameOver() const;
	void reloadLevel();

	bool hasPossibleSwap();

	static bool isValidIndex(int row, int col);
	static void loadLevelsConstant();
};
