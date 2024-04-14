#include "texture/simple_sprite.hpp"

SimpleTextureSprite::SimpleTextureSprite(
    const char *name,
    TextureAtlas *texture,
    int indexStart,
    int indexCount)
  : TextureSprite(name)
{
    this->indexStart = indexStart;
    this->indexCount = indexCount;
    this->texture = texture;
}

TextureSprite *SimpleTextureSprite::copy()
const {
    return new SimpleTextureSprite(getName(), texture, indexStart, indexCount);
}

TextureAtlas *SimpleTextureSprite::getTexture()
const {
    return texture;
}

int SimpleTextureSprite::getIndexStart()
const {
    return indexStart;
}

int SimpleTextureSprite::getIndexCount()
const {
    return indexCount;
}
