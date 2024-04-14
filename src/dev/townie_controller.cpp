#include "dev/townie_controller.hpp"

#include <math.h>

#include "path_finder.hpp"
#include "game/game_view.hpp"
#include "game/world_data.hpp"
#include "math/camera.hpp"
#include "math/ray3.hpp"
#include "townie/action_walk.hpp"
#include "view/image_view.hpp"

#include "dev/rand_walk.hpp"

#include "list.tcc"
#include "linked_list.tcc"

TownieController::TownieController(GameView *parent, Camera *cam, WorldData *world, pfa::PathFinder *pfa)
	: View(parent, VIEW_SIZE_MAX, VIEW_SIZE_MAX)
{
	this->selected = nullptr;
	this->parent = parent;
	this->cam = cam;
	this->world = world;
	this->pfa = pfa;
	world->addTownieListener(this);

	ImageView *cancel = new ImageView();
	cancel->setPrefSize(64, 64);
	cancel->setImage(0, "../assets/icon/cancel.png");
	cancel->setMargin(-1, -1, 0, 0);
	cancel->setOnClickListener(this);
	cancelButton = cancel;
}

void TownieController::onTownieAdded(Townie *townie)
{
	townies.push(townie);
	// townie->addActionFront(new dev::TownieActionRandomWalk(townie, world, pfa));
}

void TownieController::onTownieRemoved(Townie *townie)
{
	townies.removeItem(townie);
}

bool TownieController::onClick(View *src, float x, float y)
{
	selected = nullptr;
	// cancelButton->hide();
	return true;
}

bool TownieController::onClick(float x, float y)
{
	if (View::onClick(x, y))
		return true;
	Ray3 *ray = cam->getWorldRay(x * 2 / width - 1, 1 - y * 2 / height);
	float t = ray->planeIntersection(0, 0, 1, 0);
	float o[3];
	ray->at(o, t);
	if (selected != nullptr)
	{
		// logp("looking from %d,%d to %d,%d", (int)selected->getX(), (int)selected->getY(), (int)o[0], (int)o[1]);
		// const pfa::Point *rpath = pfa->findPath((int)o[0], (int)o[1], (int)selected->getX(), (int)selected->getY());
		// logp("path %p", rpath);
		// if (rpath == nullptr)
		// 	return false;
		// selected->actionQueue.push_front(new dev::TownieActionWalk(selected, rpath));
		selected->addActionFront(new TownieActionWalk(pfa, selected, (int) o[0], (int) o[1]));
		return onClick(cancelButton, 0, 0);
	}
	o[0] -= 0.5f;
	o[1] -= 0.5f;
	for (int i = 0; i < townies.size(); i++)
	{
		Townie *townie = townies[i];
		if (hypotf(townie->getX() - o[0], townie->getY() - o[1]) < 0.7f)
		{
			if (townie->hasAction())
				continue;
			selected = townie;
			// cancelButton->show();
			View::requestResize();
			return true;
		}
	}
	return false;
}
