#pragma once

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public Scene
{
public:
	CREATE_FUNC(GameScene);
	bool init() override;
	void loadScene();
	void onEnterTransitionDidFinish() override;
};
