#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

namespace Constants
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	const int TILE_SIZE = 32;

	const int TILES_PER_BLOCK_WIDTH = SCREEN_WIDTH / TILE_SIZE;
	const int TILES_PER_BLOCK_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

	const int BLOCK_WIDTH_IN_PIXELS = TILES_PER_BLOCK_WIDTH * TILE_SIZE;
	const int BLOCK_HEIGHT_IN_PIXELS = TILES_PER_BLOCK_HEIGHT * TILE_SIZE;
}

#endif //GAME_CONSTANTS_H