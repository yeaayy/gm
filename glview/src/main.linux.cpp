
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>

#include "gl.h"
#include "main.all.hpp"
#include "view/keymap.hpp"

namespace {
Display              *dpy;
Window               root;
GLint                att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
XVisualInfo          *vi;
Colormap             cmap;
XSetWindowAttributes swa;
Window               win;
GLXContext           glc;
XWindowAttributes    gwa;
XEvent               xev;

enum State {
    STATE_SHIFT = 1,
    STATE_CAPLOCK = 2,
    STATE_CTRL = 4,
    STATE_ALT = 8,
    STATE_LEFT_BUTTON_DOWN = 256,
    STATE_MIDDLE_BUTTON_DOWN = 512,
    STATE_RIFGHT_BUTTON_DOWN = 1024,

    STATE_CTRL_SHIFT = STATE_CTRL | STATE_SHIFT,
    STATE_CTRL_ALT_SHIFT = STATE_CTRL | STATE_ALT | STATE_SHIFT,
};

Keycode keycodeMap[180] {
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_ESCAPE,
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
    KEYCODE_MINUS,
    KEYCODE_EQUAL,
    KEYCODE_BACKSPACE,
    KEYCODE_TAB,
    KEYCODE_Q,
    KEYCODE_W,
    KEYCODE_E,
    KEYCODE_R,
    KEYCODE_T,
    KEYCODE_Y,
    KEYCODE_U,
    KEYCODE_I,
    KEYCODE_O,
    KEYCODE_P,
    KEYCODE_OPEN_SQUARE_BRACKET,
    KEYCODE_CLOSE_SQUARE_BRACKET,
    KEYCODE_ENTER,
    KEYCODE_UNKNOWN, // Left CTRL
    KEYCODE_A,
    KEYCODE_S,
    KEYCODE_D,
    KEYCODE_F,
    KEYCODE_G,
    KEYCODE_H,
    KEYCODE_J,
    KEYCODE_K,
    KEYCODE_L,
    KEYCODE_SEMICOLON,
    KEYCODE_QUOTE,
    KEYCODE_GRAVE_ACCENT,
    KEYCODE_UNKNOWN, // Left shift
    KEYCODE_BACKSLASH,
    KEYCODE_Z,
    KEYCODE_X,
    KEYCODE_C,
    KEYCODE_V,
    KEYCODE_B,
    KEYCODE_N,
    KEYCODE_M,
    KEYCODE_COMA,
    KEYCODE_POINT,
    KEYCODE_SLASH,
    KEYCODE_UNKNOWN, // Right shift
    KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, // Left ALT
    KEYCODE_SPACE,
    KEYCODE_UNKNOWN, // Capslock
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
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_F11,
    KEYCODE_F12,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, // Right CTRL
    KEYCODE_UNKNOWN, KEYCODE_UNKNOWN,
    KEYCODE_UNKNOWN, // Right ALT
    KEYCODE_UNKNOWN,
    KEYCODE_HOME,
    KEYCODE_ARROW_UP,
    KEYCODE_PAGE_UP,
    KEYCODE_ARROW_LEFT,
    KEYCODE_ARROW_RIGHT,
    KEYCODE_END,
    KEYCODE_ARROW_DOWN,
    KEYCODE_PAGE_DOWN,
    KEYCODE_INSERT,
    KEYCODE_DELETE,
};

bool onExit(KeyModifier mod, Keycode key)
{
    glXMakeCurrent(dpy, None, NULL);
    glXDestroyContext(dpy, glc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    exit(0);
}

KeyModifier getModifier(int state)
{
    int mod = MODIFIER_NONE;
    if(state & STATE_SHIFT) mod |= MODIFIER_SHIFT;
    if(state & STATE_CTRL) mod |= MODIFIER_CTRL;
    if(state & STATE_ALT) mod |= MODIFIER_ALT;
    if(state & STATE_CAPLOCK) mod |= MODIFIER_CAPSLOCK;
    return (KeyModifier) mod;
}


} // namespace

int main(int argc, const char **argv) {
    using namespace glview;

    chdir("bin");
    setbuf(stdout, nullptr);
    dpy = XOpenDisplay(NULL);
    if(dpy == nullptr) {
        printf("cannot connect to X Server\n");
        exit(0);
    }
    root = DefaultRootWindow(dpy);
    vi = glXChooseVisual(dpy, 0, att);
    if(vi == nullptr) {
        printf("No appropriate visual found\n");
        exit(0);
    }
    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = FocusChangeMask | ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "OPENGL WINDOW");
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);

    onGLInit();
    SimpleShortcut exitShortcut(onExit);
    exitShortcut.setActive();
    Shortcut::add(MODIFIER_SHIFT, KEYCODE_ESCAPE, &exitShortcut);
    bool validClick;
    int sx, sy;
    int lx, ly;

    while(true) {
        if(XEventsQueued(dpy, QueuedAfterFlush) == 0) {
            onGLRender();
            glXSwapBuffers(dpy, win);
            continue;
        }
        if(rootView == nullptr) continue;
        XNextEvent(dpy, &xev);
        switch(xev.type) {
        case Expose:
            XGetWindowAttributes(dpy, win, &gwa);
            onGLResize(gwa.width, gwa.height);
            break;
        case ButtonPress:
            switch(xev.xbutton.button) {
            case Button1: // Left click
                validClick = true;
                lx = sx = xev.xbutton.x;
                ly = sy = xev.xbutton.y;
                break;
            }
            break;
        case ButtonRelease:
            switch(xev.xbutton.button) {
            case Button1: // Left click
                if(validClick) {
                    rootView->onClick(xev.xbutton.x, xev.xbutton.y);
                }
                break;
            case Button4: // Scroll down
                switch(xev.xkey.state & STATE_CTRL_ALT_SHIFT) {
                case STATE_CTRL:
                    rootView->onScale(xev.xbutton.x, xev.xbutton.y, 0.9f);
                    break;
                case STATE_SHIFT:
                    rootView->onScroll(xev.xbutton.x, xev.xbutton.y, xev.xbutton.x, xev.xbutton.y, 40, 0);
                    break;
                case 0:
                    rootView->onScroll(xev.xbutton.x, xev.xbutton.y, xev.xbutton.x, xev.xbutton.y, 0, 40);
                    break;
                }
                break;
            case Button5: // Scroll up
                switch(xev.xkey.state & STATE_CTRL_ALT_SHIFT) {
                case STATE_CTRL:
                    rootView->onScale(xev.xbutton.x, xev.xbutton.y, 1 / 0.9f);
                    break;
                case STATE_SHIFT:
                    rootView->onScroll(xev.xbutton.x, xev.xbutton.y, xev.xbutton.x, xev.xbutton.y, -40, 0);
                    break;
                case 0:
                    rootView->onScroll(xev.xbutton.x, xev.xbutton.y, xev.xbutton.x, xev.xbutton.y, 0, -40);
                    break;
                }
                break;
            case 6: // Scroll right
                switch(xev.xkey.state & STATE_CTRL_ALT_SHIFT) {
                case 0:
                    rootView->onScroll(xev.xbutton.x, xev.xbutton.y, xev.xbutton.x, xev.xbutton.y, 40, 0);
                }
                break;
            case 7: // Scroll left
                switch(xev.xkey.state & STATE_CTRL_ALT_SHIFT) {
                case 0:
                    rootView->onScroll(xev.xbutton.x, xev.xbutton.y, xev.xbutton.x, xev.xbutton.y, -40, 0);
                }
                break;
            }
            break;
        case MotionNotify:
            if(xev.xkey.state & STATE_LEFT_BUTTON_DOWN) {
                int cx = xev.xmotion.x;
                int cy = xev.xmotion.y;
                validClick &= hypot(sx - cx, sy - cy) * 128 < (mwidth < mheight ? mwidth : mheight);
                if(!validClick) {
                    // rootView->onScroll(sx, sy, cx, cy, cx - lx, cy - ly);
                    rootView->onBlock(getModifier(xev.xkey.state), sx, sy, cx, cy);
                }
                lx = cx;
                ly = cy;
            }
            break;
        case KeyRelease:
            break;
        case KeyPress: {
            OnInputListener::dispatch(getModifier(xev.xkey.state), keycodeMap[xev.xkey.keycode]);
            break;
        }
        case FocusIn:
            break;
        case FocusOut:
            break;
        default:
            printf("Unhandled event type %d\n", xev.type);
        }
    }
    return 0;
}
