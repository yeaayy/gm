#include "texture/sprite.hpp"

#include <string.h>
#include <stdlib.h>

#include "log.h"

TextureSprite::TextureSprite(const char *name)
{
	this->name = strdup(name);
}

TextureSprite::~TextureSprite()
{
	free(name);
}

const char *TextureSprite::getName()
const {
    return name;
}

TextureSprite *TextureSprite::copy()
const {
	loge("TODO: implement this method");
	return nullptr;
}
