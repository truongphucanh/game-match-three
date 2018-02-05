#include "GameScene.h"
#include "HUDLayer.h"
#include "Global/VisibleRect.h"
#include "CookieLayer.h"
#include "Global/Marcos.h"
#include "Controller/GameController.h"

bool GameScene::init()
{
	if (!Scene::init())
		return false;
	this->loadScene();
	return true;
}

void GameScene::loadScene()
{
	auto background = Sprite::create("Background.png");
	const auto winSize = Director::getInstance()->getWinSize();
	const auto scaleX = winSize.width / background->getContentSize().width;
	const auto scaleY = winSize.height / background->getContentSize().height;
	background->setScale(scaleX >= scaleY ? scaleX : scaleY);
	background->setPosition(VisibleRect::center());
	this->addChild(background);

	auto cookieLayer = CookieLayer::create();
	cookieLayer->ignoreAnchorPointForPosition(false);
	cookieLayer->setContentSize(Size(COOKIELAYER_WIDTH, COOKIELAYER_HEIGHT));
	cookieLayer->setPosition(VisibleRect::center());
	this->addChild(cookieLayer);

	auto hudLayer = HUDLayer::create();
	cookieLayer->ignoreAnchorPointForPosition(false);
	background->setPosition(VisibleRect::center());
	this->addChild(hudLayer);
}

void GameScene::onEnterTransitionDidFinish()
{
	GameController::getInstance()->startGame();
}

