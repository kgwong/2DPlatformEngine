#include "Level.h"

#include "LevelLoader.h"
#include "MainGame.h"
#include "GameConstants.h"
#include "LevelMap.h"

#include <cassert>

Level::Level(MainGame* mainGame,
	GameWindow* window,
	ResourceLocator* resourceLocator,
	LevelMap* levelMap)
	:_mainGame(mainGame),
	_window(window),
	_resourceLocator(resourceLocator),
	_enemyBuilder(_resourceLocator),
	_levelMap(levelMap)
{
	addPickupAtLocation(Point(100, 100));
}


Level::~Level()
{
}

void Level::load(const std::string& filepath, TileSet* tileSet)
{
	LevelLoader::loadTiles(filepath + "Tiles", *this, tileSet);
	LevelLoader::loadEnemies(filepath + "Enemies", *this);

}

void Level::setPlayer(Player* p, Point newPlayerPosition)
{
	player = p;
	player->setPos(newPlayerPosition);
}

void Level::addPlayerProjectileAtLocation(Point position, int vel, Direction dir)
{
	playerProjectiles.push_back( PlayerProjectile(position, vel, _resourceLocator->getAnimation("Assets/Animations/playerProjectile.png"), dir) );
}

void Level::addPickupAtLocation(Point position)
{
	Pickup pickup(_resourceLocator->getSprite("Assets/Pickups/pickup.png"));
	pickup.setPos(position);
	pickups.push_back(pickup);
}

void Level::addEnemyAtLocation(const std::string& name, Point position)
{
	Enemy* enemy = _enemyBuilder.create(name);
	enemy->setPos(position);
	enemies.push_back(*enemy);
	delete enemy;//
}

void Level::update()
{
	updateTiles();
	updatePlayer();
	updatePlayerProjectiles();
	updateEnemies();
	updatePickups();
}

void Level::updateTiles()
{
	for (int r = 0; r < tileArrangement.rows; ++r)
		for (int c = 0; c < tileArrangement.cols; ++c)
			tileArrangement.tiles[r][c].update();
}

void Level::updatePlayer()
{
	player->update(*this);
}

void Level::updatePlayerProjectiles()
{
	updateEntityVector(playerProjectiles);
}

void Level::updateEnemies()
{
	updateEntityVector(enemies);
}

void Level::updatePickups()
{
	updateEntityVector(pickups);
}

void Level::render()
{
	//note order of rendering
	renderTiles();
	renderEnemies();
	renderPlayerProjectiles();
	renderPlayer();
	renderPickups();
}

int Level::getID()
{
	return _levelID;
}

void Level::setNextLevel(Block oldBlock, Point oldPlayerPosition, Direction dir)
{
	Block nextBlock = _levelMap->getAdjBlock(oldBlock + _levelMap->getBaseBlock(_levelID), dir);
	int nextLevelID = _levelMap->getLevelID(nextBlock);

	Block nextBaseBlock = _levelMap->getBaseBlock(nextLevelID);
	Block newRelativeBlock{ nextBlock.r - nextBaseBlock.r,
							nextBlock.c - nextBaseBlock.c };
	

	switch (dir)
	{
	case Direction::UP:
		oldPlayerPosition.y = Constants::BLOCK_HEIGHT_IN_PIXELS - 2 * Constants::TILE_SIZE;
		break;
	case Direction::DOWN:
		oldPlayerPosition.y = 2 * Constants::TILE_SIZE;
		break;
	case Direction::LEFT:
		oldPlayerPosition.x = Constants::BLOCK_WIDTH_IN_PIXELS - 2 * Constants::TILE_SIZE;
		break;
	case Direction::RIGHT:
		oldPlayerPosition.x = 2 * Constants::TILE_SIZE;
		break;
	}


	Point newPlayerPosition{ newRelativeBlock.c * Constants::BLOCK_WIDTH_IN_PIXELS + oldPlayerPosition.x,
								newRelativeBlock.r * Constants::BLOCK_HEIGHT_IN_PIXELS + oldPlayerPosition.y};

	_mainGame->setNextLevel(nextLevelID, newPlayerPosition);
}

int Level::getBlockWidth()
{
	return _blockWidth;
}

int Level::getBlockHeight()
{
	return _blockHeight;
}

void Level::renderTiles() 
{
	for (int r = 0; r < tileArrangement.rows; ++r)
		for (int c = 0; c < tileArrangement.cols; ++c)
			tileArrangement.tiles[r][c].render();
}

void Level::renderEnemies()
{
	renderEntityVector(enemies);
}

void Level::renderPlayerProjectiles()
{
	renderEntityVector(playerProjectiles);
}

void Level::renderPickups()
{
	renderEntityVector(pickups);
}

void Level::renderPlayer()
{
	player->render();
}

int Level::getLevelWidth() const
{
	return tileArrangement.cols * Constants::TILE_SIZE;
}

int Level::getLevelHeight() const
{
	return tileArrangement.rows * Constants::TILE_SIZE;
}