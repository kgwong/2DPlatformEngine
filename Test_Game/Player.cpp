#include "Player.h"

#include "Level.h"
#include "PhysicsComponent.h"
#include "AnimationRenderer.h"

Player::Player(ResourceLocator* resourceLocator)
	: dir(Direction::RIGHT), 
	_animation(resourceLocator->getAnimation("Assets/Animations/playerAnimation.png")),
	_renderComponent(new AnimationRenderer(_animation)),
	_colliderComponent(0, 0, _renderComponent->getWidth(), _renderComponent->getHeight()),
	_shoot(false), 
	_flying(false)
{
}

Player::~Player()
{
}

void Player::handleInput2()
{
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);

	_physicsComponent.setVelX(0);


	if (_flying)
	{
		if (keyStates[SDL_SCANCODE_LEFT])
		{
			_physicsComponent.setVelX(-5);
			dir = Direction::LEFT;
		}
		if (keyStates[SDL_SCANCODE_RIGHT])
		{
			_physicsComponent.setVelX(5);
			dir = Direction::RIGHT;
		}
		if (keyStates[SDL_SCANCODE_A])
		{
			_physicsComponent.setVelY(0);
		}
		else
			_flying = false;
		if (keyStates[SDL_SCANCODE_UP])
		{
			dir = Direction::UP;
			_physicsComponent.setVelY(-5);
		}

		if (keyStates[SDL_SCANCODE_DOWN])
		{
			dir = Direction::DOWN;
			_physicsComponent.setVelY(5);
		}


	}
	else
	{
		if (keyStates[SDL_SCANCODE_LEFT])
		{
			_physicsComponent.setVelX(-5);
			dir = Direction::LEFT;
		}
		if (keyStates[SDL_SCANCODE_RIGHT])
		{
			_physicsComponent.setVelX(5);
			dir = Direction::RIGHT;
		}
		if (keyStates[SDL_SCANCODE_UP])
		{
			dir = Direction::UP;
		}
		if (keyStates[SDL_SCANCODE_DOWN])
		{
			dir = Direction::DOWN;
		}
	}
}

void Player::handleInput(SDL_Event &e)
{
	switch (e.key.keysym.sym)
	{
		case SDLK_a:
			if (!_physicsComponent.isFalling())
				jump();
			else
				_flying = true;
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
	_renderComponent->update(*this);
}

void Player::update(Level& level)
{
	_colliderComponent.update(*this);
	_physicsComponent.update(*this, level, &_colliderComponent);

	if (_shoot)
		shoot(level);

	if (!_physicsComponent.isFalling())
		_flying = false; 

	if (_flying)
		std::cout << "Flying" << std::endl;
	else
		std::cout << "g" << std::endl;

	if (_colliderComponent.getLeft() < 0)
		level.setNextLevel(_oldBlock, _oldPosInBlock, Direction::LEFT );
	else if (_colliderComponent.getRight() > level.getLevelWidth())
		level.setNextLevel(_oldBlock, _oldPosInBlock, Direction::RIGHT);
	else if (_colliderComponent.getTop() < 0)
		level.setNextLevel(_oldBlock, _oldPosInBlock, Direction::UP);
	else if (_colliderComponent.getBottom() > level.getLevelHeight())
		level.setNextLevel(_oldBlock, _oldPosInBlock, Direction::DOWN);
	
	_oldBlock = Block::getBlock(getPos());
	_oldPosInBlock = Point{ getPosX() % Constants::BLOCK_WIDTH_IN_PIXELS,
							getPosY() % Constants::BLOCK_HEIGHT_IN_PIXELS };
}

void Player::jump()
{
	//if (!_physicsComponent.isFalling())
		_physicsComponent.setVelY(JUMP_VELOCITY);
}

void Player::shoot(Level& level)
{
	level.addPlayerProjectileAtLocation(position, 30, dir);
	_shoot = false;
}


Component* Player::getComponent(ComponentType type)
{
	if (type == ComponentType::COLLIDER)
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
	return EntityType::PLAYER;
}

void Player::onCollision(CollisionInfo& collision)
{
	if (collision.other.getType() == EntityType::PICKUP)
	{
		std::cout << "thing acquired! yay!" << std::endl;
	}
}