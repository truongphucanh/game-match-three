#include "BackgroundLayer.h"
#include "Global/VisibleRect.h"

bool BackgroundLayer::init()
{
    if (!Layer::init())
        return false;
	this->loadLayer();
    return true;
}

void BackgroundLayer::loadLayer()
{
	auto background = Sprite::create("Background.png");
	Size win_size = Director::getInstance()->getWinSize();
	float scaleX = win_size.width / background->getContentSize().width;
	float scaleY = win_size.height / background->getContentSize().height;
	background->setScale(scaleX >= scaleY ? scaleX : scaleY);
	background->setPosition(VisibleRect::center());
	this->addChild(background);
}

