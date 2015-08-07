#ifndef TILE_ARRANGEMENT_H
#define TILE_ARRANGEMENT_H	

#include "TileSet.h"
#include "Tile.h"

struct TileArrangement
{
	std::vector<std::vector<Tile>> tiles;
	int tileSize;
	int rows;
	int cols;
};

#endif //TILE_ARRANGEMENT_H

