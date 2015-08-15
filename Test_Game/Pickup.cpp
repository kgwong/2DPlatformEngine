#include "Pickup.h"

#include "StaticSpriteRenderer.h"

Pickup::Pickup(Sprite* sprite)
	:_alive(true), 
	_renderComponent(new StaticSpriteRenderer(sprite)),
	_colliderComponent(0, 0, _renderComponent->getWidth(), _renderComponent->getHeight())
{
}


Pickup::~Pickup()
{
}

void Pickup::update(Level& level)
{
	_colliderComponent.update(*this);
	_physicsComponent.update(*this, level, &_colliderComponent);
}

void Pickup::render()
{
	_renderComponent->update(*this);
}

bool Pickup::isDead()
{
	return !_alive;
}

EntityType Pickup::getType() const
{
	return EntityType::PICKUP;
}

void Pickup::onCollision(CollisionInfo& collision)
{
	if (collision.other.getType() == EntityType::PLAYER)
		_alive = false;
}

Component* Pickup::getComponent(ComponentType type)
{
	if (type == ComponentType::COLLIDER)
		return &_colliderComponent;
	return nullptr;
}