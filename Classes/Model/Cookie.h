#pragma once
#include "cocos2d.h"
#include "Global/Marcos.h"

USING_NS_CC;

enum CookieType
{
	Nothing = 0, Croissant, Cupcake, Danish, Donut, Macaroon, SugarCookie
};

class Cookie
{
private:
	static const std::string SpriteNames[NUM_COOKIE_TYPES];
	CC_SYNTHESIZE(int, _row, Row);
	CC_SYNTHESIZE(int, _col, Col);
	CC_SYNTHESIZE(CookieType, _type, Type);
public:
	Cookie();
	explicit Cookie(int row, int col, const CookieType& type);
	Cookie& operator=(const Cookie& rhs);

	std::string getSpriteName() const;
	std::string getHighlightedSpriteName() const;

	bool is(CookieType type) const;

	void reset();

	static CookieType randomCookieType();
	friend bool operator==(const Cookie& lhs, const Cookie& rhs);
	friend bool operator<(const Cookie& lhs, const Cookie& rhs);
	friend bool operator>(const Cookie& lhs, const Cookie& rhs);
};