#ifndef UTILS_TIMER_H
#define UTILS_TIMER_H

#include "updatable.hpp"

class Timer : private Updatable
{
public:
    class OnTimeout
    {
    public:
        virtual void onTimerTimeout(Timer *timer) = 0;
        virtual ~OnTimeout();
    };

    Timer(float duration, OnTimeout *);
    virtual ~Timer();
    void start();
    float getTimeLeft() const;

private:
    void update(float dt);
    float time;
    OnTimeout *onTimeout;
};

#endif /* UTILS_TIMER_H */