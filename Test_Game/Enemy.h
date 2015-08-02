#ifndef ENEMY_H
#define ENEMY_H

#include "HealthComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

#include "GameWindow.h"
#include "Sprite.h"

class Level;
class PhysicsComponent;

class Enemy : public GameObject
{
public:
	Enemy(Sprite* sprite);
	virtual ~Enemy();

	void update(Level& level);
	void render();

	void takeDamage(int damage);
	bool isDead();

	virtual std::string getName();
	virtual Component* getComponent(ComponentType type);
	virtual EntityType getType() const;
	virtual void onCollision(GameObject& other);
	virtual void onDeath(Level& level);

private:
	GameWindow* _gw;
	Sprite* _sprite;
	HealthComponent _healthComponent;
	PhysicsComponent _physicsComponent;
	ColliderComponent _colliderComponent;

};

#endif //ENEMY_H

