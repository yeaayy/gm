#ifndef GAME_SUMMER_WORLD_RENDERER_HPP
#define GAME_SUMMER_WORLD_RENDERER_HPP

#include "game/townie.hpp"
#include "game/world_renderer.hpp"
#include "game/world_data.hpp"

class SummerWorldRenderer : public WorldRenderer, public WorldData::TownieListener, public WorldData::GameObjectListener
{
  public:
	SummerWorldRenderer(WorldData *world, Camera *cam);
	void onInitGL() override;
	void onWorldChanged() override;
	void render() override;
	void onTownieAdded(Townie *townie) override;
	void onTownieRemoved(Townie *townie) override;
	void onGameObjectAdded(GameObject *object) override;
	void onGameObjectRemoved(GameObject *object) override;
};

#endif /* GAME_SUMMER_WORLD_RENDERER_HPP */
