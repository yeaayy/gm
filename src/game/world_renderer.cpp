#include "game/world_renderer.hpp"

WorldRenderer::WorldRenderer(WorldData *world, Camera *cam)
{
	this->world = world;
	this->cam = cam;
}
