#include "VisibleRect.h"

Rect VisibleRect::_visibleRect;
void VisibleRect::init()
{
	_visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();
}

Point VisibleRect::center()
{
	init();
	return Point(_visibleRect.origin.x + _visibleRect.size.width / 2, _visibleRect.origin.y + _visibleRect.size.height / 2);
}

Point VisibleRect::left()
{
	init();
	return Point(_visibleRect.origin.x, _visibleRect.origin.y + _visibleRect.size.height / 2);
}

Point VisibleRect::right()
{
	init();
	return Point(_visibleRect.origin.x + _visibleRect.size.width, _visibleRect.origin.y + _visibleRect.size.height / 2);
}

Point VisibleRect::top()
{
	init();
	return Point(_visibleRect.origin.x + _visibleRect.size.width / 2, _visibleRect.origin.y + _visibleRect.size.height);
}

Point VisibleRect::bot()
{
	init();
	return Point(_visibleRect.origin.x + _visibleRect.size.width / 2, _visibleRect.origin.y);
}

Point VisibleRect::topleft()
{
	init();
	return Point(_visibleRect.origin.x, _visibleRect.origin.y + _visibleRect.size.height);
}

Point VisibleRect::topright()
{
	init();
	return Point(_visibleRect.origin.x + _visibleRect.size.width, _visibleRect.origin.y + _visibleRect.size.height);
}

Point VisibleRect::botleft()
{
	init();
	return _visibleRect.origin;
}

Point VisibleRect::botright()
{
	init();
	return Point(_visibleRect.origin.x + _visibleRect.size.width, _visibleRect.origin.y);
}

Rect VisibleRect::getRect()
{
	init();
	return _visibleRect;
}
