
#ifndef TEXTURE_ATLAS_HPP
#define TEXTURE_ATLAS_HPP

#include "map.hpp"
#include "list.hpp"

class TextureSprite;
class Camera;

typedef Map<const char*, const TextureSprite*, Comparator<const char*>> SpriteMap;

class TextureAtlas {
	TextureAtlas(SpriteMap *dst, const char *xmlPath, float *transformation);

public:
	~TextureAtlas();

	int getImageWidth() const;
	int getImageHeight() const;
	TextureAtlas *finish();
	TextureAtlas *copySprite(const char *srcName, const char *dstName, float *transformation);
	TextureAtlas *transformSprite(const char *name, float *transformation);
	static void initRender();
	static void beginRender(Camera *cam);
	static TextureAtlas *loadXML(SpriteMap *dst, const char *path, float *transformation);

	unsigned int texture;
	unsigned int vbo;
	unsigned int ibo;

private:
	int imageWidth;
	int imageHeight;

	SpriteMap *dst;
	List<float> *vertexBuffer;
	List<unsigned short> *indexBuffer;
};

#endif /* GAME_TEXTURE_ATLAS_HPP */
