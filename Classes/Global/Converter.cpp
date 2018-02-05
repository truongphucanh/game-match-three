#include "Converter.h"
#include "Marcos.h"


/**
 * \brief Converts RowCol to location in CookieLayer
 * \param row 
 * \param col 
 * \return Location (of cookie sprite) in CookieLayer
 */
Vec2 Converter::toLocation(int row, int col)
{
	return Vec2(static_cast<float>(col) * TILE_WIDTH + TILE_WIDTH / 2, static_cast<float>(row) * TILE_HEIGHT + TILE_HEIGHT / 2);
}
/**
 * \brief Convert location (in CookieLayer) to RowCol
 * \param location in CookieLayer
 * \return RowCol of location
 */
RowCol Converter::toRowCol(const Vec2& location)
{
	if (location.x < 0 || location.x >= COOKIELAYER_WIDTH || location.y < 0 || location.y >= COOKIELAYER_HEIGHT)
		return RowCol(-1, -1); // invaid row, col
	int row = static_cast<int>(location.y / TILE_HEIGHT);
	int col = static_cast<int>(location.x / TILE_WIDTH);
	return RowCol(row, col);
}

