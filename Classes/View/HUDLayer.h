#pragma once

#include "cocos2d.h"
#include "CookieLayer.h"
#include "ui/UIButton.h"

USING_NS_CC;

class HUDLayer : public Layer, public HUDLayerDelegate
{
private:
	Label* _target;
	Label* _movesLeft;
	Label* _score;
	Sprite* _gameOver;
	Sprite* _levelComplete;
	ui::Button* _btnTryAgain;
public:
	CREATE_FUNC(HUDLayer);
	bool init() override;
	void loadLayer();

	void updateLabels(int target, int movesLeft, int score) override ;
	void updateTarget(int target) override;
	void updateMovesLeft(int movesLeft) override;
	void updateScore(int score) override;

	void showGameOver() override;
	void hideGameOver() override;
	void showLevelComplete() override;
	void hideLevelComplete() override;

	void onEnter() override;
	void onExit() override;
};