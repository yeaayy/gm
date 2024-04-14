#include "game/object.hpp"

#include "game/world_data.hpp"

#include "list.tcc"

GameObject::GameObject(int type, int x, int y, int width, int height)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

GameObject::~GameObject()
{
}

void GameObject::onPlaced(WorldData *dst)
{
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			// dst->game[(y + i) * (dst->width - 1) + x + j] = this;
            dst->objectGrid[(y + i) * (dst->width - 1) + x + j] = this;
		}
	}
}

void GameObject::onRemoved(WorldData *dst)
{
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			dst->objectGrid[(y + i) * dst->width + x + j] = nullptr;
		}
	}
}

int GameObject::getType()
const {
	return type;
}

int GameObject::getX()
const {
	return x;
}

int GameObject::getY()
const {
	return y;
}

int GameObject::getWidth()
const {
	return width;
}

int GameObject::getHeight()
const {
	return height;
}
