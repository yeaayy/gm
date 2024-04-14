#include "game/summer_world_renderer.hpp"

#include <stdlib.h>
#include <math.h>

#include "gl.h"
#include "math/mat4.h"

#include "math/camera.hpp"
#include "frame_counter.hpp"
#include "game/world_data.hpp"
#include "renderer/renderable_object.hpp"
#include "renderer/renderable_townie.hpp"
#include "texture/animated_sprite.hpp"
#include "texture/atlas.hpp"
#include "texture/renderable_sprite.hpp"

#include "list.tcc"

namespace {
SpriteMap atlas;
List<RenderableSprite*> sprites;
List<const gm::RenderableTownie*> townies;
List<const gm::RenderableObject*> objects;
gm::RenderableTownie::Factory *townieRendererFactory;
gm::RenderableObject::Factory *objectRendererFactory;
RenderableSprite **testRender;

#include "game/rule.tcc"

} // namespace

SummerWorldRenderer::SummerWorldRenderer(WorldData *world, Camera *cam)
	: WorldRenderer(world, cam)
{}

void createAnim(const char *name, int frameCount, float fps)
{
	const TextureSprite *frames[frameCount];
	int digit = 1;
	if(frameCount >= 10) {
		digit = 2;
	}
	char *format;
	asprintf(&format, "%s.%%0%dd", name, digit);
	for(int i=0; i<frameCount; i++) {
		char *frameName;
		asprintf(&frameName, format, i);
		frames[i] = atlas[frameName];
		free(frameName);
	}
	free(format);
	AnimatedSprite *anim = new AnimatedSprite(name, frameCount, frames, fps);
	atlas.add(anim->getName(), anim);
}

void createCharacterSingleMirror(TextureAtlas *atlas, const char *basename, const char *type, int direction, int frameCount, float *mirrorMatrix)
{
	for(int j=0; j<frameCount; j++) {
		char *src;
		char *dst;
		asprintf(&src, "%s_%s_%d.%d", basename, type, direction, j);
		asprintf(&dst, "%s_%s_%d.%d", basename, type, 8 - direction, j);
		atlas->copySprite(src, dst, mirrorMatrix);
		free(src);
		free(dst);
	}
}

void createCharacterMirror(TextureAtlas *atlas, const char *basename, const char *type, int frameCount, float *mirrorMatrix)
{
	for(int i=1; i<4; i++) {
		createCharacterSingleMirror(atlas, basename, type, i, frameCount, mirrorMatrix);
	}
}

const TextureSprite **createCharacterWalkAnimation(const char *basename, const char *type)
{
	const TextureSprite **result = new const TextureSprite*[32];
	for(int i=0; i<4; i++) {
		for(int j=0; j<8; j++) {
			char *name;
			asprintf(&name, "%s_%s_%d.%d", basename, type, j, i);
			result[i * 8 + j] = atlas[name];
			free(name);
		}
	}
	return result;
}

void SummerWorldRenderer::onInitGL()
{
	float transformation1[16];
	float transformation2[16];
	float flatten[16];
	float mirror1[16];
	// float mirror2[16];
	mat4_setIdentity(transformation1);
	mat4_setIdentity(transformation2);

	mat4_translateA(transformation1, transformation1, 0, -M_SQRT1_2, 0);
	mat4_scaleA(transformation1, transformation1, 1, 2, 1);
	mat4_rotateA(transformation1, transformation1, 0,0,1, M_PI * 3 / 4);

	mat4_translateA(transformation2, transformation2, 0, -M_SQRT1_2, 0);
	mat4_scaleA(transformation2, transformation2, 1, M_SQRT2, 1);
	mat4_rotateA(transformation2, transformation2, 1,0,0, M_PI_2);
	mat4_rotateA(transformation2, transformation2, 0,0,1, M_PI * 3 / 4);

	mat4_invert(mirror1, transformation2);
	mat4_rotateA(mirror1, mirror1, 0, 1, 0, M_PI);
	mat4_mulmm(mirror1, transformation2, mirror1);

	mat4_invert(flatten, transformation2);
	mat4_mulmm(flatten, transformation1, flatten);

	TextureAtlas::initRender();
	TextureAtlas::loadXML(&atlas, "../assets/tilesets/tiles_summer_1.xml", transformation1)->finish();
	TextureAtlas *buildingAtlas1 = TextureAtlas::loadXML(&atlas, "../assets/buildings/buildings_01.xml", transformation2);
	buildingAtlas1->transformSprite("building_hq_01.0", flatten);
	buildingAtlas1->finish();
	TextureAtlas::loadXML(&atlas, "../assets/buildings/buildings_02.xml", transformation2)->finish();
	TextureAtlas::loadXML(&atlas, "../assets/buildings/buildings_03.xml", transformation2)->finish();
	TextureAtlas::loadXML(&atlas, "../assets/objects/objects_summer.xml", transformation2)->finish();
	TextureAtlas *charsAtlas = TextureAtlas::loadXML(&atlas, "../assets/chars/townies.xml", transformation2);
	const char *walkChars[18] = {
		"bandit_fire",
		"bandit",
		"craftsman_firefighter",
		"craftsman",
		"farmer_medicus",
		"farmer",
		"female_01",
		"female_02",
		"hunter",
		"interceptor",
		"juicer",
		"lumberjack",
		"miller",
		"monk",
		"sheriff",
		"soldier",
		"townie",
		nullptr
	};
	const char *carryChars[15] = {
		"bandit",
		"craftsman_firefighter",
		"craftsman",
		"farmer_medicus",
		"farmer",
		"female_01",
		"female_02",
		"hunter",
		"juicer",
		"lumberjack",
		"miller",
		"monk",
		"sheriff",
		"townie",
		nullptr
	};
	const char *standChars[17] = {
		"bandit",
		"craftsman_firefighter",
		"craftsman",
		"farmer_medicus",
		"farmer",
		"female_01",
		"female_02",
		"hunter",
		"interceptor",
		"juicer",
		"lumberjack",
		"miller",
		"monk",
		"sheriff",
		"soldier",
		"townie",
		nullptr
	};
	const char *pickupChars[14] = {
		"craftsman_firefighter",
		"craftsman",
		"farmer_medicus",
		"farmer",
		"female_01",
		"female_02",
		"hunter",
		"juicer",
		"lumberjack",
		"miller",
		"monk",
		"sheriff",
		"townie",
		nullptr
	};
	const char *carriedObjects[34] = {
		"armor",
		"basket",
		"bread",
		"brew",
		"cake",
		"cloths",
		"coal",
		"coins",
		"corn",
		"fabric",
		"fish",
		"flour",
		"gold",
		"grapes",
		"herbs",
		"hops",
		"iron",
		"jewelry",
		"juice",
		"leather",
		"meat",
		"nuggets",
		"ore",
		"pig",
		"planks",
		"sausage",
		"skins",
		"stone",
		"tools",
		"water",
		"weapons",
		"wood",
		"wool",
		nullptr
	};
	for(const char **name = walkChars; *name; name++) {
		createCharacterMirror(charsAtlas, *name, "walk", 4, mirror1);
	}
	for(const char **name = carryChars; *name; name++) {
		createCharacterMirror(charsAtlas, *name, "carry", 4, mirror1);
	}
	for(const char **name = standChars; *name; name++) {
		createCharacterMirror(charsAtlas, *name, "stand", 1, mirror1);
	}
	createCharacterMirror(charsAtlas, "bandit_fire", "stand", 3, mirror1);
	for(const char **name = carriedObjects; *name; name++) {
		createCharacterMirror(charsAtlas, *name, "carried", 2, mirror1);
	}
	for(const char **name = carriedObjects; *name; name++) {
		createCharacterMirror(charsAtlas, *name, "carried", 2, mirror1);
	}
	createCharacterSingleMirror(charsAtlas, "lumberjack", "work", 1, 6, mirror1);
	createCharacterSingleMirror(charsAtlas, "lumberjack", "work", 3, 5, mirror1);
	charsAtlas->finish();
	createAnim("ani_hq_00", 4, 10);
	createAnim("ani_hq_01", 4, 10);
	createAnim("ani_hq_02", 4, 10);
	createAnim("ani_hq_03", 4, 10);
	createAnim("tile_2x2_w_0_1", 8, 10);
	createAnim("tile_2x2_ggggwwgww_0_1", 8, 10);
	createAnim("tile_2x2_gggwwgwwg_0_1", 8, 10);
	createAnim("tile_2x2_gwwgwwggg_0_1", 8, 10);
	createAnim("tile_2x2_wwgwwgggg_0_1", 8, 10);
	createAnim("tile_2x2_gggwwwwww_0_1_00", 8, 10);
	createAnim("tile_2x2_wwwwwwggg_0_1_00", 8, 10);
	createAnim("tile_2x2_gwwgwwgww_0_1_00", 8, 10);
	createAnim("tile_2x2_wwgwwgwwg_0_1_00", 8, 10);
	createAnim("tile_2x2_gwwwwwwww_0_1", 8, 10);
	createAnim("tile_2x2_wwwwwwwwg_0_1", 8, 10);
	createAnim("tile_2x2_wwgwwwwww_0_1", 8, 10);
	createAnim("tile_2x2_wwwwwwgww_0_1", 8, 10);
	createAnim("water_carried_0", 2, 6);
	createAnim("water_carried_1", 2, 6);
	createAnim("water_carried_2", 2, 6);
	createAnim("water_carried_3", 2, 6);
	createAnim("water_carried_4", 2, 6);
	createAnim("forest_1x1_00", 5, 5);
	createAnim("forest_1x1_10", 5, 5);
	createAnim("forest_1x1_13", 5, 5);
	createAnim("forest_1x1_20", 5, 5);
	createAnim("forest_1x1_23", 5, 5);
	createAnim("forest_1x1_30", 5, 5);
	createAnim("forest_1x1_33", 5, 5);

	testRender = new RenderableSprite*[2] {
		// new RenderableSprite(atlas["tile_2x2_g_0_000022021"]->copy(), 0, 0, 0),
		new RenderableSprite(atlas["deco_2x2_statue"], 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_000222111_00"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_000222111_01"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_021021021_00"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_021021021_01"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_021022000"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_021221111"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_111122120"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_111221021"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_111222000_00"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_111222000_01"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_120120120_00"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_120120120_01"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_120122111"]->copy(), 0, 0, 0),
		// new RenderableSprite(atlas["tile_2x2_g_0_120220000"]->copy(), 0, 0, 0),
		nullptr
	};

	townieRendererFactory = new gm::RenderableTownie::Factory(atlas);
	objectRendererFactory = new gm::RenderableObject::Factory(atlas);
	initPattern();
	world->addTownieListener(this);
	world->addGameObjectListener(this);
}

void SummerWorldRenderer::onWorldChanged()
{
	for(int i=0; i<sprites.size(); i++) {
		delete sprites[i]->data;
		delete sprites[i];
	}
	sprites.clear();
	int width = world->getTerrainWidth();
	int height = world->getTerrainHeight();
	for(int y=0; y<height; y+=2) {
		for(int x=0; x<width; x+=2) {
			getMatchingRule(world, x, y);
		}
	}
}

void SummerWorldRenderer::onTownieAdded(Townie *townie)
{
	townies.push(townieRendererFactory->createFor(townie));
}

void SummerWorldRenderer::onTownieRemoved(Townie *townie)
{}

void SummerWorldRenderer::onGameObjectAdded(GameObject *object)
{
	const gm::RenderableObject *result = objectRendererFactory->createFor(object);
	if(result != nullptr) {
		objects.push(result);
	}
}

void SummerWorldRenderer::onGameObjectRemoved(GameObject *object)
{}

void SummerWorldRenderer::render()
{
	TextureAtlas::beginRender(cam);
	for(int i=0; i<sprites.size(); i++) {
		sprites[i]->render();
	}
	glDepthMask(GL_TRUE);
	for(int i=0; i<townies.size(); i++) {
		townies[i]->render();
	}
	for(int i=0; i<objects.size(); i++) {
		objects[i]->render();
	}
	for(RenderableSprite **rs = testRender; *rs; rs++) {
		(*rs)->render();
	}
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
}
