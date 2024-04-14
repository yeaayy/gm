#ifndef DEV_WORLD_HPP
#define DEV_WORLD_HPP

#include "path_finder.hpp"

class WorldData;

namespace dev {

class World : public pfa::World
{
  public:
	World(WorldData *data);
	float getTerrainCost(int x, int y) const override;
    int getElevation(int x, int y) const override;
	int getWidth() const override;
	int getHeight() const override;

  private:
	WorldData *data;
};

} // namespace dev

#endif /* DEV_WORLD_HPP */