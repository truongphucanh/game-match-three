#include "Parser.h"

Document Parser::_document;

/**
 * \brief parse Level from json file
 * \param fileName .json file name
 * \return parsed level object
 */
Level Parser::parseLevel(const string& fileName)
{
	auto content = FileUtils::getInstance()->getDataFromFile(fileName);
	auto buffer = reinterpret_cast<const char*>(content.getBytes());
	auto len = content.getSize();
	string js(buffer, len);
	_document.Parse<0>(js.c_str());
	if (_document.HasParseError())
	{
		CCLOG("%d", len);
		return Level();
	}
	int targetScore = _document["targetScore"].GetInt();
	int moves = _document["moves"].GetInt();
	int hasCookie[NUM_ROWS][NUM_COLS];
	rapidjson::Value &tiles = _document["tiles"];
	for (int i = 0; i < tiles.Size(); i++)
	{
		rapidjson::Value &row = tiles[i];
		for (int j = 0; j < row.Size(); j++)
		{
			int val = row[j].GetInt();
			hasCookie[i][j] = val;
		}
	}
	return Level(hasCookie, targetScore, moves);
}
