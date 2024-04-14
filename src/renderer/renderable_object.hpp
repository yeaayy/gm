#ifndef RENDERER_RENDERABLE_OBJECT_HPP
#define RENDERER_RENDERABLE_OBJECT_HPP

#include "updatable.hpp"
#include "frame_counter.hpp"
#include "game/object.hpp"
#include "texture/atlas.hpp"
#include "texture/sprite.hpp"

namespace gm {

class RenderableObject : public Updatable
{
    friend class Factory;
    class Texture;
public:
	RenderableObject(GameObject *, const Texture *);
	void render() const;

    class Factory
    {
    public:
        Factory(SpriteMap &sprite);
        const RenderableObject *createFor(GameObject*);
    public:
        const Texture **standbyAnimation;
    };

protected:
	void update(float dt) override;

private:
	GameObject *object;
	Texture *textures;
	FrameCounter frameCounter;
};

} // namespace gm

#endif /* RENDERER_RENDERABLE_OBJECT_HPP */
