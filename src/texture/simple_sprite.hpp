#ifndef TEXTURE_SIMPLE_SPRITE_HPP
#define TEXTURE_SIMPLE_SPRITE_HPP

#include "texture/sprite.hpp"

class SimpleTextureSprite : public TextureSprite
{
  public:
	SimpleTextureSprite(
        const char *name,
		TextureAtlas *texture,
		int indexStart,
		int indexCount);
	TextureAtlas *getTexture() const override;
	int getIndexStart() const override;
	int getIndexCount() const override;
	TextureSprite *copy() const override;

  protected:
	TextureAtlas *texture;
	int indexStart;
	int indexCount;
};

#endif /* TEXTURE_SIMPLE_SPRITE_HPP */
