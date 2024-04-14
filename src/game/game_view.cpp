#include "game/game_view.hpp"

#include <math.h>

#include "dev/townie_controller.hpp"
#include "dev/townie_action_carry.hpp"
#include "dev/simple_world_renderer.hpp"
#include "dev/storage_view.hpp"
#include "dev/world.hpp"

#include "building/castle.hpp"
#include "building/small_house.hpp"
#include "building/wood.hpp"
#include "building/lumberjack.hpp"
#include "game/storage.hpp"
#include "game/world_data.hpp"
#include "game/world_editor.hpp"
#include "game/summer_world_renderer.hpp"
#include "townie/action_start_work.hpp"
#include "math/camera.hpp"
#include "math/mat4.h"
#include "updatable.hpp"
#include "view/image_view.hpp"

#include "gl.h"
#include "log.h"
#include "list.tcc"

namespace {

float speed = 1.0f;

bool speedUp(KeyModifier mod, Keycode key) {
	speed *= 2;
	return true;
}

bool speedDown(KeyModifier mod, Keycode key) {
	speed /= 2;
	return true;
}

enum ID {
	ID_RENDER_MODE,
	ID_CAMERA_MODE,
	ID_BUILD,
};

Townie *newTownie(WorldData *w, Gender gender, int variant)
{
	int x, y;
	do {
		x = rand() % w->getWidth();
		y = rand() % w->getHeight();
	} while(w->getFlag(x, y) != TERRAIN_FLAG_FLAT);
	return new Townie(gender, variant, x, y, w->getElevation(x, y));
}

} // namespace

GameView::GameView(View *parent)
	: View(parent, VIEW_SIZE_MAX, VIEW_SIZE_MAX)
{
	cameraMode = 2;
	cam = new Camera();
	cam->frustum = false;
	cam->setNearFar(-50, 50);
	setCameraMode(0);
	scale = 5.0f;
	sharedStorage = new Storage((int) ResourceType::COUNT);

	world = new WorldData();
	if(!world->load("../assets/world/000.data")) {
		world->init(50, 50);
	}

	setOrientation(VIEW_ORIENTATION_NONE);

	new WorldEditor(this, cam, world);
	new TownieController(this, cam, world, world->getPathFinder());
	new dev::StorageView(this, cam, world);

	rendererCount = 2;
	usedRenderer = 0;
	float buttonSize = 32;

	View *tools = new View(this, VIEW_SIZE_MIN, VIEW_SIZE_MAX);
	tools->setMargin(-1, 0, 0, 0);
	tools->setPrefSize(buttonSize, buttonSize);

	ImageView *renderMode = new ImageView(tools);
	renderMode->setId(ID_RENDER_MODE);
	renderMode->setPrefSize(buttonSize, buttonSize);
	renderMode->setToggleCycle(2);
	renderMode->setImage(0, "../assets/icon/view-flat.png");
	renderMode->setImage(1, "../assets/icon/view-real.png");
	renderMode->setOnToggledListener(this);

	ImageView *cameraMode = new ImageView(tools);
	cameraMode->setId(ID_CAMERA_MODE);
	cameraMode->setPrefSize(buttonSize, buttonSize);
	cameraMode->setToggleCycle(3);
	cameraMode->setImage(0, "../assets/icon/camera-isometric.png");
	cameraMode->setImage(1, "../assets/icon/camera-top1.png");
	cameraMode->setImage(2, "../assets/icon/camera-top2.png");
	cameraMode->setOnToggledListener(this);

	ImageView *build = new ImageView(this);
	build->setId(ID_BUILD);
	build->setMargin(-1, -1, 64, 64);
	build->setPrefSize(buttonSize, buttonSize);
	build->setImage("../assets/icon/build.png");
	build->setOnClickListener(this);

	renderMode->setActive();
	cameraMode->setActive();
	Shortcut::add(MODIFIER_NONE, KEYCODE_C, cameraMode);
	Shortcut::add(MODIFIER_NONE, KEYCODE_SPACE, renderMode);
	SimpleShortcut *sup = new SimpleShortcut(speedUp);
	SimpleShortcut *sdo = new SimpleShortcut(speedDown);
	sup->setActive();
	sdo->setActive();
	Shortcut::add(MODIFIER_NONE, KEYCODE_ARROW_RIGHT, sup);
	Shortcut::add(MODIFIER_NONE, KEYCODE_ARROW_LEFT, sdo);

	renderers = new WorldRenderer*[rendererCount];
	renderers[0] = new SimpleWorldRenderer(world, cam);
	renderers[1] = new SummerWorldRenderer(world, cam);

	Castle *c = new Castle(sharedStorage, 30, 30);

	Townie *t1 = newTownie(world, Gender::FEMALE, 0);
	Townie *t2 = newTownie(world, Gender::FEMALE, 1);
	Townie *t3 = newTownie(world, Gender::MALE, 0);
	Townie *t4 = newTownie(world, Gender::MALE, 0);
	Townie *t5 = newTownie(world, Gender::MALE, 0);
	Townie *t6 = newTownie(world, Gender::MALE, 0);
	// t2->setJob(TownieJob::FARMER);
	// t3->setJob(TownieJob::CRAFTSMAN);
	// t4->setJob(TownieJob::HUNTER);
	// t5->setJob(TownieJob::FEMALE);
	SmallHouse *h1 = new SmallHouse(20, 5);
	SmallHouse *h2 = new SmallHouse(20, 9);
	Wood *w1 = new Wood(-1, 30, 30, 5);
	Wood *w2 = new Wood(-1, 30, 30, 7);
	Wood *w3 = new Wood(-1, 30, 32, 5);
	BuildingLumberjack *lj = new BuildingLumberjack(0, 25, 15);
	world->put(w1);
	world->put(w2);
	world->put(w3);
	world->put(h1);
	world->put(h2);
	world->put(c);
	world->put(lj);
	t1->setHouse(h1);
	t2->setHouse(h1);
	t3->setHouse(h1);
	t4->setHouse(h2);
	t5->setHouse(h2);
	t6->setHouse(h2);
	t3->setPosition(22, 8, 0);
	t3->setJob(lj->getJobDescs()[0]);
	t4->setJob(c->getJobDescs()[0]);
	world->put(t1);
	world->put(t2);
	world->put(t3);
	world->put(t4);
	world->put(t3);
	// t3->addActionBack(new TownieActionStartWork(t3, world));
	// t3->setState(TownieStateDirection, 1);
	// t3->setState(TownieStateAction, TOWNIE_ACTION_WORKING);
	// t3->addActionBack(new TownieActionLumberjackWork(t3, spf, world));
	// t3->addActionBack(new dev::TownieActionCarry(t1, spf, w2, c));
}

GameView::~GameView()
{
	delete cam;
	delete world;
}

void GameView::onInitGL()
{
	View::onInitGL();
	for(int i=0; i<rendererCount; i++) {
		renderers[i]->onInitGL();
	}
	regenerate();
}

void GameView::regenerate()
{
	needRegenerate = true;
}

void GameView::doRegenerate()
{
	needRegenerate = false;
	for(int i=0; i<rendererCount; i++) {
		renderers[i]->onWorldChanged();
	}
	world->save();
}

void GameView::onResize(float maxWidth, float maxHeight)
{
	View::onResize(maxWidth, maxHeight);
	float x, y;
	if(maxWidth < maxHeight) {
		x = 1;
		y = maxHeight / maxWidth;
		sensitivity = 2.0f / maxWidth;
	} else {
		x = maxWidth / maxHeight;
		y = 1;
		sensitivity = 2.0f / maxHeight;
	}
	cam->setViewport(x, -x, y, -y);

	cam->scaleViewport(scale, scale);
	sensitivity *= scale;

	clock_gettime(CLOCK_MONOTONIC_RAW, &timeLast);
}

void GameView::onRender()
{
	if(needRegenerate) {
		doRegenerate();
	}
	renderers[usedRenderer]->render();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	View::onRender();

	struct timespec timeCurrent;
	clock_gettime(CLOCK_MONOTONIC_RAW, &timeCurrent);
	int ns = timeCurrent.tv_nsec - timeLast.tv_nsec;
	int us = ns / 1000;
	int ms = us / 1000;
	float dt = (timeCurrent.tv_sec - timeLast.tv_sec) + (float) ms / 1000.0f;
	Updatable::updateAll(dt * speed);
	timeLast = timeCurrent;
}

bool GameView::onScroll(float sx, float sy, float ax, float ay, float dx, float dy)
{
	if(View::onScroll(sx, sy, ax, ay, dx, dy)) return true;
	float y=cam->viewMatrix[9], z = cam->viewMatrix[10];
	cam->translateViewRelative(dx * sensitivity, -dy * sensitivity, dy * y / z * sensitivity);
	return true;
}

bool GameView::onScale(float x, float y, float scale)
{
	this->scale *= scale;
	cam->scaleViewport(scale, scale);
	sensitivity *= scale;
	return true;
}

bool GameView::onClick(float x, float y)
{
	if(View::onClick(x, y)) return true;

	return false;
}

bool GameView::onToggled(View *src, int state)
{
	switch(src->getId()) {
	case ID_RENDER_MODE:
		usedRenderer = state;
		break;
	case ID_CAMERA_MODE:
		setCameraMode(state);
		break;
	}
	return true;
}

bool GameView::onClick(View *src, float x, float y)
{
	switch(src->getId()) {
	case ID_BUILD:
		sharedStorage->addAmount(ResourceType::WOOD, -20);
		return true;
	}
	return false;
}

void GameView::setCameraMode(int mode)
{
	switch(cameraMode) {
	case 0:
		cam->rotateViewRelative(1,0,0, acosf(sqrtf(1.0f / 3.0f)));
		cam->rotateViewRelative(0,0,1, M_PI * 3 / 4);
		break;
	case 1:
		cam->rotateViewRelative(0,0,1, M_PI * 3 / 4);
		break;
	case 2:
		break;
	}
	cameraMode = mode;
	switch(mode) {
	case 0:
		cam->rotateViewRelative(0,0,1, -M_PI * 3 / 4);
		cam->rotateViewRelative(1,0,0, -acosf(sqrtf(1.0f / 3.0f)));
		break;
	case 1:
		cam->rotateViewRelative(0,0,1, -M_PI * 3 / 4);
		break;
	case 2:
		break;
	}
	cam->updateViewProjectionMatrix();
}
