#ifndef UTILS_STATE_HPP
#define UTILS_STATE_HPP

#include "list.hpp"

class StateGroup;

class State
{
public:
    static const StateGroup *create(int shift, int bitCount);
    static const StateGroup *create(const StateGroup *prev, int bitCount);

    State();
    State(int initialState);
    int getState() const;
    int getState(const StateGroup *) const;
    bool getStateBoolean(const StateGroup *) const;
    void setState(const StateGroup *group, int value);
    void setState(const StateGroup *group, bool value);
    void toggleState(const StateGroup *group);
    void dispatchOnStateChangedListener(const StateGroup *changed, int newValue);

    class OnStateChangedListener
    {
    public:
        virtual void onStateChanged(State *state, const StateGroup *changed, int newValue) = 0;
    };
    void addOnStateChangedListener(OnStateChangedListener *listener);
    void removeOnStateChangedListener(OnStateChangedListener *listener);

private:
    unsigned int state;
    List<OnStateChangedListener*> onStateChangedListeners;
};

#endif /* UTILS_STATE_HPP */