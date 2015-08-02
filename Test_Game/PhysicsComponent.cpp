#include "PhysicsComponent.h"

#include "GameObject.h"
#include "Level.h"

PhysicsComponent::PhysicsComponent()
	:_velX(0), _velY(0),
	_accelX(0), _accelY(0),
	_gravityEnabled(true), _falling(true)
{
}


PhysicsComponent::~PhysicsComponent()
{
}


void PhysicsComponent::update(GameObject& owner, Level& level, ColliderComponent* collider)
{
	updatePosition(owner, X_AXIS);
	updatePositionAfterCollision(owner, level, collider, X_AXIS);

	updatePosition(owner, Y_AXIS);
	updatePositionAfterCollision(owner, level, collider, Y_AXIS);
}

void PhysicsComponent::enableGravity(bool gravity)
{
	_gravityEnabled = gravity;
}

void PhysicsComponent::enactGravity()
{
	if (_gravityEnabled)
	{
		_velY += GRAVITY;
		if (_velY > TERMINAL_VELOCITY)
			_velY = TERMINAL_VELOCITY;
	}

	_falling = true;
}

void PhysicsComponent::setVelX(int velX)
{
	_velX = velX;
}

void PhysicsComponent::setVelY(int velY)
{
	_velY = velY;
}

void PhysicsComponent::setAccelX(int accelX)
{
	_accelX = accelX;
}

void PhysicsComponent::setAccelY(int accelY)
{
	_accelY = accelY;		
}


int PhysicsComponent::getVelX() const
{
	return _velX;
}

int PhysicsComponent::getVelY() const
{
	return _velY;
}

int PhysicsComponent::getAccelX() const
{
	return _accelX;
}

int PhysicsComponent::getAccelY() const
{
	return _accelY;		
}

bool PhysicsComponent::isMovingUp() const
{
	return _velY < 0;
}

bool PhysicsComponent::isMovingDown() const
{
	return _velY > 0;
}

bool PhysicsComponent::isMovingLeft() const
{
	return _velX < 0;
}

bool PhysicsComponent::isMovingRight() const
{
	return _velX > 0;
}

bool PhysicsComponent::isFalling() const
{
	return _falling;
}

void PhysicsComponent::updatePosition(GameObject& owner, Axis axis)
{
	switch(axis)
	{
		case X_AXIS:
			_velX += _accelX; 
			owner.setPosX(owner.getPosX() + _velX);
			break;
		case Y_AXIS:
			enactGravity();
			_velY += _accelY; 
			owner.setPosY(owner.getPosY() + _velY);
			break;
		default:
			break;
	}
}

void PhysicsComponent::updatePositionAfterCollision(GameObject& owner, Level& level, ColliderComponent* collider, Axis axis)
{
	if (collider)
	{
		collider->update(owner);

		if (owner.getType() != ENVIRONMENT)
		{
			for (int c = owner.getPosX()/level.tileArrangement._tileSize; c <= (owner.getPosX()+owner.getWidth())/level.tileArrangement._tileSize; ++c)
			{
				for(int r = owner.getPosY()/level.tileArrangement._tileSize; r <= (owner.getPosY()+owner.getHeight())/level.tileArrangement._tileSize; ++r)
				{
					Tile& tile = level.tileArrangement._tiles[r][c];

					if(tile.tileType == 1 && collider->checkCollision(tile))
						handleCollision(owner, tile, collider, axis);
				}
			}
		}

		if (owner.getType() != ENEMY)
		{
			for (auto enemyIter = level.enemies.begin(); enemyIter < level.enemies.end(); ++enemyIter)
			{
				if(collider->checkCollision(*enemyIter))
					handleCollision(owner, *enemyIter, collider, axis);
			}
		}

		if (owner.getType() == PLAYER) //only player has collision with pickups 
		{
			for (auto pickupIter = level.pickups.begin(); pickupIter < level.pickups.end(); ++pickupIter)
			{
				if (collider->checkCollision(*pickupIter))
					handleCollision(owner, *pickupIter, collider, axis); //make this a non-physical collision!
			}
		}
	}

}

void PhysicsComponent::handleCollision(GameObject& owner, GameObject& other, ColliderComponent* collider, Axis axis)
{
	switch(axis)
	{
		case X_AXIS:
			if (isMovingLeft())
				owner.setPosX(other.getPosX() + other.getWidth());
			else //isMovingRight
				owner.setPosX(other.getPosX() - owner.getWidth());
			setVelX(0);
			break;

		case Y_AXIS:
			if (isMovingUp())
				owner.setPosY(other.getPosY() + other.getHeight());
			else //isMovingDown
			{
				owner.setPosY(other.getPosY() - owner.getHeight());
				_falling = false;
			}
			setVelY(0);
			break;
		default:
			break;
	}
	collider->update(owner); 
	other.onCollision(owner);
	owner.onCollision(other);
}