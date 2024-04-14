#include "renderer/renderable_object.hpp"

#include <stdarg.h>

#include "building/building.hpp"
#include "building/wood.hpp"
#include "log.h"

namespace gm {

enum TextureName {
    TEXTURE_CASTLE,
    TEXTURE_WOOD,
    TEXTURE_SMALL_HOUSE_1,
    TEXTURE_LUMBERJACK,
    TEXTURE_COUNT
};

class RenderableObject::Texture
{
    friend class RenderableObject;
public:
    Texture(SpriteMap &sprite, float *shifts, int frameCount, int compoundSize, const char *textureName, ...);
    Texture *copy(FrameCounter *) const;
    void render(float x, float y, float z) const;

private:
    Texture();
    const TextureSprite**textures;
    FrameCounter *frameCounter;
    float *shifts;
    int frameCount;
    int compoundSize;
};

RenderableObject::Texture::Texture()
{}

RenderableObject::Texture::Texture(SpriteMap &sprite, float *_shifts, int _frameCount, int _compoundSize, const char *textureNames, ...)
    : shifts(_shifts), frameCount(_frameCount), compoundSize(_compoundSize)
{
    int count = frameCount * compoundSize;
    textures = new const TextureSprite*[count];
    va_list args;
    va_start(args, textureNames);
    const char *textureName = textureNames;
    for(int i = 0; i < count; i++) {
        textures[i] = sprite[textureName];
        textureName = va_arg(args, const char*);
    }
    va_end(args);
}

RenderableObject::Texture *RenderableObject::Texture::copy(FrameCounter *frameCounter) const {
    Texture *result = new Texture();
    result->frameCounter = frameCounter;
    result->frameCount = frameCount;
    result->compoundSize = compoundSize;
    result->shifts = shifts;
    int count = frameCount * compoundSize;
    result->textures = new const TextureSprite*[count];
    for(int i = 0; i < count; i++) {
        result->textures[i] = textures[i];
    }
    return result;
}

void RenderableObject::Texture::render(float x, float y, float z)
const {
    int frame = (frameCounter->getFrameIndex() % frameCount) * compoundSize;
    float *shift = shifts;
    for(int i = 0; i < compoundSize; i++, frame++, shift+=3) {
        textures[frame]->renderAt(
            x + shift[0],
            y + shift[1],
            (z + shift[2]) * 0.5f
        );
    }
}

RenderableObject::RenderableObject(GameObject *_object, const Texture *textures)
    : Updatable(true), object(_object), frameCounter(1, 5)
{
    this->textures = textures->copy(&frameCounter);
    frameCounter.setFrameCount(textures->frameCount);
}

void RenderableObject::render()
const {
    textures->render(object->getX() + object->getWidth(), object->getY() + object->getHeight(), 0);
}

void RenderableObject::update(float dt)
{
    frameCounter.update(dt);
}

RenderableObject::Factory::Factory(SpriteMap &sprite)
{
    standbyAnimation = new const Texture*[TEXTURE_COUNT];
    standbyAnimation[TEXTURE_CASTLE] = new Texture(
        sprite, new float[21] {
            2, 0, 0,
            -0.01f-2, -0.01f-2, -4,
            0, 0, 0,
            0, 0, 0,
            0, 0, 0,
            0, 0, 0,
            0, 0, 0,
        }, 4, 7,
        "building_hq_01.0", "building_hq_00.0", "building_hq_02.0", "ani_hq_00.0", "ani_hq_01.0", "ani_hq_02.0", "ani_hq_03.0",
        "building_hq_01.0", "building_hq_00.0", "building_hq_02.0", "ani_hq_00.1", "ani_hq_01.1", "ani_hq_02.1", "ani_hq_03.1",
        "building_hq_01.0", "building_hq_00.0", "building_hq_02.0", "ani_hq_00.2", "ani_hq_01.2", "ani_hq_02.2", "ani_hq_03.2",
        "building_hq_01.0", "building_hq_00.0", "building_hq_02.0", "ani_hq_00.3", "ani_hq_01.3", "ani_hq_02.3", "ani_hq_03.3"
    );
    standbyAnimation[TEXTURE_SMALL_HOUSE_1] = new Texture(
        sprite, new float[3] {-1, -1, -2}, 1, 1,
        "building_residence_00_0.0"
    );
    standbyAnimation[TEXTURE_LUMBERJACK] = new Texture(
        sprite, new float[3] {-1, -1, -2}, 1, 1,
        "building_lumberjack_00.0"
    );
    standbyAnimation[TEXTURE_WOOD] = new Texture(
        sprite, new float[3] {-1, -1, -2}, 5, 1,
        "forest_2x2_23.0",
        "forest_2x2_23.1",
        "forest_2x2_23.2",
        "forest_2x2_23.3",
        "forest_2x2_23.4"
    );
}

const RenderableObject *RenderableObject::Factory::createFor(GameObject *object)
{
    switch(object->getType()) {
    case BUILDING_CASTLE:           return new RenderableObject(object, standbyAnimation[TEXTURE_CASTLE]);
    case BUILDING_HOUSE_SMALL_1:    return new RenderableObject(object, standbyAnimation[TEXTURE_SMALL_HOUSE_1]);
    case BUILDING_LUMBERJACK:       return new RenderableObject(object, standbyAnimation[TEXTURE_LUMBERJACK]);
    case OBJECT_WOOD:               return new RenderableObject(object, standbyAnimation[TEXTURE_WOOD]);
    default:
        logp("Can't get RenderableObject from GameObject. Please check using debugger.");
        return nullptr;
    }
}

} // namespace gm