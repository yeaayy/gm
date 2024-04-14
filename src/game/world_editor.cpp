#include "game/world_editor.hpp"

#include "math/camera.hpp"
#include "view/image_view.hpp"
#include "shader_program.hpp"
#include "game/game_view.hpp"
#include "game/world_data.hpp"

#include "list.tcc"

namespace {
ShaderProgram program("shader/game");

enum ID {
	ID_VIEW_MODE,
	ID_ACTION_MODE,
	ID_DISCARD_SELECTION,
	ID_SET_GROUND,
	ID_SET_WATER,
	ID_ELEVATE,
	ID_DELEVATE,
};
} // namespace

WorldEditor::WorldEditor(GameView *parent, Camera *cam, WorldData *world)
	: View(parent, VIEW_SIZE_MAX, VIEW_SIZE_MAX)
{
	this->parent = parent;
	this->cam = cam;
	this->world = world;
	this->selectionMode = false;
}

void WorldEditor::onInitGL()
{
	program.loadStringIfNeeded(
#include "shader/game.vertex.glsl"
	,
#include "shader/game.fragment.glsl"
	);

	float r=1, g=1, b=1;
	float vertex[28] {
		0.4f, 0.4f, 0, r,g,b,1,
		0.6f, 0.4f, 0, r,g,b,1,
		0.6f, 0.6f, 0, r,g,b,1,
		0.4f, 0.6f, 0, r,g,b,1,
	};
	glGenBuffers(1, &rectVbo);
	glBindBuffer(GL_ARRAY_BUFFER, rectVbo);
	glBufferData(GL_ARRAY_BUFFER, 7 * 4 * sizeof(float), (GLvoid*) vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float buttonSize = 64;

	ImageView *actionMode = new ImageView(this);
	actionMode->setPrefSize(buttonSize, buttonSize);
	actionMode->setToggleCycle(2);
	actionMode->setImage(0, "../assets/icon/move.png");
	actionMode->setImage(1, "../assets/icon/edit.png");
	actionMode->setId(ID_ACTION_MODE);
	actionMode->setOnToggledListener(this);

	ImageView *elevate = new ImageView(this);
	elevate->setPrefSize(buttonSize, buttonSize);
	elevate->setImage(0, "../assets/icon/set-elevate.png");
	elevate->setId(ID_ELEVATE);
	elevate->setOnClickListener(this);

	ImageView *delevate = new ImageView(this);
	delevate->setPrefSize(buttonSize, buttonSize);
	delevate->setImage(0, "../assets/icon/set-delevate.png");
	delevate->setId(ID_DELEVATE);
	delevate->setOnClickListener(this);

	ImageView *setGround = new ImageView(this);
	setGround->setPrefSize(buttonSize, buttonSize);
	setGround->setImage(0, "../assets/icon/set-ground.png");
	setGround->setId(ID_SET_GROUND);
	setGround->setOnClickListener(this);

	ImageView *setWater = new ImageView(this);
	setWater->setPrefSize(buttonSize, buttonSize);
	setWater->setImage(0, "../assets/icon/set-water.png");
	setWater->setId(ID_SET_WATER);
	setWater->setOnClickListener(this);

	ImageView *discardSel = new ImageView(this);
	discardSel->setPrefSize(buttonSize, buttonSize);
	discardSel->setImage(0, "../assets/icon/cancel.png");
	discardSel->setId(ID_DISCARD_SELECTION);
	discardSel->setMargin(0, 0, 0, 0);
	discardSel->setOnClickListener(this);

	Shortcut::add(MODIFIER_NONE, KEYCODE_ESCAPE, discardSel);
	Shortcut::add(MODIFIER_NONE, KEYCODE_W, setWater);
	Shortcut::add(MODIFIER_NONE, KEYCODE_G, setGround);
	Shortcut::add(MODIFIER_NONE, KEYCODE_ARROW_UP, elevate);
	Shortcut::add(MODIFIER_NONE, KEYCODE_ARROW_DOWN, delevate);
	Shortcut::add(MODIFIER_NONE, KEYCODE_TAB, actionMode);
	discardSel->setActive(true);
	setWater->setActive(true);
	setGround->setActive(true);
	elevate->setActive(true);
	delevate->setActive(true);
	actionMode->setActive(true);
}

void WorldEditor::onRender()
{
	if(selection.size() == 0) {
		View::onRender();
		return;
	}
	program.use();
	glUniformMatrix4fv(program.unifomMatrix1Index, 1, false, cam->viewProjectionMatrix);
	glBindBuffer(GL_ARRAY_BUFFER, rectVbo);
	glUniform4f(program.uniformColorIndex, 0.9f, 0.2f, 0.1f, 1);
	glVertexAttribPointer(program.vertexPositionIndex, 3, GL_FLOAT, GL_FALSE, 28, (GLvoid*) 0);
	glVertexAttribPointer(program.vertexColorIndex, 4, GL_FLOAT, GL_FALSE, 28, (GLvoid*) 12);
	for(int i=0; i<selection.size(); i++) {
		ivec2 p = selection[i];
		glUniform4f(program.uniformPositionIndex, p.x - 0.5f, p.y - 0.5f, 0.0f, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	View::onRender();
}

bool WorldEditor::onToggled(View *src, int state)
{
	switch(src->getId()) {
	case ID_ACTION_MODE:
		selectionMode = state == 1;
		break;
	}
	return true;
}

bool WorldEditor::onClick(View *src, float x, float y)
{
	switch(src->getId()) {
	case ID_SET_GROUND:
	case ID_SET_WATER:
	case ID_ELEVATE:
	case ID_DELEVATE:
		for(int i=0; i<selection.size(); i++) {
			ivec2 p = selection[i];
			switch(src->getId()) {
			case ID_SET_GROUND:
				world->setTerrain(p.x, p.y, TERRAIN_TYPE_GROUND);
				break;
			case ID_SET_WATER:
				world->setTerrain(p.x, p.y, TERRAIN_TYPE_WATER);
				break;
			case ID_ELEVATE:
				world->addElevation(p.x, p.y, 1);
				break;
			case ID_DELEVATE:
				world->addElevation(p.x, p.y, -1);
				break;
			}
		}
		if(selection.size() > 0)
			parent->regenerate();
		break;
	case ID_DISCARD_SELECTION:
		selection.clear();
		break;
	default:
		return false;
	}
	return true;
}

float selSX = 0, selSY = 0;
bool removing;

void WorldEditor::select(float sx, float sy, float ax, float ay)
{
	Ray3 *ray = cam->getWorldRay(ax * 2 / width - 1, 1 - ay * 2 / height);
	float t = ray->planeIntersection(0, 0, 1, 0);
	float o[3];
	ray->at(o, t);
	o[0] += 0.5f;
	o[1] += 0.5f;
	int ix = (int) o[0];
	int iy = (int) o[1];
	if(o[0] < 0) ix--;
	if(o[1] < 0) iy--;
	if(ix < 0 || ix >= world->getTerrainWidth()) return;
	if(iy < 0 || iy >= world->getTerrainHeight()) return;
	ivec2 pointer = {ix, iy};
	if(selSX != sx || selSY != sy) {
		selSX = sx;
		selSY = sy;
		if(selection.removeItem(pointer)) {
			removing = true;
		} else {
			removing = false;
		adding:
			for(int i = 0; i < selection.size(); i++) {
				if(selection[i] == pointer) {
					return;
				}
			}
			selection.push(pointer);
		}
		return;
	}
	if(removing) {
		selection.removeItem(pointer);
	} else {
		goto adding;
	}
}

bool WorldEditor::onScroll(float sx, float sy, float ax, float ay, float dx, float dy)
{
	if(selectionMode) {
		select(sx, sy, ax, ay);
		return true;
	}
	return false;
}

bool WorldEditor::onClick(float x, float y)
{
	if(View::onClick(x, y)) return true;
	select(x, y, x, y);
	return true;
}
