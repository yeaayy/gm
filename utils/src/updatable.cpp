#include "updatable.hpp"

#include "list.tcc"

namespace {
List<Updatable*> updatebles;
} // namespace


Updatable::Updatable(bool active)
{
    activateUpdate(active);
}

void Updatable::updateAll(float dt)
{
    for(int i=0; i<updatebles.size(); i++) {
        if(updatebles[i]->isUpdateActivated()) {
            updatebles[i]->update(dt);
        }
    }
}

void Updatable::activateUpdate(bool active)
{
    if(isUpdateActivate == active) {
        return;
    }
    this->isUpdateActivate = active;
    if(active) {
        updatebles.push(this);
    } else {
        updatebles.removeItem(this);
    }
}

bool Updatable::isUpdateActivated()
{
    return this->isUpdateActivate;
}

void Updatable::update(float dt)
{}
