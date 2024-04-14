#ifndef GAME_VIEW_HPP
#define GAME_VIEW_HPP

#include <time.h>

#include "view/view.hpp"
#include "game/townie.hpp"
#include "game/world_renderer.hpp"

class Camera;
class Storage;
class WorldData;

class GameView : public View, public View::OnToggledListener, public View::OnClickListener
{
	using View::onToggled;
  public:
	GameView(View *parent = nullptr);
	~GameView();
	bool onToggled(View *src, int state) override;
	bool onScroll(float sx, float sy, float ax, float ay, float dx, float dy) override;
	bool onScale(float x, float y, float scale) override;
	void onResize(float maxWidth, float maxHeight) override;
	void onInitGL() override;
	void onRender() override;
	bool onClick(float x, float y) override;
	bool onClick(View *src, float x, float y) override;
	void regenerate();
	void setCameraMode(int);

  private:
	Camera *cam;
	float scale;
	float sensitivity;
	WorldData *world;
	bool needRegenerate;
	WorldRenderer **renderers;
	int rendererCount;
	int usedRenderer;
	int cameraMode;

	struct timespec timeLast;
	Storage *sharedStorage;

	void doRegenerate();
};

#endif /* GAME_VIEW_HPP */