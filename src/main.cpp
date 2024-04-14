
#include "gl.h"

#include "view/view.hpp" 
#include "game/game_view.hpp" 

void onGLInit()
{
	glClearColor(0, 0, 0, 1);
	(new GameView())->show();
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}
