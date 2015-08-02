#include "Enemy.h"

#include "Level.h"
#include "Pickup.h"

Enemy::Enemy(Sprite* sprite)
	: _sprite(sprite), _healthComponent(100)
{
	width = _sprite->getWidth();
	height = _sprite->getHeight();
}


Enemy::~Enemy()
{
}

void Enemy::update(Level& level)
{
	_colliderComponent.update(*this);
	_physicsComponent.update(*this, level, &_colliderComponent);
}

void Enemy::render()
{
	_sprite->render(position.x, position.y);
}

void Enemy::takeDamage(int damage)
{
	_healthComponent.takeDamage(damage);
}

bool Enemy::isDead()
{
	return _healthComponent.isDead();
}

void Enemy::onDeath(Level& level)
{
	level.addPickupAtLocation(position);
}

Component* Enemy::getComponent(ComponentType type)
{
	if (type == COLLIDER)
		return &_colliderComponent;
	return nullptr;
}

std::string Enemy::getName()
{
	return "Enemy";
}

EntityType Enemy::getType() const
{
	return ENEMY;
}

void Enemy::onCollision(GameObject& other)
{
	if (other.getType() == PLAYER_PROJECTILE)
	{
		//well this is ugly
		takeDamage(*((int*)other.getComponent(DAMAGE)->getAttribute("damage"))); 
	}
}