#include "building/lumberjack.hpp"

#include <math.h>

#include "game/storage.hpp"
#include "townie/lumberjack_work.hpp"

BuildingLumberjack::BuildingLumberjack(int level, int x, int y)
    : BuildingProduction(BUILDING_LUMBERJACK, x, y, 3, 3)
{
    addEntrance(3, 2, Dir::up_right);
    storage = new Storage(1);
    storage->setCapacity(ResourceType::WOOD, 40);
    slotCount = new int[3] {1, 2, 3};
    JobDesc *jobLumberjack = new TownieActionLumberjackWork(this);
    JobDesc **pointer = new JobDesc*[2] {
        jobLumberjack,
        nullptr
    };
    jobDesc = new JobDesc**[3] {
        pointer,
        pointer,
        pointer,
    };
}

BuildingLumberjack::~BuildingLumberjack()
{
    delete storage;
    delete jobDesc[0][0];
    delete[] jobDesc[0];
}
