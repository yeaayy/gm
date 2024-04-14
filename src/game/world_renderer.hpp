#ifndef GAME_WORLD_RENDERER_HPP
#define GAME_WORLD_RENDERER_HPP

class Camera;
class WorldData;

class WorldRenderer
{
  public:
	WorldRenderer(WorldData *world, Camera *cam);
	virtual void onInitGL() = 0;
	virtual void onWorldChanged() = 0;
	virtual void render() = 0;

  protected:
	WorldData *world;
	Camera *cam;
};

#endif /* GAME_WORLD_RENDERER_HPP */