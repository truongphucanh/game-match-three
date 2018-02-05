#include "GameModel.h"
#include <new>
#include "Global/Marcos.h"
#include "Helper/Faller.h"
#include "Global/Parser.h"

GameModel* GameModel::_sharedGameModel = nullptr;
vector<Level> GameModel::_levels;

/**
 * \brief 
 * \return Sigleton instance
 */
GameModel* GameModel::getInstance()
{
	if (!_sharedGameModel)
		_sharedGameModel = new(std::nothrow)GameModel();
	return _sharedGameModel;
}

GameModel::GameModel()
{
	_cookies = std::vector<std::vector<Cookie>>(NUM_ROWS, std::vector<Cookie>(NUM_COLS, Cookie()));
	_currLevel = 0;
	_movesLeft = 0;
	_score = 0;
	_cookieLayerDelegate = nullptr;
	_hudLayerDelegate = nullptr;
}

/**
 * \brief create cookies with random type
 */
void GameModel::initCookies()
{
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			if (currentLevel().hasCookieAt(row, col))
			{
				auto type = Cookie::randomCookieType();
				while ((row >= 2 && _cookies[row - 1][col].is(type) && _cookies[row - 2][col].is(type))
					|| (col >= 2 && _cookies[row][col - 1].is(type) && _cookies[row][col - 2].is(type)))
				{
					type = Cookie::randomCookieType();
				}

				auto cookie = Cookie(row, col, type);
				_cookies[row][col] = cookie;
			}
			else
				_cookies[row][col] = Cookie(row, col, Nothing);
		}
	}
	if (!this->hasPossibleSwap())
		this->initCookies();
}

/**
 * \brief initialize in the begining of game
 */
void GameModel::init()
{
	this->loadLevel(START_LEVEL);
}

/**
 * \brief Shuffle the cookie matrix when no possible swap was found
 */
void GameModel::shuffle()
{
	CCLOG("No possible swap was found. need to shuffle");
	this->initCookies();
	_cookieLayerDelegate->animateShuffle(_cookies);
}

/**
 * \brief for debug only
 */
void GameModel::print() const
{
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			CCLOG("%d %d: %d", row, col, _cookies[row][col].getType());
		}
	}
}

/**
 * \brief get cookie at row, col
 * \param row 
 * \param col 
 * \return _cookies[row][col] if row,col is inside the cookie matrix
 */
Cookie GameModel::cookieAt(int row, int col)
{
	if (isValidIndex(row, col))
		return _cookies[row][col];
	return Cookie();
}

/**
 * \brief 
 * \return get current level info
 */
Level GameModel::currentLevel() const
{
	return _levels[_currLevel];
}


/**
* \brief count numbers of cookies have the same type with Cookies[row][col]
* \param row Row of cookie need to count matches
* \param col Col of cookie need to count matches
* \param verDelta Vertical delta (1: top, -1: bot)
* \param horDelta Horizontal delta (1: right, -1: left)
* \return Number of cookies have the same type with Cookies[row][col] in direction (ver_delta, hor_delta)
*/
int GameModel::countSameCookies(int row, int col, int verDelta, int horDelta) const
{
	int count = 0;
	auto typeNeedToFind = _cookies[row][col].getType();
	row += verDelta;
	col += horDelta;
	while (row >= 0 && row < NUM_ROWS && col >= 0 && col < NUM_COLS && this->_cookies[row][col].is(typeNeedToFind))
	{
		count++;
		row += verDelta;
		col += horDelta;
	}
	return count;
}

/**
* \brief Checks whether Cookies at (row, col) has any chain (in vertical or horizontal direction)
* \param row Row index of the cookie
* \param col Col index of the cookie
* \return true if has chain (3 or more cookies have the same type)
*/
bool GameModel::hasChainAt(int row, int col) const
{
	int numLeftMatches = countSameCookies(row, col, 0, -1);
	int numRightMatches = countSameCookies(row, col, 0, 1);
	int numUpMatches = countSameCookies(row, col, 1, 0);
	int numDownMatches = countSameCookies(row, col, -1, 0);

	// todo: un-comment code below for diagonal direction
	//int numLeftUpMatches = countSameCookies(row, col, 1, -1);
	//int numLeftDownMatches = countSameCookies(row, col, -1, -1);
	//int numRightUpMatches = countSameCookies(row, col, 1, 1);
	//int numRightDownMatches = countSameCookies(row, col, -1, 1);

	return numLeftMatches + numRightMatches + 1 >= 3
		|| numDownMatches + numUpMatches + 1 >= 3;
	//|| numLeftUpMatches + numRightDownMatches + 1 >= 3
	//|| numRightUpMatches + numLeftDownMatches + 1 >= 3;
}

/**
 * \brief decrease movesLeft
 */
void GameModel::decreaseMove()
{
	_movesLeft--;
	_hudLayerDelegate->updateMovesLeft(_movesLeft);
}

/**
 * \brief try swap cookieA and cookieB
 * \param rowA 
 * \param colA 
 * \param rowB 
 * \param colB 
 */
void GameModel::trySwap(int rowA, int colA, int rowB, int colB)
{
	if (rowA < 0 || rowA >= NUM_ROWS) return;
	if (colA < 0 || colA >= NUM_COLS) return;
	if (rowB < 0 || rowB >= NUM_ROWS) return;
	if (colB < 0 || colB >= NUM_COLS) return;
	if (!currentLevel().hasCookieAt(rowA, colA)) return;
	if (!currentLevel().hasCookieAt(rowB, colB)) return;

	// decreas move lelft
	this->decreaseMove();

	// swap 2 cookies to see if any chain is made.
	std::swap(_cookies[rowA][colA], _cookies[rowB][colB]);

	// if swaping makes chains
	if (this->hasChainAt(rowA, colA) || this->hasChainAt(rowB, colB))
	{
		// call view to animate swaping sucess action
		_cookieLayerDelegate->animateSwapSucess(rowA, colA, rowB, colB);
		return;
	}

	// if swaping doesn't make any chain, just re-swap cookies
	std::swap(_cookies[rowA][colA], _cookies[rowB][colB]);

	// swap fail and have no move left
	if (_movesLeft <= 0)
	{
		_hudLayerDelegate->showGameOver();
	}

	// call view to animate swaping fail action
	_cookieLayerDelegate->animateSwapFail(rowA, colA, rowB, colB);
}

/**
 * \brief Find matches by direction
 * \param row index
 * \param col index
 * \param ver_delta 1: find on top, -1: find in bottom
 * \param hor_delta 1: find in right, -1: find in left
 * \return all RowCol index of cookies in any chain
 */
set<RowCol> GameModel::findMatchesByDirection(int row, int col, int ver_delta, int hor_delta) const
{
	if (!isValidIndex(row, col) || _cookies[row][col].is(Nothing))
		return set<RowCol>();
	set<RowCol> matches;
	matches.insert(RowCol(row, col));
	auto typeNeedToFind = _cookies[row][col].getType();
	row += ver_delta;
	col += hor_delta;
	while (row >= 0 && row < NUM_ROWS && col >= 0 && col < NUM_COLS)
	{
		if (!_cookies[row][col].is(typeNeedToFind))
			break;
		matches.insert(RowCol(row, col));
		row += ver_delta;
		col += hor_delta;
	}
	if (matches.size() >= 3)
		return matches;
	return set<RowCol>();
}

/**
 * \brief find matches by horizontal (left and right) at cookie[row][col]
 * \param row 
 * \param col 
 * \return all RowCol index of cookies in horizontal chain
 */
set<RowCol> GameModel::findHorizontalMatches(int row, int col)
{
	if (!isValidIndex(row, col) || _cookies[row][col].is(Nothing))
		return set<RowCol>();
	set<RowCol> matches;
	matches.insert(RowCol(row, col));
	auto typeNeedToFind = _cookies[row][col].getType();
	int left = col;
	int right = col;
	// find same cookie type in left
	while (left >= 0)
	{
		if (!_cookies[row][left].is(typeNeedToFind))
			break;
		matches.insert(RowCol(row, left));
		left--;
	}
	// find same cookie type in right
	while (right < NUM_COLS)
	{
		if (!_cookies[row][right].is(typeNeedToFind))
			break;
		matches.insert(RowCol(row, right));
		right++;
	}
	if (matches.size() >= 3)
		return matches;
	return set<RowCol>();
}

/**
* \brief find matches by vertical (left and right) at cookie[row][col]
* \param row
* \param col
* \return all RowCol index of cookies in vertical chain
 */
set<RowCol> GameModel::findVerticalMatches(int row, int col)
{
	if (!isValidIndex(row, col) || _cookies[row][col].is(Nothing))
		return set<RowCol>();
	set<RowCol> matches;
	matches.insert(RowCol(row, col));
	auto typeNeedToFind = _cookies[row][col].getType();
	int top = row;
	int bot = row;
	// find same cookie type in bottom
	while (bot >= 0)
	{
		if (!_cookies[bot][col].is(typeNeedToFind))
			break;
		matches.insert(RowCol(bot, col));
		bot--;
	}
	// find same cookie type in top
	while (top < NUM_ROWS)
	{
		if (!_cookies[top][col].is(typeNeedToFind))
			break;
		matches.insert(RowCol(top, col));
		top++;
	}
	if (matches.size() >= 3)
		return matches;
	return set<RowCol>();
}

/**
 * \brief Find all cookies in chain from cookie[row][col]
 * \param row 
 * \param col 
 * \return all RowCol index of cookies in chain
 */
set<RowCol> GameModel::findMatchesAt(int row, int col)
{
	set<RowCol> matches;

	// find horizontal matches
	auto hor = this->findHorizontalMatches(row, col);
	matches.insert(hor.begin(), hor.end());

	// find vertical matches
	auto ver = this->findVerticalMatches(row, col);
	matches.insert(ver.begin(), ver.end());	

	return matches;
}

/**
 * \brief Remove cookies in chain (after swap them)
 * \param rowA 
 * \param colA 
 * \param rowB 
 * \param colB 
 */
void GameModel::removeSwapMatches(int rowA, int colA, int rowB, int colB)
{
	auto matches = this->findMatchesAt(rowA, colA);
	auto matchesB = this->findMatchesAt(rowB, colB);
	matches.insert(matchesB.begin(), matchesB.end());
	for_each(matches.begin(), matches.end(), [&](RowCol rowcol)
	{
		CCLOG("removing (%d %d)", rowcol.getRow(), rowcol.getCol());
		_cookies[rowcol.getRow()][rowcol.getCol()].setType(Nothing);
		_score += SCORE_PER_COOKIE;
		_hudLayerDelegate->updateScore(_score);
	});
	_cookieLayerDelegate->animateRemoveMatches(matches);
}

/**
 * \brief Remove all possible matches in cookie matrix
 */
void GameModel::removeAllMatches()
{
	set<RowCol> matches;
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			// find horizontal matches
			auto hor = this->findHorizontalMatches(row, col);
			matches.insert(hor.begin(), hor.end());

			// find vertical matches
			auto ver = this->findVerticalMatches(row, col);
			matches.insert(ver.begin(), ver.end());
		}
	}
	// no matches to remove
	if (matches.empty())
	{
		// win
		if (_score >= currentLevel().getTargetScore())
		{
			_hudLayerDelegate->showLevelComplete();
		}
		// lose
		else if (_movesLeft <= 0)
		{
			_hudLayerDelegate->showGameOver();
		}
		// not win and not lose, shuffle if no possible swap in this game
		else if (!this->hasPossibleSwap())
		{
			this->shuffle();
		}
		Director::getInstance()->getEventDispatcher()->setEnabled(true);
		return;
	}
	// remove all matches
	for_each(matches.begin(), matches.end(), [&](RowCol rowcol)
	{
		CCLOG("removing (%d %d)", rowcol.getRow(), rowcol.getCol());
		_cookies[rowcol.getRow()][rowcol.getCol()].setType(Nothing);
		_score += SCORE_PER_COOKIE;
		_hudLayerDelegate->updateLabels(currentLevel().getTargetScore(), _movesLeft, _score);
	});
	// animation
	_cookieLayerDelegate->animateRemoveMatches(matches);
}

/**
 * \brief Fall down cookies in hole
 * \param matches RowCol of cookies have already been removed
 */
void GameModel::fall(const set<RowCol>& matches)
{
	set<Faller> fallers; // stores fall down step for animation
	// for each cookie in _cookies
	for (int row = 0; row < NUM_ROWS - 1; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			// if this cookie is a hole (need to be fill)
			if (currentLevel().hasCookieAt(row, col) && _cookies[row][col].is(Nothing))
			{
				// get the nearest cookie (on top) and fall it down
				for (int up = row + 1; up < NUM_ROWS; up++)
				{
					if (!_cookies[up][col].is(Nothing))
					{
						_cookies[row][col].setType(_cookies[up][col].getType());
						_cookies[up][col].setType(Nothing);
						fallers.insert(Faller(up, col, up - row));
						break;
					}
				}
			}
		}
	}
	_cookieLayerDelegate->animateFall(fallers);
}

/**
 * \brief Fill empty cookie after falling down
 * \param fallers 
 */
void GameModel::fill(const set<Faller>& fallers)
{
	vector<vector<Cookie>> filled = vector<vector<Cookie>>(NUM_COLS, vector<Cookie>());; // cookies have been filled.
	for (int col = 0; col < NUM_COLS; col++)
	{
		for (int row = NUM_ROWS - 1; row >= 0; row --)
		{
			if (currentLevel().hasCookieAt(row, col))
			{
				if (!_cookies[row][col].is(Nothing))
					break;
				_cookies[row][col] = Cookie(row, col, Cookie::randomCookieType());
				filled[col].push_back(_cookies[row][col]);
			}
		}
	}
	_cookieLayerDelegate->animateFill(filled);
}

/**
 * \brief Load the next level
 */
void GameModel::loadNextLevel()
{
	if (_currLevel >= NUM_LEVELS - 1)
	{
		// todo: do somthing here to finish the game
		CCLOG("Final level complete");
		return;
	}
	this->loadLevel(_currLevel + 1);
	CCLOG("level %d loaded", _currLevel);
}

/**
 * \brief Load a level
 * \param level level need to be loaded
 */
void GameModel::loadLevel(int level)
{
	_currLevel = level;
	_movesLeft = currentLevel().getMaxMove();
	_score = 0;
	this->initCookies();
	_cookieLayerDelegate->animateBeginGame();
	_cookieLayerDelegate->addCookies(_cookies);
	_hudLayerDelegate->hideLevelComplete();
	_hudLayerDelegate->hideGameOver();
	_hudLayerDelegate->updateLabels(currentLevel().getTargetScore(), _movesLeft, _score);
}

/**
 * \brief called when player loses the game
 */
void GameModel::gameOver() const
{
	_cookieLayerDelegate->animateGameOver();
}

void GameModel::reloadLevel()
{
	this->loadLevel(_currLevel);
}

/**
 * \brief check whether current cookie matrix has possible swap or not
 * \return true: has at least one possible swap
 */
bool GameModel::hasPossibleSwap()
{
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			// skip "Nothing" cookie
			if (!currentLevel().hasCookieAt(row, col))
				continue;
			
			// just need to check top and right (no need for left and bot)

			// 1. top
			if (isValidIndex(row + 1, col) && currentLevel().hasCookieAt(row + 1, col))
			{
				// swap 2 cookies to see if any chain is made.
				std::swap(_cookies[row][col], _cookies[row + 1][col]);

				// if swaping makes chains
				if (this->hasChainAt(row, col) || this->hasChainAt(row + 1, col))
				{
					// swap back
					std::swap(_cookies[row][col], _cookies[row + 1][col]);
					return true;
				}
				
				// anyway, swap it back
				std::swap(_cookies[row][col], _cookies[row + 1][col]);
			}

			// 2. right
			if (isValidIndex(row, col + 1) && currentLevel().hasCookieAt(row, col + 1))
			{
				// swap 2 cookies to see if any chain is made.
				std::swap(_cookies[row][col], _cookies[row][col + 1]);

				// if swaping makes chains
				if (this->hasChainAt(row, col) || this->hasChainAt(row, col + 1))
				{
					// swap back
					std::swap(_cookies[row][col], _cookies[row][col + 1]);
					return true;
				}

				// anyway, swap it back
				std::swap(_cookies[row][col], _cookies[row][col + 1]);
			}
		}
	}

	// no possible swap has been found.
	return false;
}

/**
 * \brief check whether row, col is inside the cookie matrix
 * \param row row index
 * \param col column index
 * \return true = row, col is inside the cookie matrix
 */
bool GameModel::isValidIndex(int row, int col)
{
	return row >= 0 && row < NUM_ROWS && col >= 0 && col < NUM_COLS;
}

/**
 * \brief Loads level constant from json file when loading game
 */
void GameModel::loadLevelsConstant()
{
	for (int i = 0; i < NUM_LEVELS; i++)
	{
		auto fileName = string("Level_").append(to_string(i)).append(".json");
		_levels.push_back(Parser::parseLevel(fileName));
	}
}
