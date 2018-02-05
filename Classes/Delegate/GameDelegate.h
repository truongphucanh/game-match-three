#pragma once

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "Model/Cookie.h"
#include "Model/Level.h"
#include "Global/RowCol.h"
#include "Helper/Faller.h"
#include <functional>
#include <set>

USING_NS_CC;

class CookieLayerDelegate
{
public:
	virtual ~CookieLayerDelegate() = default;
	virtual void addCookies(const vector<vector<Cookie>>& cookies) = 0;
	virtual void animateSwapSucess(int rowA, int colA, int rowB, int colB) = 0;
	virtual void animateSwapFail(int rowA, int colA, int rowB, int colB) = 0;
	virtual void animateRemoveMatches(const set<RowCol>& matches) = 0;
	virtual void animateFall(const set<Faller>& fallers) = 0;
	virtual void animateFill(const vector<vector<Cookie>>& filledCookies) = 0;
	virtual void animateGameOver() = 0;
	virtual void animateBeginGame() = 0;
	virtual void animateShuffle(const vector<vector<Cookie>>& vector) = 0;
};

class HUDLayerDelegate
{
public:
	virtual ~HUDLayerDelegate() = default;
	virtual void updateLabels(int target, int movesLeft, int score) = 0;
	virtual void updateTarget(int target) = 0;
	virtual void updateMovesLeft(int movesLeft) = 0;
	virtual void updateScore(int score) = 0;
	virtual void showGameOver() = 0;
	virtual void hideGameOver() = 0;
	virtual void showLevelComplete() = 0;
	virtual void hideLevelComplete() = 0;
};