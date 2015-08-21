#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "GameWindow.h"
#include "Animation.h"
#include "Direction.h"
#include "Block.h"

#include "RenderComponent.h"
#include "HealthComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"


#include "ResourceLocator.h"

class Level;
class PhysicsComponent;

class Player : public GameObject
{

public:
	Player(ResourceLocator* resourceLocator);
	~Player();

	void handleInput(SDL_Event &e);
	void handleInput2(); //

	void update(Level& Level);
	void jump();
	void shoot(Level& level);

	virtual Component* getComponent(ComponentType type);
	virtual std::string getName() const;
	virtual EntityType getType() const;
	virtual void onCollision(CollisionInfo& collision);

	void render();

private:
	static const int JUMP_VELOCITY = -20; //negative Y means up!

	Animation* _animation;

	std::unique_ptr<RenderComponent> _renderComponent;
	HealthComponent _healthComponent;
	PhysicsComponent _physicsComponent;
	ColliderComponent _colliderComponent;

	Block _oldBlock;
	Point _oldPosInBlock;

	Direction dir; 

	bool _shoot;
	bool _flying;

};

#endif //PLAYER_H

