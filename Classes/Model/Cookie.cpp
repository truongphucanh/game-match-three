#include "Cookie.h"

const std::string Cookie::SpriteNames[NUM_COOKIE_TYPES] = { "Nothing", "Croissant", "Cupcake", "Danish", "Donut", "Macaroon", "SugarCookie" };

Cookie::Cookie()
{
	_row = -1;
	_col = -1;
	_type = Nothing;
}

Cookie::Cookie(int row, int col, const CookieType& type)
{
	_row = row;
	_col = col;
	_type = type;
}

Cookie& Cookie::operator=(const Cookie& rhs)
{
	if (this == &rhs)
		return *this;
	_row = rhs.getRow();
	_col = rhs.getCol();
	_type = rhs.getType();
	return *this;
}

/**
* \brief
* \return Sprite name of this cookie
*/
std::string Cookie::getSpriteName() const
{
	return SpriteNames[_type];
}

/**
* \brief
* \return Sprite name of this cookie when highlighted
*/
std::string Cookie::getHighlightedSpriteName() const
{
	return this->getSpriteName().append("-Highlighted");
}

/**
 * \brief Check cookie type 
 * \param type 
 * \return 
 */
bool Cookie::is(CookieType type) const
{
	return _type == type;
}

/**
* \brief
* \return a random cookie type (exclude CookieType::Nothing)
*/
CookieType Cookie::randomCookieType()
{
	return static_cast<CookieType>(rand() % (NUM_COOKIE_TYPES - 1) + 1);
}

/**
* \brief Implemented for using Cookie in std::set
* \param lhs
* \param rhs
* \return
*/
bool operator==(const Cookie& lhs, const Cookie& rhs)
{
	return lhs.getRow() == rhs.getRow() && lhs.getCol() == rhs.getCol();
}

/**
* \brief for using cookie in std::set
* \param lhs
* \param rhs
* \return
*/
bool operator<(const Cookie& lhs, const Cookie& rhs)
{
	return lhs.getRow() < rhs.getRow() || (lhs.getRow() == rhs.getRow() && lhs.getCol() < rhs.getCol());
}

/**
* \brief for using cookie in std::set
* \param lhs
* \param rhs
* \return
*/
bool operator>(const Cookie& lhs, const Cookie& rhs)
{
	return !(lhs == rhs) && !(lhs < rhs);
}
