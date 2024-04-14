#ifndef GLVIEW_MAIN_ALL_HPP
#define GLVIEW_MAIN_ALL_HPP

#include "view/view.hpp"

namespace glview {

extern View *rootView;
extern int mwidth;
extern int mheight;

void onGLResize(int width, int height);
void onGLRender();

} // namespace glview

void onGLInit();

#endif /* GLVIEW_MAIN_ALL_HPP */
