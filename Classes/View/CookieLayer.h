#pragma once
#include "cocos2d.h"
#include "Delegate/GameDelegate.h"

USING_NS_CC;

class CookieLayer : public Layer, CookieLayerDelegate
{
private:
	vector<vector<Sprite*>> _cookies;
	Sprite* _highlight;
	int _swipeFromRow;
	int _swipeFromCol;
public:
	CREATE_FUNC(CookieLayer);
	CookieLayer();
	bool init() override;
	void reset();
	void addEvents();
	void addTiles(const vector<vector<Cookie>>& cookies);
	void addCookies(const vector<vector<Cookie>>& cookies) override;
	
	void highlight(int row, int col);
	void unhighlight();

	void animateSwapSucess(int rowA, int colA, int rowB, int colB) override;
	void animateSwapFail(int rowA, int colA, int rowB, int colB) override;
	void animateRemoveMatches(const set<RowCol>& matches) override;
	void animateFall(const set<Faller>& fallers) override;
	void animateFill(const vector<vector<Cookie>>& filledCookies) override;
	float animateHideCookies();
	void animateShuffle(const vector<vector<Cookie>>& vector) override;
	void animateGameOver() override;
	void animateBeginGame() override;

	void onEnter() override;
	void onExit() override;
};