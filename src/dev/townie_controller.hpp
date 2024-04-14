#ifndef DEV_TOWNIE_CONTROLLER_HPP
#define DEV_TOWNIE_CONTROLLER_HPP

#include "list.hpp"
#include "view/view.hpp"
#include "game/world_data.hpp"

class WorldData;
class GameView;
class Camera;
namespace pfa {
class PathFinder;
}

class TownieController : public View, private View::OnClickListener, private WorldData::TownieListener
{
  public:
	TownieController(GameView *parent, Camera *cam, WorldData *data, pfa::PathFinder *pfa);
	void onTownieAdded(Townie *townie) override;
	void onTownieRemoved(Townie *townie) override;
	bool onClick(float x, float y) override;
	bool onClick(View *src,float x, float y) override;

  private:
	Townie *selected;
	List<Townie*> townies;
	WorldData *world;
	Camera *cam;
	pfa::PathFinder *pfa;
	View *cancelButton;
};

#endif /* DEV_TOWNIE_CONTROLLER_HPP */