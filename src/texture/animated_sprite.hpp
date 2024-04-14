#ifndef RENDERABLE_SPRITE_HPP
#define RENDERABLE_SPRITE_HPP

#include "texture/atlas.hpp"
#include "texture/sprite.hpp"
#include "updatable.hpp"

class AnimatedSprite : public TextureSprite, public Updatable
{
  public:
	AnimatedSprite(const char *name, int frameCount, const TextureSprite *const* frames, float fps);
	~AnimatedSprite();
    void update(float dt) override;
	TextureAtlas *getTexture() const override;
	int getIndexStart() const override;
	int getIndexCount() const override;
	TextureSprite *copy() const override;

  private:
	int frameIndex;
	int frameCount;
    const TextureSprite **frames;
    float fps;
    float time;
};

#endif /* RENDERABLE_SPRITE_HPP */
