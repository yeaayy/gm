#include "view/keymap.hpp"

#include "list.tcc"
#include "map.tcc"

namespace {
List<OnInputListener*> onInputListeners;
}

class OnShortcutListener: public OnInputListener
{
public:
    bool onInput(KeyModifier mod, Keycode code) override;
    void add(int code, Shortcut *listener);
    bool remove(int code, Shortcut *listener);
    static int getIndex(KeyModifier mod, Keycode code);
    static OnShortcutListener *getInstance();

private:
    Map<int, Shortcut*> shortcuts;
};

char keycodeGetCharacter(KeyModifier mod, Keycode code)
{
    char lmap[KEYCODE_COUNT] {
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        ' ', '-', '=', '[', ']', ';', '\'', '`', '\\', ',', '.', '/', '\n',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    };
    char umap[KEYCODE_COUNT] {
        '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        ' ', '_', '+', '{', '}', ':', '"', '~', '|', '<', '>', '?', '\n',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    };
    char *map = lmap;
    if((mod & MODIFIER_SHIFT) == MODIFIER_SHIFT) {
        if((mod & MODIFIER_CAPSLOCK) == 0 || !keycodeIsLetter(code)) {
            map = umap;
        }
    }
    return map[code];
}

bool keycodeIsLetter(Keycode code)
{
    return code >= KEYCODE_A && code <= KEYCODE_Z;
}

void OnInputListener::add(OnInputListener *listener)
{
    onInputListeners.push(listener);
}

bool OnInputListener::remove(OnInputListener *listener)
{
    return onInputListeners.removeItem(listener);
}

void OnInputListener::dispatch(KeyModifier mod, Keycode code)
{
    for(int i = 0; i < onInputListeners.size(); i++) {
        if(onInputListeners[i]->onInput(mod, code)) {
            return;
        }
    }
}

Shortcut* Shortcut::setActive(bool active)
{
    this->isActivated = active;
    return this;
}

bool Shortcut::isActive()
{
    return isActivated;
}

void Shortcut::add(KeyModifier mod, Keycode key, Shortcut *listener)
{
    OnShortcutListener::getInstance()->add(OnShortcutListener::getIndex(mod, key), listener);
}

bool Shortcut::remove(KeyModifier mod, Keycode key, Shortcut *listener)
{
    return OnShortcutListener::getInstance()->remove(OnShortcutListener::getIndex(mod, key), listener);
}

bool OnShortcutListener::onInput(KeyModifier mod, Keycode code)
{
    int indexCode = getIndex(mod, code);
    for(int i = 0; i < shortcuts.size(); i++) {
        int key = shortcuts.key(i);
        if(key != indexCode) continue;
        Shortcut *shortcut = shortcuts.value(i);
        if(shortcut->isActive()) {
            if(shortcut->onShortcutTriggered(mod, code)) {
                return true;
            }
        }
    }
    return false;
}

int OnShortcutListener::getIndex(KeyModifier mod, Keycode key)
{
    return (mod & ~MODIFIER_CAPSLOCK) * KEYCODE_COUNT + key;
}

void OnShortcutListener::add(int code, Shortcut *listener)
{
    shortcuts.add(code, listener);
}

bool OnShortcutListener::remove(int code, Shortcut *listener)
{
    loge("TODO: Impelemnt this method");
    return false;
}

OnShortcutListener *OnShortcutListener::getInstance()
{
    static OnShortcutListener *instance = nullptr;
    if(instance == nullptr) {
        instance = new OnShortcutListener();
        onInputListeners.push(instance);
    }
    return instance;
}

SimpleShortcut::SimpleShortcut(bool (*listener)(KeyModifier, Keycode))
{
    this->listener = listener;
}

bool SimpleShortcut::onShortcutTriggered(KeyModifier mod, Keycode key)
{
    return listener(mod, key);
}