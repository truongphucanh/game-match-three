#include "CookieLayer.h"
#include "Global/Converter.h"
#include "Controller/GameController.h"
#include "SimpleAudioEngine.h"
#include "Global/VisibleRect.h"

CookieLayer::CookieLayer()
{
	this->reset();
}

/**
 * \brief success or not
 * \return called automatically
 */
bool CookieLayer::init()
{
	if (!Layer::init())
		return false;
	_cookies = vector<vector<Sprite*>>(NUM_ROWS, vector<Sprite*>(NUM_COLS, nullptr));
	this->reset();
	this->addEvents();
	return true;
}

/**
 * \brief Reset private fields
 */
void CookieLayer::reset()
{
	this->removeAllChildren();
	for (auto row : _cookies)
	{
		std::fill(row.begin(), row.end(), nullptr);
	}
	_highlight = nullptr;
	_swipeFromRow = -1;
	_swipeFromCol = -1;
	this->ignoreAnchorPointForPosition(false);
	this->setContentSize(Size(COOKIELAYER_WIDTH, COOKIELAYER_HEIGHT));
	this->setPosition(VisibleRect::center());
}

/**
 * \brief Add events listener
 */
void CookieLayer::addEvents()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		auto location = this->convertToNodeSpace(touch->getLocation());
		auto rowcol = Converter::toRowCol(location);
		int row = rowcol.getRow();
		int col = rowcol.getCol();
		CCLOG("touch on: %d, %d", rowcol.getRow(), rowcol.getCol());
		if (rowcol.isValid())
		{
			// if rowcol is not empty in this level
			if (_cookies[row][col])
			{
				_swipeFromRow = row;
				_swipeFromCol = col;
				this->highlight(row, col);
				CCLOG("touch began return true");
				return true;
			}
		}
		CCLOG("touch began return fasle");
		return false;
	};
	listener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		if (_swipeFromRow < 0 || _swipeFromRow >= NUM_ROWS) return;
		if (_swipeFromCol < 0 || _swipeFromCol >= NUM_COLS) return;
		auto location = this->convertToNodeSpace(touch->getLocation());
		auto rowcol = Converter::toRowCol(location);
		int row = rowcol.getRow();
		int col = rowcol.getCol();
		if (rowcol.isValid())
		{
			int horDelta = 0;
			int verDelta = 0;

			if (row < _swipeFromRow)
				verDelta = -1;
			else if (row > _swipeFromRow)
				verDelta = 1;
			else if (col < _swipeFromCol)
				horDelta = -1;
			else if (col > _swipeFromCol)
				horDelta = 1;

			if (horDelta != 0 || verDelta != 0)
			{
				GameController::handleSwap(_swipeFromRow, _swipeFromCol, _swipeFromRow + verDelta, _swipeFromCol + horDelta);
				_swipeFromRow = _swipeFromCol = -1; // reset it
			}
		}
	};
	listener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		CCLOG("touch end");
		_swipeFromRow = _swipeFromCol = -1;
		this->unhighlight();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

/**
 * \brief add Tiles (behide cookies)
 * \param cookies 
 */
void CookieLayer::addTiles(const vector<vector<Cookie>>& cookies)
{
	for (int row = 0; row <= NUM_ROWS; row++)
	{
		for (int col = 0; col <= NUM_COLS; col++)
		{
			auto topLeft = (col > 0) && (row < NUM_ROWS)
				&& !cookies[row][col - 1].is(Nothing);
			auto bottomLeft = (col > 0) && (row > 0)
				&& !cookies[row - 1][col - 1].is(Nothing);
			auto topRight = (col < NUM_COLS) && (row < NUM_ROWS)
				&& !cookies[row][col].is(Nothing);
			auto bottomRight = (col < NUM_COLS) && (row > 0)
				&& !cookies[row - 1][col].is(Nothing);

			auto value =
				static_cast<int>(topLeft) |
				static_cast<int>(topRight) << 1 |
				static_cast<int>(bottomLeft) << 2 |
				static_cast<int>(bottomRight) << 3;

			if (value != 0 && value != 6 && value != 9) {
				auto name = string("Tile_").append(std::to_string(value)).append("@2x.png");
				auto tileSprite = Sprite::create(name);
				tileSprite->setScale(TILE_WIDTH / tileSprite->getContentSize().width);
				auto pos = Converter::toLocation(row, col) - Vec2(TILE_WIDTH / 2, TILE_HEIGHT / 2);
				tileSprite->setPosition(pos);
				this->addChild(tileSprite);
			}

		}
	}

	//auto tileSprite = Sprite::create("tile.png");
	//tileSprite->setPosition(Converter::toLocation(row, col));
	//tileSprite->setScale(TILE_WIDTH / tileSprite->getContentSize().width);
	//this->addChild(tileSprite, -1);
}

/**
 * \brief Add cookie sprites 
 * \param cookies Cookies need to add
 */
void CookieLayer::addCookies(const vector<vector<Cookie>>& cookies)
{
	float maxDuration = 0; 
	this->reset();
	_eventDispatcher->setEnabled(false);
	this->addTiles(cookies);
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (!cookies[row][col].is(Nothing))
			{
				auto cookieSprite = Sprite::create(cookies[row][col].getSpriteName().append("@2x.png"));
				cookieSprite->setPosition(Converter::toLocation(row, col));
				auto scale = (TILE_WIDTH / cookieSprite->getContentSize().width);
				cookieSprite->setScale(0.1f);
				cookieSprite->setOpacity(0);

				// just animate (in random time) to scale up cookie
				auto LO = MIN_DELAY_TIME_IN_BEGIN;
				auto HI = MAX_DELAY_TIME_IN_BEGIN;
				auto delay = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));;
				auto wait = DelayTime::create(delay);
				auto scaleto = ScaleTo::create(DURATION_FADE_IN_BEGIN, scale);
				auto fadein = FadeIn::create(DURATION_FADE_IN_BEGIN);
				auto actions = Sequence::create(wait, Spawn::create(scaleto, fadein, nullptr), nullptr);
				cookieSprite->runAction(actions);
				if (delay + DURATION_FADE_IN_BEGIN > maxDuration)
					maxDuration = delay + DURATION_FADE_IN_BEGIN;

				// add this cookie
				this->addChild(cookieSprite);
				_cookies[row][col] = cookieSprite;
			}
		}
	}

	auto callback = Sequence::create(DelayTime::create(maxDuration + 2*MAKE_SURE_ANIMATION_FINISH), CallFunc::create([&]()
	{
		_eventDispatcher->setEnabled(true);
	}), nullptr);
	this->runAction(callback);
}

/**
 * \brief Highights _cookies[row][col]
 * \param row 
 * \param col 
 */
void CookieLayer::highlight(int row, int col)
{
	auto textureName = GameModel::getInstance()->cookieAt(row, col).getHighlightedSpriteName().append("@2x.png");
	_highlight = Sprite::create(textureName);
	auto cookie = _cookies[row][col];
	auto center = Vec2(cookie->getContentSize().width / 2, cookie->getContentSize().height / 2);
	_highlight->setPosition(center);
	cookie->addChild(_highlight);
}

/**
 * \brief unhightligh the higlighted cookie
 */
void CookieLayer::unhighlight()
{
	if (!_highlight || !_highlight->getParent()) return;
	_highlight->removeFromParent();
	_highlight = nullptr;
}

/**
 * \brief Animates swap 2 cookies success (remove matches after that)
 * \param rowA 
 * \param colA 
 * \param rowB 
 * \param colB 
 */
void CookieLayer::animateSwapSucess(int rowA, int colA, int rowB, int colB)
{
	_eventDispatcher->setEnabled(false);
	this->unhighlight();

	// swap sprites
	auto tmp = _cookies[rowA][colA];
	_cookies[rowA][colA] = _cookies[rowB][colB];
	_cookies[rowB][colB] = tmp;

	auto cookieA = _cookies[rowA][colA];
	auto cookieB = _cookies[rowB][colB];

	// run action
	cookieA->runAction(MoveTo::create(DURATION_SWAP, cookieB->getPosition()));
	cookieB->runAction(MoveTo::create(DURATION_SWAP, cookieA->getPosition()));

	// play sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Swap.wav");

	// call back when action was done.
	auto callback = Sequence::create(DelayTime::create(DURATION_SWAP + MAKE_SURE_ANIMATION_FINISH), CallFunc::create([&, rowA, colA, rowB, colB](){
		GameController::onSwapDone(rowA, colA, rowB, colB);
	}), nullptr);
	this->runAction(callback);
}

/**
 * \brief Animate swap 2 cookies fail
 * \param rowA 
 * \param colA 
 * \param rowB 
 * \param colB 
 */
void CookieLayer::animateSwapFail(int rowA, int colA, int rowB, int colB)
{
	_eventDispatcher->setEnabled(false);
	this->unhighlight();

	auto cookieA = _cookies[rowA][colA];
	auto cookieB = _cookies[rowB][colB];
	auto MoveToA = MoveTo::create(DURATION_SWAP, cookieA->getPosition());
	auto MoveToB = MoveTo::create(DURATION_SWAP, cookieB->getPosition());
	auto delay = DelayTime::create(DELAY_TIME);
	cookieA->runAction(Sequence::create(MoveToB, delay, MoveToA, nullptr));
	cookieB->runAction(Sequence::create(MoveToA, delay, MoveToB, nullptr));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("InvalidSwap.wav");
	auto callback = Sequence::create(DelayTime::create(DURATION_SWAP * 2 + DELAY_TIME + MAKE_SURE_ANIMATION_FINISH), CallFunc::create([&](){
		_eventDispatcher->setEnabled(true);
	}), nullptr);
	this->runAction(callback);
}

/**
 * \brief Remove cookies in matches
 * \param matches RowCol of cookies need to remove
 */
void CookieLayer::animateRemoveMatches(const set<RowCol>& matches)
{
	if (matches.empty())
		return;
	// action
	for_each(matches.begin(), matches.end(), [&](RowCol rowcol)
	{
		int row = rowcol.getRow();
		int col = rowcol.getCol();
		auto sprite = _cookies[row][col];

		// animations of score
		auto score = Label::createWithTTF(to_string(SCORE_PER_COOKIE), FONT_FILE, NUM_SIZE);
		score->setPosition(sprite->getPosition());
		this->addChild(score);
		auto scoreActions = Sequence::create(MoveBy::create(DURATION_MOVE_SCORE, Vec2(0, DELTA_MOVE_SCORE)), RemoveSelf::create(), nullptr);
		score->runAction(scoreActions);

		// animation of removing cookie
		auto actions = Sequence::create(ScaleTo::create(DURATION_REMOVE, 0.0f), RemoveSelf::create(), nullptr);
		sprite->runAction(actions);
	});

	// sound
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Match.wav");

	// call back when remove done.
	auto seq = Sequence::create(DelayTime::create(DURATION_REMOVE + MAKE_SURE_ANIMATION_FINISH), CallFunc::create([&, matches](){
		GameController::onRemoveDone(matches);
	}), nullptr);
	this->runAction(seq);
}

/**
 * \brief Animates fallings cookies
 * \param fallers Information about which cookie need to fall and how far the falling is
 */
void CookieLayer::animateFall(const set<Faller>& fallers)
{
	float maxDuration = 0;
	for_each(fallers.begin(), fallers.end(), [&](Faller faller)
	{
		int row = faller.getRow();
		int col = faller.getCol();
		int step = faller.getStep();
		_cookies[row - step][col] = _cookies[row][col];
		_cookies[row][col] = nullptr;
		float duration = step * DURATION_FALLING;
		if (duration > maxDuration)
			maxDuration = duration;
		Vec2 position = Converter::toLocation(row - step, col);
		auto moveto = MoveTo::create(duration, position);
		_cookies[row - step][col]->runAction(moveto);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Falling.wav");
	});
	auto callback = Sequence::create(DelayTime::create(maxDuration + MAKE_SURE_ANIMATION_FINISH), CallFunc::create([&, fallers]()
	{
		GameController::onFallDone(fallers);
	}), nullptr);
	this->runAction(callback);
}

/**
 * \brief animate fill cookies in filledCookies (falling down from the top)
 * \param filledCookies 
 */
void CookieLayer::animateFill(const vector<vector<Cookie>>& filledCookies)
{
	float maxDuration = 0;
	for_each(filledCookies.begin(), filledCookies.end(), [&](vector<Cookie> col)
	{
		// delay = wait for the last falling cookie.
		auto delay = DURATION_FALLING * (col.size() - 1);
		for (int i = 0; i < col.size(); i++)
		{
			auto cookie = col[i];

			auto sprite = Sprite::create(cookie.getSpriteName().append("@2x.png"));
			sprite->setScale(TILE_WIDTH / sprite->getContentSize().width);
			auto position = Converter::toLocation(NUM_ROWS, cookie.getCol());
			sprite->setPosition(position);
			sprite->setOpacity(0);
			_cookies[cookie.getRow()][cookie.getCol()] = sprite;
			this->addChild(sprite);

			auto newPostion = Converter::toLocation(cookie.getRow(), cookie.getCol());
			auto duration = (NUM_ROWS - cookie.getRow()) * DURATION_FALLING;
			auto currDelay = delay + DURATION_FALLING * (col.size() - i - 1); // delay for this cookie
			auto wait = DelayTime::create(currDelay);
			auto fadein = FadeIn::create(DURATION_FADE_IN);
			auto moveto = MoveTo::create(duration, newPostion);
			auto playsound = CallFunc::create([&]()
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("AddCookie.wav");
			});
			auto actions = Sequence::create(wait, Spawn::create(fadein, moveto, playsound, nullptr), nullptr);
			sprite->runAction(actions);
			auto totalDuration = currDelay + max(DURATION_FADE_IN, duration);
			if (totalDuration > maxDuration)
				maxDuration = totalDuration;
			delay -= DURATION_FALLING; // cookie at higher row need to wait more long than cookie below
		}
	});
	auto callback = Sequence::create(DelayTime::create(maxDuration + MAKE_SURE_ANIMATION_FINISH),CallFunc::create([&]()
	{
		GameController::onFillDone();
	}), nullptr);
	this->runAction(callback);
}

/**
 * \brief hide all cookie (scale it to 0 and remove it from this layer)
 * \return maximum time of animations
 */
float CookieLayer::animateHideCookies()
{
	auto maxDuration = 0.0f;
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (_cookies[row][col])
			{
				auto LO = MIN_DELAY_TIME_IN_BEGIN;
				auto HI = MAX_DELAY_TIME_IN_BEGIN;
				auto delay = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));;
				auto wait = DelayTime::create(delay);
				auto scaleto = ScaleTo::create(DURATION_FADE_IN_BEGIN, 0.0);
				auto fadeout = FadeOut::create(DURATION_FADE_IN_BEGIN);
				auto actions = Sequence::create(wait, Spawn::create(scaleto, fadeout, nullptr), RemoveSelf::create(), nullptr);
				_cookies[row][col]->runAction(actions);
				if (delay + DURATION_FADE_IN_BEGIN > maxDuration)
					maxDuration = delay + DURATION_FADE_IN_BEGIN;
			}
		}
	}
	auto callback = Sequence::create(DelayTime::create(maxDuration + MAKE_SURE_ANIMATION_FINISH), CallFunc::create([&]()
	{
	}), nullptr);
	this->runAction(callback);
	return maxDuration;
}

/**
 * \brief animate shuffle cookie matrix
 * \param cookies new cookies value
 */
void CookieLayer::animateShuffle(const vector<vector<Cookie>>& cookies)
{
	_eventDispatcher->setEnabled(false);
	auto wait = this->animateHideCookies();
	// wait all cookies to hide
	auto callback = Sequence::create(DelayTime::create(wait + MAKE_SURE_ANIMATION_FINISH), CallFunc::create([&, cookies]()
	{
		this->addCookies(cookies);
	}), nullptr);
	this->runAction(callback);
}

/**
 * \brief called when player loses the game
 */
void CookieLayer::animateGameOver()
{
	_eventDispatcher->setEnabled(false);
	auto move = MoveTo::create(DURATION_GAME_OVER, VisibleRect::bot() + Vec2(0, -COOKIELAYER_HEIGHT / 2));
	auto callback = CallFunc::create([&]()
	{
		_eventDispatcher->setEnabled(true);
	});
	this->runAction(Sequence::create(move, callback, nullptr));
}

void CookieLayer::animateBeginGame()
{
	// todo Add some animation when begin a game
	/*this->setPosition(VisibleRect::bot() + Vec2(0, -COOKIELAYER_HEIGHT / 2));
	auto move = MoveTo::create(DURATION_GAME_OVER, VisibleRect::center());
	this->runAction(move);*/
}


/**
 * \brief enter transistion
 */
void CookieLayer::onEnter()
{
	Layer::onEnter();
	GameModel::getInstance()->setCookieLayerDelegate(this);
}

/**
 * \brief exit 
 */
void CookieLayer::onExit()
{
	Layer::onExit();
	GameModel::getInstance()->setCookieLayerDelegate(nullptr);
}
