#ifndef BUILDING_LUMBERJACK_HPP
#define BUILDING_LUMBERJACK_HPP

#include "building/production.hpp"

class BuildingLumberjack : public BuildingProduction
{
public:
    BuildingLumberjack(int level, int x, int y);
    ~BuildingLumberjack();
};

#endif /* BUILDING_LUMBERJACK_HPP */
