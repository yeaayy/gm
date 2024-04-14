#ifndef TEXTURE_RENDERABLE_SPRITE_HPP
#define TEXTURE_RENDERABLE_SPRITE_HPP

#include "texture/sprite.hpp"

class RenderableSprite
{
  public:
	RenderableSprite();
	RenderableSprite(const TextureSprite *data,
					 float x,
					 float y,
					 float z);
	virtual ~RenderableSprite();
	virtual void render();

	const TextureSprite *data;
	float x, y, z;
};

#endif /* TEXTURE_RENDERABLE_SPRITE_HPP */
