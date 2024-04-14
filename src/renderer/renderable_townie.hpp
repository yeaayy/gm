#ifndef RENDERER_RENDERABLE_TOWNIE_H
#define RENDERER_RENDERABLE_TOWNIE_H

#include "texture/atlas.hpp"
#include "game/townie.hpp"
#include "frame_counter.hpp"
#include "state.hpp"
#include "updatable.hpp"

namespace gm {

typedef enum TownieTexture {
    TOWNIE_TEXTURE_BANDIT_FIRE,
    TOWNIE_TEXTURE_BANDIT,
    TOWNIE_TEXTURE_CRAFTSMAN_FIREFIGHTER,
    TOWNIE_TEXTURE_CRAFTSMAN,
    TOWNIE_TEXTURE_FARMER_MEDICUS,
    TOWNIE_TEXTURE_FARMER,
    TOWNIE_TEXTURE_FEMALE_01,
    TOWNIE_TEXTURE_FEMALE_02,
    TOWNIE_TEXTURE_HUNTER,
    TOWNIE_TEXTURE_INTERCEPTOR,
    TOWNIE_TEXTURE_JUICER,
    TOWNIE_TEXTURE_LUMBERJACK,
    TOWNIE_TEXTURE_MILLER,
    TOWNIE_TEXTURE_MONK,
    TOWNIE_TEXTURE_SHERIFF,
    TOWNIE_TEXTURE_SOLDIER,
    TOWNIE_TEXTURE_TOWNIE,
    TOWNIE_TEXTURE_COUNT
} TownieTexture;

class RenderableTownie: public Updatable, protected State::OnStateChangedListener
{
    friend class Factory;
    class Texture
    {
    public:
        Texture(const TextureSprite **sprite, int frameCount);
        void use(FrameCounter *counter) const;
        void render(int frame, float x, float y, float z) const;

    private:
        const TextureSprite **sprite;
        int frameCount;
    };

public:
	void render() const;

    class Factory
    {
        friend class RenderableTownie;
    public:
        Factory(SpriteMap &sprite);
        const RenderableTownie *createFor(Townie*);

    private:
        TownieTexture getTextureFor(Townie*);

        const TextureSprite ***walkAnimations;
        const TextureSprite ***carryAnimations;
        const TextureSprite ***standAnimations;
        const TextureSprite ***carriedAnimations;
        const Texture ***workAnimations;
    };

protected:
	void update(float dt) override;
    void onStateChanged(State *state, const StateGroup *changed, int newValue) override;

private:
	RenderableTownie(Factory *factory, Townie *townie);
	Townie *townie;
	FrameCounter frameCounter;
	const TextureSprite **walkAnimation;
	const TextureSprite **carryAnimation;
	const TextureSprite **standAnimation;
    const Texture **workAnimation;
    int standAnimationFrameCount;
    TownieActionState actionState;
    Factory *factory;
};

} // namespace gm

#endif /* RENDERER_RENDERABLE_TOWNIE_H */
