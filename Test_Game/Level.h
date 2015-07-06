#ifndef LEVEL_H
#define LEVEL_H

#include <fstream>
#include <vector>
#include <forward_list>
#include <list>

#include "TileSet.h"
#include "Sprite.h"
#include "GameWindow.h"
#include "LevelEntities.h"

class Level
{
public:
	Level(GameWindow* window, TileSet* tileSet);
	~Level();

	void load(const std::string& filepath);
	void setPlayer(Player* player);

	void update();
	void updatePlayer();
	void updatePlayerProjectiles();

	void render();
	void renderPlayer();
	void renderTiles();
	void renderEnemies();
	void renderPlayerProjectiles();

	int getLevelWidth() const;
	int getLevelHeight() const;

	int getTileSize() const;


public:
	//std::vector<std::vector<Tile>> tiles;
	//std::vector<Enemy> enemies; //should probably change this to a forward list
	//std::list<PlayerProjectile> playerProjectiles;
	/*maybe std::vector would be better, projectiles are not expected to last long,
	so the length of the vector would short, therefore random removals would not 
	take long either. vector also is more cache friendly*/
		LevelEntities _entities;

private:
	Sprite enemySprite;
	GameWindow* _window;
	TileSet* _tileSet;



	int _tileSize;

	int _numTilesWide;
	int _numTilesHigh;
};

#endif //LEVEL_H

