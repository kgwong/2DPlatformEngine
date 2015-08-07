#include "TileSet.h"

#include <cassert>

#include "SpriteSheet.h"


TileSet::TileSet(GameWindow* window, std::shared_ptr<SDL_Texture> texture, 
					SpritesheetInfo spritesheetInfo)
	: _window(window), _texture(texture), _numTiles(spritesheetInfo.num), _tileSize(spritesheetInfo.width)
{
	assert(spritesheetInfo.width == spritesheetInfo.height); //square tiles only
	calculateSpriteLocations(_tiles, spritesheetInfo);
}

TileSet::~TileSet()
{
}

SDL_Texture* TileSet::getTexture()
{
	return _texture.get();
}

SDL_Rect TileSet::getTile(int tileIndex)
{
	return _tiles[tileIndex];
}

Tile TileSet::createTile(int tileIndex, int r, int c)
{
	return Tile(_window, _texture.get(), _tiles[tileIndex], static_cast<TileType>(tileIndex), r, c, _tileSize);
}

int TileSet::getTileSize()
{
	return _tileSize;
}

int TileSet::getNumTiles()
{
	return _numTiles;
}