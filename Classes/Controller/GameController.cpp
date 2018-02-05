#include "GameController.h"
#include <new>
#include "Model/GameModel.h"
#include "SimpleAudioEngine.h"

static GameController* sharedGameController = nullptr;

GameController* GameController::getInstance()
{
	if (!sharedGameController)
		sharedGameController = new(std::nothrow) GameController();
	return sharedGameController;
}

void GameController::startGame()
{
	srand(time(nullptr));
	GameModel::getInstance()->init();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
}

void GameController::handleSwap(int rowA, int colA, int rowB, int colB)
{
	CCLOG("***Handling swap: (%d %d) (%d %d)", rowA, colA, rowB, colB);
	GameModel::getInstance()->trySwap(rowA, colA, rowB, colB);
}

void GameController::onSwapDone(int rowA, int colA, int rowB, int colB)
{
	CCLOG("***Removing swap matches (%d %d) (%d %d)", rowA, colA, rowB, colB);
	GameModel::getInstance()->removeSwapMatches(rowA, colA, rowB, colB);
}

void GameController::onRemoveDone(const set<RowCol>& matches)
{
	CCLOG("Remove done.");
	CCLOG("***Falling");
	GameModel::getInstance()->fall(matches);
}

void GameController::onFallDone(const set<Faller>& fallers)
{
	CCLOG("Falling done.");
	CCLOG("Filling empty cookies...");
	GameModel::getInstance()->fill(fallers);
}

void GameController::onFillDone()
{
	CCLOG("Fill done.");
	CCLOG("Remove all matches if have some");
	GameModel::getInstance()->removeAllMatches();
}

void GameController::onGameOver()
{
	CCLOG("Game over");
	GameModel::getInstance()->gameOver();
}

void GameController::onLevelComplete()
{
	CCLOG("Level complete, move to next level");
	GameModel::getInstance()->loadNextLevel();
}

void GameController::onbtnTryAgainClick()
{
	GameModel::getInstance()->reloadLevel();
}
