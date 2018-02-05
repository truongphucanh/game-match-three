#pragma once
#include "cocos2d.h"
#include "View/CookieLayer.h"

USING_NS_CC;

class GameController
{
public:
	static GameController* getInstance();
	static void startGame();
	static void handleSwap(int rowA, int colA, int rowB, int colB);
	static void onSwapDone(int rowA, int colA, int rowB, int colB);
	static void onRemoveDone(const set<RowCol>& matches);
	static void onFallDone(const set<Faller>& fallers);
	static void onFillDone();
	static void onGameOver();
	static void onLevelComplete();
	static void onbtnTryAgainClick();
};
