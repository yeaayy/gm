#ifndef DEV_SIMPLE_WORLD_RENDERER_HPP
#define DEV_SIMPLE_WORLD_RENDERER_HPP

#include "game/object.hpp"
#include "game/townie.hpp"
#include "game/world_data.hpp"
#include "game/world_renderer.hpp"

#include "list.hpp"

class SimpleWorldRenderer : public WorldRenderer, public WorldData::TownieListener, public WorldData::GameObjectListener
{
	class RenderableTownie;
	class RenderableGameObject;
  public:
	SimpleWorldRenderer(WorldData *world, Camera *cam);
	void onTownieAdded(Townie *townie) override;
	void onTownieRemoved(Townie *townie) override;
	void onGameObjectAdded(GameObject *object) override;
	void onGameObjectRemoved(GameObject *object) override;
	void onInitGL() override;
	void onWorldChanged() override;
	void render() override;

  private:
	unsigned int vbo, ibo;
	unsigned int rectVbo;
	unsigned int rect2Vbo;
	int count;
	List<RenderableTownie*> renderableTownies;
	List<RenderableGameObject*> renderableObjects;
};

#endif /* DEV_SIMPLE_WORLD_RENDERER_HPP */