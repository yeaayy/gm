#include "timer.hpp"


Timer::Timer(float duration, OnTimeout *onTimeout)
{
    this->time = duration;
    this->onTimeout = onTimeout;
}

Timer::~Timer()
{}

void Timer::start()
{
    activateUpdate();
}

float Timer::getTimeLeft()
const {
    return time;
}

void Timer::update(float dt)
{
    if(time > dt) {
        time -= dt;
    } else {
        time = 0;
        activateUpdate(false);
        onTimeout->onTimerTimeout(this);
    }
}

Timer::OnTimeout::~OnTimeout()
{}
