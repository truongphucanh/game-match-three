#include "HUDLayer.h"
#include "Global/VisibleRect.h"
#include "Global/Marcos.h"
#include "Model/GameModel.h"
#include "Controller/GameController.h"

bool HUDLayer::init()
{
	if (!Layer::init())
		return false;
	this->loadLayer();
	return true;
}

void HUDLayer::loadLayer()
{
	auto targetText = Label::createWithTTF("Target:", FONT_FILE, TEXT_SIZE);
	targetText->setPosition(VisibleRect::topleft() + Vec2(FROM_LEFT, -FROM_TOP));
	targetText->ignoreAnchorPointForPosition(false);
	this->addChild(targetText);

	_target = Label::createWithTTF("", FONT_FILE, NUM_SIZE);
	_target->setPosition(targetText->getPosition() + Vec2(0, - 0.75 * FROM_TOP));
	_target->ignoreAnchorPointForPosition(false);
	this->addChild(_target);

	auto movesLeftText = Label::createWithTTF("Moves:", FONT_FILE, TEXT_SIZE);
	movesLeftText->setPosition(VisibleRect::top() + Vec2(0, -FROM_TOP));
	movesLeftText->ignoreAnchorPointForPosition(false);
	this->addChild(movesLeftText);

	_movesLeft = Label::createWithTTF("", FONT_FILE, NUM_SIZE);
	_movesLeft->setPosition(movesLeftText->getPosition() + Vec2(0, -0.75 * FROM_TOP));
	_movesLeft->ignoreAnchorPointForPosition(false);
	this->addChild(_movesLeft);

	auto scoreText = Label::createWithTTF("Score:", FONT_FILE, TEXT_SIZE);
	scoreText->setPosition(VisibleRect::topright() + Vec2(-FROM_RIGHT, -FROM_TOP));
	scoreText->ignoreAnchorPointForPosition(false);
	this->addChild(scoreText);

	_score = Label::createWithTTF("", FONT_FILE, NUM_SIZE);
	_score->setPosition(scoreText->getPosition() + Vec2(0, -0.75 * FROM_TOP));
	_score->ignoreAnchorPointForPosition(false);
	this->addChild(_score);

	_gameOver = Sprite::create("GameOver.png");
	_gameOver->setPosition(VisibleRect::center());
	_gameOver->setContentSize(Size(PANEL_WIDTH, PANEL_HEIGHT));
	_gameOver->setOpacity(0);
	this->addChild(_gameOver);

	_levelComplete = Sprite::create("LevelComplete.png");
	_levelComplete->setPosition(VisibleRect::center());
	_levelComplete->setContentSize(Size(PANEL_WIDTH, PANEL_HEIGHT));
	_levelComplete->setOpacity(0);
	this->addChild(_levelComplete);

	_btnTryAgain = ui::Button::create("Button.png");
	_btnTryAgain->setTitleText("Try Again");
	_btnTryAgain->setTitleFontSize(TEXT_SIZE);
	_btnTryAgain->setTitleFontName(FONT_FILE);
	_btnTryAgain->setPosition(VisibleRect::center() - Vec2(0, _gameOver->getContentSize().height / 2 + FROM_TOP));
	_btnTryAgain->setVisible(false);
	_btnTryAgain->ignoreAnchorPointForPosition(false);
	_btnTryAgain->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			CCLOG("Clicked btn try again");
			GameController::onbtnTryAgainClick();
			_btnTryAgain->setVisible(false);
			break;
		default:
			break;
		}
	});
	this->addChild(_btnTryAgain);
}

void HUDLayer::updateLabels(int target, int movesLeft, int score)
{
	_target->setString(to_string(target));
	_movesLeft->setString(to_string(movesLeft));
	_score->setString(to_string(score));
}

void HUDLayer::updateTarget(int target)
{
	_target->setString(to_string(target));
}

void HUDLayer::updateMovesLeft(int movesLeft)
{
	_movesLeft->setString(to_string(movesLeft));
}

void HUDLayer::updateScore(int score)
{
	_score->setString(to_string(score));
}

void HUDLayer::showGameOver()
{
	Director::getInstance()->getEventDispatcher()->setEnabled(false);
	auto fadein = FadeIn::create(DURATION_FADE_IN_GAMEOVER);
	_gameOver->runAction(fadein);
	_btnTryAgain->setVisible(true);
	auto callback = Sequence::create(DelayTime::create(2.0f * DURATION_FADE_IN_GAMEOVER), CallFunc::create([&]()
	{
		GameController::onGameOver();
	}), nullptr);
	this->runAction(callback);
}

void HUDLayer::hideGameOver()
{
	Director::getInstance()->getEventDispatcher()->setEnabled(false);
	_gameOver->runAction(FadeOut::create(DURATION_FADE_IN_LEVEL_COMPLETE));
}

void HUDLayer::showLevelComplete()
{
	Director::getInstance()->getEventDispatcher()->setEnabled(false);
	_levelComplete->runAction(FadeIn::create(DURATION_FADE_IN_LEVEL_COMPLETE));
	auto callback = Sequence::create(DelayTime::create(2 * DURATION_FADE_IN_LEVEL_COMPLETE),CallFunc::create([&]()
	{
		GameController::onLevelComplete();
	}), nullptr);
	this->runAction(callback);
}

void HUDLayer::hideLevelComplete()
{
	Director::getInstance()->getEventDispatcher()->setEnabled(false);
	_levelComplete->runAction(FadeOut::create(DURATION_FADE_IN_LEVEL_COMPLETE));
}

void HUDLayer::onEnter()
{
	Layer::onEnter();
	GameModel::getInstance()->setHUDLayerDelegate(this);
}

void HUDLayer::onExit()
{
	Layer::onExit();
	GameModel::getInstance()->setHUDLayerDelegate(nullptr);
}
