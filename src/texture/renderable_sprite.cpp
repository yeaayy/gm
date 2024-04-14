#include "texture/renderable_sprite.hpp"

#include "log.h"

RenderableSprite::RenderableSprite()
{
	this->data = nullptr;
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

RenderableSprite::RenderableSprite(const TextureSprite *data,
								   float x,
								   float y,
								   float z)
{
	this->data = data;
	this->x = x;
	this->y = y;
	this->z = z;
}

RenderableSprite::~RenderableSprite()
{}


void RenderableSprite::render()
{
	data->renderAt(x, y, z);
}
