#pragma once

#include "cocos2d.h"

USING_NS_CC;

class BackgroundLayer : public Layer
{
public:
	CREATE_FUNC(BackgroundLayer);
	bool init() override;
	void loadLayer();
};
