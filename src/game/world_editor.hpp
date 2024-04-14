#ifndef GAME_WORLD_EDITOR_HPP
#define GAME_WORLD_EDITOR_HPP

#include "list.hpp"
#include "math/ivec2.hpp"
#include "view/view.hpp"

class GameView;
class WorldData;
class Camera;

class WorldEditor : public View, public View::OnToggledListener, public View::OnClickListener
{
	using View::onToggled;
  public:
	WorldEditor(GameView *parent, Camera *cam, WorldData *world);
	bool onToggled(View *src, int state) override;
	bool onClick(View *src, float x, float y) override;
	bool onScroll(float sx, float sy, float ax, float ay, float dx, float dy) override;
	// void onResize(float maxWidth, float maxHeight) override;
	void onInitGL() override;
	void onRender() override;
	bool onClick(float x, float y) override;

  private:
	bool selectionMode;
	GameView *parent;
	WorldData *world;
	Camera *cam;
	List<ivec2> selection;
	unsigned int rectVbo;

	void select(float sx, float sy, float ax, float ay);
};

#endif /* GAME_WORLD_EDITOR_HPP */