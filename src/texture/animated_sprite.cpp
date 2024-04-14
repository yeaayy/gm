#include "texture/animated_sprite.hpp"

#include <stdio.h>

AnimatedSprite::AnimatedSprite(const char *name, int frameCount, const TextureSprite *const*frames, float fps)
  : TextureSprite(name), Updatable(false)
{
    this->frameCount = frameCount;
    this->frameIndex = 0;
    this->time = 0;
    this->fps = fps;
    this->frames = new const TextureSprite*[frameCount];
    for(int i=0; i<frameCount; i++) {
        this->frames[i] = frames[i];
    }
}

AnimatedSprite::~AnimatedSprite()
{
    if(isUpdateActivated()) {
        activateUpdate(false);
    }
    delete frames;
}

void AnimatedSprite::update(float dt)
{
    time += dt;
    while(time >= 1 / fps) {
        frameIndex = (frameIndex + 1) % frameCount;
        time -= 1 / fps;
    }
}

TextureSprite *AnimatedSprite::copy() const
{
    AnimatedSprite *result = new AnimatedSprite(getName(), frameCount, frames, fps);
    result->activateUpdate();
    return result;
}

TextureAtlas *AnimatedSprite::getTexture()
const {
    return frames[frameIndex]->getTexture();
}

int AnimatedSprite::getIndexStart()
const {
    return frames[frameIndex]->getIndexStart();
}

int AnimatedSprite::getIndexCount()
const {
    return frames[frameIndex]->getIndexCount();
}