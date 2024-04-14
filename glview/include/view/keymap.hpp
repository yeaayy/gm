#ifndef VIEW_KEYMAP_HPP
#define VIEW_KEYMAP_HPP

typedef enum KeyModifier {
    MODIFIER_NONE = 0,
    MODIFIER_SHIFT = 1,
    MODIFIER_CTRL = 2,
    MODIFIER_ALT = 4,
    MODIFIER_CAPSLOCK = 8,

    // Combination
    MODIFIER_CTRL_SHIFT = MODIFIER_CTRL | MODIFIER_SHIFT,
    MODIFIER_CTRL_ALT = MODIFIER_CTRL | MODIFIER_ALT,
    MODIFIER_CTRL_ALT_SHIFT = MODIFIER_CTRL | MODIFIER_ALT | MODIFIER_SHIFT,
} KeyModifier;

typedef enum Keycode {
    KEYCODE_1,
    KEYCODE_2,
    KEYCODE_3,
    KEYCODE_4,
    KEYCODE_5,
    KEYCODE_6,
    KEYCODE_7,
    KEYCODE_8,
    KEYCODE_9,
    KEYCODE_0,
    KEYCODE_A,
    KEYCODE_B,
    KEYCODE_C,
    KEYCODE_D,
    KEYCODE_E,
    KEYCODE_F,
    KEYCODE_G,
    KEYCODE_H,
    KEYCODE_I,
    KEYCODE_J,
    KEYCODE_K,
    KEYCODE_L,
    KEYCODE_M,
    KEYCODE_N,
    KEYCODE_O,
    KEYCODE_P,
    KEYCODE_Q,
    KEYCODE_R,
    KEYCODE_S,
    KEYCODE_T,
    KEYCODE_U,
    KEYCODE_V,
    KEYCODE_W,
    KEYCODE_X,
    KEYCODE_Y,
    KEYCODE_Z,
    KEYCODE_SPACE,
    KEYCODE_MINUS,
    KEYCODE_EQUAL,
    KEYCODE_OPEN_SQUARE_BRACKET,
    KEYCODE_CLOSE_SQUARE_BRACKET,
    KEYCODE_SEMICOLON,
    KEYCODE_QUOTE,
    KEYCODE_GRAVE_ACCENT,
    KEYCODE_BACKSLASH,
    KEYCODE_COMA,
    KEYCODE_POINT,
    KEYCODE_SLASH,
    
    // Special
    KEYCODE_ENTER,
    KEYCODE_TAB,
    KEYCODE_BACKSPACE,
    KEYCODE_INSERT,
    KEYCODE_DELETE,

    // Function
    KEYCODE_ESCAPE,
    KEYCODE_F1,
    KEYCODE_F2,
    KEYCODE_F3,
    KEYCODE_F4,
    KEYCODE_F5,
    KEYCODE_F6,
    KEYCODE_F7,
    KEYCODE_F8,
    KEYCODE_F9,
    KEYCODE_F10,
    KEYCODE_F11,
    KEYCODE_F12,
    KEYCODE_ARROW_UP,
    KEYCODE_ARROW_LEFT,
    KEYCODE_ARROW_DOWN,
    KEYCODE_ARROW_RIGHT,
    KEYCODE_PAGE_UP,
    KEYCODE_PAGE_DOWN,
    KEYCODE_HOME,
    KEYCODE_END,

    KEYCODE_UNKNOWN,
    KEYCODE_COUNT,
} Keycode;

char keycodeGetCharacter(KeyModifier, Keycode);
bool keycodeIsSymbol(Keycode);
bool keycodeIsLetter(Keycode);
bool keycodeIsPrintable(Keycode);
bool keycodeIsFunction(Keycode);
bool keycodeIsNavigation(Keycode);

class OnInputListener
{
public:
    static void add(OnInputListener*);
    static bool remove(OnInputListener*);
    static void dispatch(KeyModifier mod, Keycode code);

    virtual bool onInput(KeyModifier mod, Keycode code) = 0;
};

class Shortcut
{
public:
    virtual bool onShortcutTriggered(KeyModifier mod, Keycode key) = 0;
    Shortcut *setActive(bool active = true);
    bool isActive();
    
    static void add(KeyModifier mod, Keycode key, Shortcut *listener);
    static bool remove(KeyModifier mod, Keycode key, Shortcut *listener);

private:
    bool isActivated;
};

class SimpleShortcut: public Shortcut
{
public:
    SimpleShortcut(bool (*listener)(KeyModifier, Keycode));
    bool onShortcutTriggered(KeyModifier mod, Keycode key) override;

private:
    bool (*listener)(KeyModifier, Keycode);
};

#endif /* VIEW_KEYMAP_HPP */