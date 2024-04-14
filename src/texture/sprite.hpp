#ifndef TEXTURE_SPRITE_HPP
#define TEXTURE_SPRITE_HPP

class TextureAtlas;

class TextureSprite
{
  public:
	TextureSprite(const char *name);
	virtual ~TextureSprite();
	const char *getName() const;
	virtual TextureAtlas *getTexture() const = 0;
	virtual int getIndexStart() const = 0;
	virtual int getIndexCount() const = 0;
	virtual TextureSprite *copy() const;
	void renderAt(float x, float y, float z) const;

  private:
	char *name;
};

#endif /* TEXTURE_SPRITE_HPP */
