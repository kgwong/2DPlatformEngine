#include "Level.h"

#include "Resources.h"


Level::Level(GameWindow* window, TileSet* tileSet) //
	:_window(window), _tileSet(tileSet), enemySprite(window, "Enemies/enemy.png"), _tileSize(tileSet->getTileSize())
{

	Enemy newEnemy(_window, &enemySprite);
	newEnemy.setPos(660, 660);
	_entities.enemies.push_back(newEnemy);

	Enemy newEnemy1(_window, &enemySprite);
	newEnemy1.setPos(630, 660);
	_entities.enemies.push_back(newEnemy1);

	Enemy newEnemy2(_window, &enemySprite);
	newEnemy2.setPos(444, 444);
	_entities.enemies.push_back(newEnemy2);

	Enemy newEnemy3(_window, &enemySprite);
	newEnemy3.setPos(333, 333);
	_entities.enemies.push_back(newEnemy3);

	_entities.tileSize = tileSet->getTileSize();
}


Level::~Level()
{
}

void Level::load(const std::string& filepath)
{
	std::ifstream map(getLevelsPath(filepath)); //should probably do more error checking
	if (map == nullptr)
		std::cout << "Loading Level failed!" << std::endl;

	map >> _numTilesWide >> _numTilesHigh;

	_entities.tiles = std::vector<std::vector<Tile>>(_numTilesHigh, std::vector<Tile>(_numTilesWide));

	for (int y = 0; y < _numTilesHigh; ++y)
	{
		for (int x = 0; x < _numTilesWide; ++x)
		{
			int tileNum;
			map >> tileNum;
			_entities.tiles[y][x] = _tileSet->createTile(tileNum, x, y);
		}
	}
}

void Level::setPlayer(Player* player)
{
	_entities.player = player;
}

void Level::update()
{
	updatePlayer();
	updatePlayerProjectiles();
}

void Level::updatePlayer()
{
	_entities.player->update(_entities);
}

void Level::updatePlayerProjectiles()
{
	std::cout << _entities.playerProjectiles.size() << std::endl;
	/*for (auto i = _entities.playerProjectiles.begin(); i != _entities.playerProjectiles.end(); ++i)
	{
		i->update(_entities);
	}*/

	auto it = _entities.playerProjectiles.begin();
	while (it != _entities.playerProjectiles.end())
	{
		if (it->isActive())
		{
			it->update(_entities);
			++it;
		}
		else
		{
			it = _entities.playerProjectiles.erase(it);
		}
	}
}


void Level::render()
{
	renderPlayer();
	renderTiles();
	renderEnemies();
	renderPlayerProjectiles();
}

void Level::renderPlayer()
{
	_entities.player->render();
}

void Level::renderTiles() 
{
	for (int y = 0; y < _numTilesHigh; ++y)
		for (int x = 0; x < _numTilesWide; ++x)
			_entities.tiles[y][x].render();
}

void Level::renderEnemies()
{
	for (auto i = _entities.enemies.begin(); i < _entities.enemies.end(); ++i)
		i->draw();
}

void Level::renderPlayerProjectiles()
{
	for (auto i = _entities.playerProjectiles.begin(); i != _entities.playerProjectiles.end(); ++i)
	{
		i->render();
	}
}

int Level::getLevelWidth() const
{
	return _numTilesWide * _tileSet->getTileSize();
}

int Level::getLevelHeight() const
{
	return _numTilesHigh * _tileSet->getTileSize();
}

int Level::getTileSize() const
{
	return _tileSize;
}
