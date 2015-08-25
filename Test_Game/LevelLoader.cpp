#include "LevelLoader.h"

#include "Level.h"
#include "GameConstants.h"
#include "MainGame.h"

#include <fstream>


LevelLoader::LevelLoader(MainGame* mainGame,
	ResourceLocator* resourceLocator,
	LevelMap* levelMap)
	:_mainGame(mainGame),
	_resourceLocator(resourceLocator),
	_levelMap(levelMap)
{
}


LevelLoader::~LevelLoader()
{
}

Level& LevelLoader::getLevel(const std::string& filepath, TextureSheet* tileSet, TileCreator* creator)
{
	if (_loadedLevels.count(filepath) == 0)
	{
		auto level = _loadedLevels.insert({ filepath, Level(_mainGame, _resourceLocator, _levelMap) });
		level.first->second.load(filepath, tileSet, creator);
	}
	return _loadedLevels.at(filepath);
}

void LevelLoader::loadTiles(const std::string& filepath, Level& level, TextureSheet* tileSet, TileCreator* creator)
{
	std::ifstream ifs(filepath); //should probably do more error checking
	if (!ifs)
		std::cout << "Failed to load: " << filepath << std::endl;

	ifs >> level._levelID;

	int blockWidth, blockHeight;
	ifs >> blockWidth >> blockHeight;

	int numRows = blockHeight * Constants::TILES_PER_BLOCK_HEIGHT;
	int numCols = blockWidth * Constants::TILES_PER_BLOCK_WIDTH;

	level.tileArrangement.rows = numRows;
	level.tileArrangement.cols = numCols;
	level.tileArrangement.tiles = std::vector<std::vector<Tile>> (numRows, std::vector<Tile>(numCols));

	for (int r = 0; r < numRows; ++r)
	{
		for (int c = 0; c < numCols; ++c)
		{
			int tileNum;
			ifs >> tileNum;
			level.tileArrangement.tiles[r][c] = creator->create(tileSet, tileNum, r, c);
			//level.tileArrangement.tiles[r][c] = tileSet->createTile(tileNum, r, c);
		}
	}
}

void LevelLoader::loadEnemies(const std::string& filepath, Level& level)
{
	std::ifstream ifs(filepath);
	if (!ifs)
		std::cout << "Failed to load: " << filepath << std::endl;

	std::string enemyName;
	Point pos;

	while (ifs >> enemyName >> pos.x >> pos.y)
	{
		level.addEnemyAtLocation(enemyName, pos);
	}

}