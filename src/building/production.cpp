#include "building/production.hpp"


JobDesc::JobDesc(BuildingProduction *jobSite, int typeId)
{
    this->jobSite = jobSite;
    this->typeId = typeId;
}

JobDesc::~JobDesc()
{}

int JobDesc::getTypeId()
const {
    return typeId;
}

BuildingProduction *JobDesc::getJobSite()
{
    return jobSite;
}

BuildingProduction::BuildingProduction(int type, int x, int y, int width, int height)
    : Building(type, x, y, width, height)
{
    slotCount = nullptr;
    jobDesc = nullptr;
}

BuildingProduction::~BuildingProduction()
{
    delete[] slotCount;
    delete[] jobDesc;
}

int BuildingProduction::getSlotCount()
const {
    return slotCount[level];
}

JobDesc **BuildingProduction::getJobDescs()
{
    return jobDesc[level];
}
