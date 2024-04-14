#include "state.hpp"

#include "list.tcc"

class StateGroup
{
    friend State;
    StateGroup(int shift, int bitCount) {
        this->shift = shift;
        this->bitCount = bitCount;
        this->one = 0;
        for(int i=0; i<bitCount; i++) {
            this->one = (this->one << 1) | 1;
        }
        this->one <<= shift;
        this->zero = ~this->one;
    }

    int shift;
    int bitCount;
    int one;
    int zero;
};


const StateGroup *State::create(int shift, int bitCount)
{
    return new StateGroup(shift, bitCount);
}

const StateGroup *State::create(const StateGroup *prev, int bitCount)
{
    if(prev == nullptr) {
        return new StateGroup(0, bitCount);
    }
    return new StateGroup(prev->shift + prev->bitCount, bitCount);
}

State::State()
{
    this->state = 0;
}

State::State(int initialState)
{
    this->state = initialState;
}

int State::getState()
const {
    return this->state;
}

int State::getState(const StateGroup *group)
const {
    return (this->state & group->one) >> group->shift;
}

bool State::getStateBoolean(const StateGroup *group)
const {
    return (this->state & group->one) == group->one;
}

void State::setState(const StateGroup *group, int value)
{
    int oldValue = this->state;
    int newValue = (oldValue & group->zero) | ((value << group->shift) & group->one);
    if(oldValue != newValue) {
        this->state = newValue;
        dispatchOnStateChangedListener(group, value);
    }
}

void State::setState(const StateGroup *group, bool value)
{
    int oldValue = this->state;
    int newValue;
    if(value) {
        newValue = oldValue | group->one;
    } else {
        newValue = oldValue & group->zero;
    }
    if(oldValue != newValue) {
        this->state = newValue;
        dispatchOnStateChangedListener(group, value);
    }
}

void State::toggleState(const StateGroup *group)
{
    this->state ^= group->one;
    dispatchOnStateChangedListener(group, (this->state & group->one) >> group->shift);
}


void State::dispatchOnStateChangedListener(const StateGroup *changed, int newValue)
{
    for(int i=0; i<onStateChangedListeners.size(); i++) {
        onStateChangedListeners[i]->onStateChanged(this, changed, newValue);
    }
}

void State::addOnStateChangedListener(OnStateChangedListener *listener)
{
    onStateChangedListeners.push(listener);
}

void State::removeOnStateChangedListener(OnStateChangedListener *listener)
{
    onStateChangedListeners.removeItem(listener);
}
