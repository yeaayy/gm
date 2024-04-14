#include "frame_counter.hpp"

FrameCounter::FrameCounter(int frameCount, float fps)
{
    this->time = 0;
    this->index = 0;
    this->fps = fps;
    this->count = frameCount;
}

void FrameCounter::reset(int frame)
{
    this->time = 0;
    this->index = frame;
}

void FrameCounter::reset(int frameCount, float fps, int frame)
{
    this->time = 0;
    this->index = frame;
    this->fps = fps;
    this->count = frameCount;
}

void FrameCounter::setFrameIndex(int frameIndex)
{
    this->index = frameIndex;
}

void FrameCounter::setFrameCount(int frameCount)
{
    this->count = frameCount;
}

void FrameCounter::setFPS(float fps)
{
    this->fps = fps;
}

int FrameCounter::getFrameIndex()
const {
    return this->index;
}

int FrameCounter::getFrameCount()
const {
    return this->count;
}

float FrameCounter::getFPS()
const {
    return this->fps;
}

void FrameCounter::update(float dt)
{
    time += dt;
    while(time >= 1 / fps) {
        index = (index + 1) % count;
        time -= 1 / fps;
    }
}

