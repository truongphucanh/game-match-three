#pragma once
#include "Model/Level.h"
#include "json/document.h"

#include "cocos2d.h"

USING_NS_CC;
using namespace rapidjson;

class Parser
{
private:
	static Document _document;
public:
	static Level parseLevel(const string& fileName);
};
