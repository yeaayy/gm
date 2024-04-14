#include "dev/storage_view.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "building/building.hpp"
#include "math/ray3.hpp"
#include "math/camera.hpp"
#include "game/storage.hpp"
#include "game/world_data.hpp"
#include "view/image_view.hpp"

namespace dev {

StorageView::StorageView(View *parent, Camera *cam, WorldData *world)
	: View(parent, VIEW_SIZE_MAX, VIEW_SIZE_MAX)
{
	this->world = world;
	this->cam = cam;

	root = new View(this, VIEW_SIZE_MAX, VIEW_SIZE_MAX);
	root->setRenderBackground(true);
	root->setBackgroundColor(0.25f, 0.25f, 0.25f, 0.75f);
	root->setMargin(100, 100, 100, 100);

	ImageView *exit = new ImageView(root);
	exit->setPrefSize(64, 64);
	exit->setImage(0, "../assets/icon/cancel.png");
	exit->setMargin(-1, 0, 0, 0);
	exit->setOnClickListener(this);

	this->txt = new TextView(root);
	txt->setTextSize(18);
	root->hide();
}

bool StorageView::onClick(View *view, float x, float y)
{
	watched->removeOnStorageChangeListener(this);
	root->hide();
	View::requestResize();
	return true;
}

void StorageView::onStorageChange(Storage *storage)
{
	char *text;
	asprintf(&text, "wood: %d / %d", storage->getAmount(ResourceType::WOOD), storage->getCapacity(ResourceType::WOOD));
	txt->setText(text);
	free(text);
}

bool StorageView::onClick(float x, float y)
{
	Ray3 *ray = cam->getWorldRay(x * 2 / width - 1, 1 - y * 2 / height);
	float t = ray->planeIntersection(0, 0, 1, 0);
	float o[3];
	ray->at(o, t);
	Building *building = dynamic_cast<Building*>(world->getObjectAt((int) o[0], (int) o[1]));
	if(building != nullptr) {
		Storage *storage = building->getStorage();
		if(storage != nullptr) {
			watched = storage;
			onStorageChange(storage);
			storage->addOnStorageChangeListener(this);
			root->show();
			View::requestResize();
			return true;
		}
	}
	if(View::onClick(x, y)) return true;
	return false;
}

} // namespace dev
