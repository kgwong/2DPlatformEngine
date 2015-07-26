#include "Player.h"

#include "LevelEntities.h"

Player::Player(GameWindow* gw)
	:_renderComponent(gw), dir(RIGHT), _animation(gw, "playerAnimation.png", 4, 30, 60, 4, 1), 
	projectileAnimation(gw, "playerProjectile.png", 2, 20, 10, 2, 1), _gw(gw),
	_shoot(false)
{
	_renderComponent.setAnimation(&_animation);

	width = _animation.getWidth();
	height = _animation.getHeight();
}


Player::~Player()
{
}

void Player::handleInput2()
{
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);

	if(keyStates[SDL_SCANCODE_LEFT])
	{
		_physicsComponent.setVelX(-5);
		dir = LEFT;
	}
	else if(keyStates[SDL_SCANCODE_RIGHT])
	{
		_physicsComponent.setVelX(5);
		dir = RIGHT;
	}
	else if (keyStates[SDL_SCANCODE_UP])
	{
		dir = UP;
		_physicsComponent.setVelX(0);
	}
	else if (keyStates[SDL_SCANCODE_DOWN])
	{
		dir = DOWN;
		_physicsComponent.setVelX(0);
	}
	else
		_physicsComponent.setVelX(0);

}

void Player::handleInput(SDL_Event &e)
{
	switch (e.key.keysym.sym)
	{
		case SDLK_a:
			jump();
			break;
		case SDLK_s:
			_shoot = true;
			break;
		default:
			break;
	}
}

void Player::render()
{
	_renderComponent.update(*this);
}

void Player::update(LevelEntities& entities)
{
	_colliderComponent.update(*this);
	_physicsComponent.update(*this, entities, &_colliderComponent);

	if (_shoot)
		shoot(entities);

}

void Player::jump()
{
	if (!_physicsComponent.isFalling())
		_physicsComponent.setVelY(JUMP_VELOCITY);
}

void Player::shoot(LevelEntities& entities)
{
	entities.playerProjectiles.push_back(PlayerProjectile(position, 30, &projectileAnimation, dir));
	_shoot = false;
}


Component* Player::getComponent(const std::string& componentName)
{
	if (componentName == "Collider")
		return &_colliderComponent;
	else 
		return nullptr;
}

std::string Player::getName() const
{
	return "Player";
}

EntityType Player::getType() const
{
	return PLAYER;
}

void Player::onCollision(GameObject& other)
{
	if (other.getType() == PICKUP)
	{
		std::cout << "thing acquired! yay!" << std::endl;
	}
}