#pragma once

#include "cocos2d.h"

USING_NS_CC;

class VisibleRect
{
private:
	static Rect _visibleRect;
public:
	static void init();
	static Point center();
	static Point top();
	static Point bot();
	static Point left();
	static Point right();
	static Point topleft();
	static Point topright();
	static Point botleft();
	static Point botright();
	static Rect getRect();
};
