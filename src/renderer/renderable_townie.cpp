#include "renderer/renderable_townie.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

#include "building/production.hpp"
#include "game/storage.hpp"
#include "texture/sprite.hpp"

namespace gm {

namespace {

const TextureSprite **makeCharMoveAnim(SpriteMap &sprite, const char *basename, const char *type, int frameCount = 4)
{
	const TextureSprite **result = new const TextureSprite*[frameCount * 8];
	for(int i=0; i<frameCount; i++) {
		for(int j=0; j<8; j++) {
			char *name;
			asprintf(&name, "%s_%s_%d.%d", basename, type, j, i);
			result[i * 8 + j] = sprite[name];
			free(name);
		}
	}
	return result;
}

const TextureSprite **makeCharSimpleAnim(SpriteMap &sprite, const char *basename, const char *type, int direction, int frameCount)
{
	const TextureSprite **result = new const TextureSprite*[frameCount];
	for(int i=0; i<frameCount; i++) {
        char *name;
        asprintf(&name, "%s_%s_%d.%d", basename, type, direction, i);
        result[i] = sprite[name];
        free(name);
	}
	return result;
}

} // namespace

RenderableTownie::Texture::Texture(const TextureSprite **_sprite, int _frameCount)
    : sprite(_sprite), frameCount(_frameCount) {}

void RenderableTownie::Texture::use(FrameCounter *counter)
const {
    counter->setFrameCount(frameCount);
}

void RenderableTownie::Texture::render(int frame, float x, float y, float z)
const {
    sprite[frame]->renderAt(x, y, z);
}

RenderableTownie::RenderableTownie(Factory *factory, Townie *townie)
	: Updatable(true), frameCounter(4, 6)
{
	this->townie = townie;
    this->factory = factory;
}

void RenderableTownie::update(float dt)
{
	frameCounter.update(dt);
}

void RenderableTownie::render()
const {
    if(!townie->isVisible())
        return;
    int direction = townie->getState(TownieStateDirection);
    switch(actionState) {
    case TOWNIE_ACTION_STAND: {
        const TextureSprite *frame = standAnimation[frameCounter.getFrameIndex() * 8 + direction];
        frame->renderAt(townie->getX() + 1, townie->getY() + 1, townie->getZ() * 0.5f);
        break;
    }
    case TOWNIE_ACTION_WALKING: {
        bool isCarrying = townie->getState(TownieStateCarrying);
        int walkFrameIndex = frameCounter.getFrameIndex() * 8 + direction;
        const TextureSprite *frame;
        if(isCarrying) {
            frame = carryAnimation[walkFrameIndex];
            const float shifts[8] = {-0.01f, -0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, -0.01f};
            float shift = shifts[direction];
            const TextureSprite *frame2 = factory->carriedAnimations[(int) townie->getCarriedType()][(frameCounter.getFrameIndex() % 2) * 8 + direction];
            frame2->renderAt(townie->getX() + 1 + shift, townie->getY() + 1 + shift, townie->getZ() * 0.5f + shift);
        } else {
            frame = walkAnimation[walkFrameIndex];
        }
        frame->renderAt(townie->getX() + 1, townie->getY() + 1, townie->getZ() * 0.5f);
        break;
    }
    case TOWNIE_ACTION_WORKING:
        workAnimation[direction]->render(frameCounter.getFrameIndex(), townie->getX() + 1, townie->getY() + 1, townie->getZ() * 0.5f);
        break;
    case TOWNIE_ACTION_IDLE:
        break;
    default:
        loge("Unknown action %d", actionState);
    }
}

void RenderableTownie::onStateChanged(State *state, const StateGroup *changed, int newValue)
{
	if(changed == TownieStateAction) {
        this->actionState = (TownieActionState) newValue;
        frameCounter.setFrameIndex(0);
        if(actionState == TOWNIE_ACTION_STAND) {
            frameCounter.setFrameCount(standAnimationFrameCount);
        } else if(actionState == TOWNIE_ACTION_WORKING) {
            int direction = townie->getState(TownieStateDirection);
            workAnimation[direction]->use(&frameCounter);
        } else {
            frameCounter.setFrameCount(4);
        }
    } else if(changed == TownieStateJob) {
        TownieTexture texture = factory->getTextureFor(townie);
        this->walkAnimation = factory->walkAnimations[texture];
        this->carryAnimation = factory->carryAnimations[texture];
        this->standAnimation = factory->standAnimations[texture];
        this->workAnimation = factory->workAnimations[texture];
        if(texture == TOWNIE_TEXTURE_BANDIT_FIRE) {
            standAnimationFrameCount = 3;
        } else {
            standAnimationFrameCount = 1;
        }
    }
}


RenderableTownie::Factory::Factory(SpriteMap &atlas)
{
	walkAnimations = new const TextureSprite **[TOWNIE_TEXTURE_COUNT];
    walkAnimations[TOWNIE_TEXTURE_BANDIT_FIRE] = makeCharMoveAnim(atlas, "bandit_fire", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_BANDIT] = makeCharMoveAnim(atlas, "bandit", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_CRAFTSMAN_FIREFIGHTER] = makeCharMoveAnim(atlas, "craftsman_firefighter", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_CRAFTSMAN] = makeCharMoveAnim(atlas, "craftsman", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_FARMER_MEDICUS] = makeCharMoveAnim(atlas, "farmer_medicus", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_FARMER] = makeCharMoveAnim(atlas, "farmer", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_FEMALE_01] = makeCharMoveAnim(atlas, "female_01", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_FEMALE_02] = makeCharMoveAnim(atlas, "female_02", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_HUNTER] = makeCharMoveAnim(atlas, "hunter", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_INTERCEPTOR] = makeCharMoveAnim(atlas, "interceptor", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_JUICER] = makeCharMoveAnim(atlas, "juicer", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_LUMBERJACK] = makeCharMoveAnim(atlas, "lumberjack", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_MILLER] = makeCharMoveAnim(atlas, "miller", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_MONK] = makeCharMoveAnim(atlas, "monk", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_SHERIFF] = makeCharMoveAnim(atlas, "sheriff", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_SOLDIER] = makeCharMoveAnim(atlas, "soldier", "walk", 4);
    walkAnimations[TOWNIE_TEXTURE_TOWNIE] = makeCharMoveAnim(atlas, "townie", "walk", 4);

	carryAnimations = new const TextureSprite **[TOWNIE_TEXTURE_COUNT];
    carryAnimations[TOWNIE_TEXTURE_BANDIT_FIRE] = nullptr;
    carryAnimations[TOWNIE_TEXTURE_BANDIT] = makeCharMoveAnim(atlas, "bandit", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_CRAFTSMAN_FIREFIGHTER] = makeCharMoveAnim(atlas, "craftsman_firefighter", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_CRAFTSMAN] = makeCharMoveAnim(atlas, "craftsman", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_FARMER_MEDICUS] = makeCharMoveAnim(atlas, "farmer_medicus", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_FARMER] = makeCharMoveAnim(atlas, "farmer", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_FEMALE_01] = makeCharMoveAnim(atlas, "female_01", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_FEMALE_02] = makeCharMoveAnim(atlas, "female_02", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_HUNTER] = makeCharMoveAnim(atlas, "hunter", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_INTERCEPTOR] = nullptr;
    carryAnimations[TOWNIE_TEXTURE_JUICER] = makeCharMoveAnim(atlas, "juicer", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_LUMBERJACK] = makeCharMoveAnim(atlas, "lumberjack", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_MILLER] = makeCharMoveAnim(atlas, "miller", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_MONK] = makeCharMoveAnim(atlas, "monk", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_SHERIFF] = makeCharMoveAnim(atlas, "sheriff", "carry", 4);
    carryAnimations[TOWNIE_TEXTURE_SOLDIER] = nullptr;
    carryAnimations[TOWNIE_TEXTURE_TOWNIE] = makeCharMoveAnim(atlas, "townie", "carry", 4);

	standAnimations = new const TextureSprite **[TOWNIE_TEXTURE_COUNT];
    standAnimations[TOWNIE_TEXTURE_BANDIT_FIRE] = makeCharMoveAnim(atlas, "bandit_fire", "stand", 3);
    standAnimations[TOWNIE_TEXTURE_BANDIT] = makeCharMoveAnim(atlas, "bandit", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_CRAFTSMAN_FIREFIGHTER] = makeCharMoveAnim(atlas, "craftsman_firefighter", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_CRAFTSMAN] = makeCharMoveAnim(atlas, "craftsman", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_FARMER_MEDICUS] = makeCharMoveAnim(atlas, "farmer_medicus", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_FARMER] = makeCharMoveAnim(atlas, "farmer", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_FEMALE_01] = makeCharMoveAnim(atlas, "female_01", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_FEMALE_02] = makeCharMoveAnim(atlas, "female_02", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_HUNTER] = makeCharMoveAnim(atlas, "hunter", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_INTERCEPTOR] = makeCharMoveAnim(atlas, "interceptor", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_JUICER] = makeCharMoveAnim(atlas, "juicer", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_LUMBERJACK] = makeCharMoveAnim(atlas, "lumberjack", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_MILLER] = makeCharMoveAnim(atlas, "miller", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_MONK] = makeCharMoveAnim(atlas, "monk", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_SHERIFF] = makeCharMoveAnim(atlas, "sheriff", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_SOLDIER] = makeCharMoveAnim(atlas, "soldier", "stand", 1);
    standAnimations[TOWNIE_TEXTURE_TOWNIE] = makeCharMoveAnim(atlas, "townie", "stand", 1);

    carriedAnimations = new const TextureSprite **[(int) ResourceType::COUNT];
    carriedAnimations[(int) ResourceType::WOOD] = makeCharMoveAnim(atlas, "wood", "carried", 2);
    carriedAnimations[(int) ResourceType::STONE] = makeCharMoveAnim(atlas, "stone", "carried", 2);
    carriedAnimations[(int) ResourceType::WATER] = makeCharMoveAnim(atlas, "water", "carried", 2);

    workAnimations = new const Texture**[TOWNIE_TEXTURE_COUNT];
    workAnimations[TOWNIE_TEXTURE_LUMBERJACK] = new const Texture*[8] {
        nullptr,
        new Texture(makeCharSimpleAnim(atlas, "lumberjack", "work", 1, 6), 6),
        nullptr,
        new Texture(makeCharSimpleAnim(atlas, "lumberjack", "work", 3, 5), 5),
        nullptr,
        new Texture(makeCharSimpleAnim(atlas, "lumberjack", "work", 5, 5), 5),
        nullptr,
        new Texture(makeCharSimpleAnim(atlas, "lumberjack", "work", 7, 6), 6),
    };
}

const RenderableTownie *RenderableTownie::Factory::createFor(Townie *townie)
{
    RenderableTownie *result = new RenderableTownie(this, townie);
    townie->addOnStateChangedListener(result);
    result->onStateChanged(townie, TownieStateJob, townie->getState(TownieStateJob));
    result->onStateChanged(townie, TownieStateAction, townie->getState(TownieStateAction));
    return result;
}

TownieTexture RenderableTownie::Factory::getTextureFor(Townie *townie)
{
    if(townie->gender == Gender::FEMALE) {
        int variant = townie->variant;
        if(variant == 0) return TOWNIE_TEXTURE_FEMALE_01;
        if(variant == 1) return TOWNIE_TEXTURE_FEMALE_02;
        loge("Invalid townie variant %d", variant);
    }
    JobDesc *job = townie->getJob();
    if(job == nullptr) return TOWNIE_TEXTURE_TOWNIE;
    switch (job->getJobSite()->getType()) {
    case BUILDING_LUMBERJACK:
        return TOWNIE_TEXTURE_LUMBERJACK;
    default:
        logp("Warning: Can not get walk animation for this townie. Please check using debugger.");
        return TOWNIE_TEXTURE_TOWNIE;
    }
}

} // namespace gm


