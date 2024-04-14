
#include <math.h>

#include "gl.h"
#include "math/mat4.h"

#include "math/camera.hpp"
#include "texture/atlas.hpp"
#include "texture/renderable_sprite.hpp"

class View;
View *rootView;
int mwidth;
int mheight;

Camera *cam;
SpriteMap atlas;
RenderableSprite **testRender;

void onGLInit()
{
	glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_ADD);
	glEnable(GL_DEPTH_TEST);


	cam = new Camera();
	cam->frustum = false;
	cam->setNearFar(-50, 50);
	cam->rotateView(0, 0, 1, -M_PI * 3 / 4);
	cam->rotateViewRelative(1,0,0, -acosf(sqrtf(1.0f / 3.0f)));
	cam->updateViewProjectionMatrix();

	float transformation1[16];
	float transformation2[16];
	float r[16];
	mat4_setIdentity(transformation1);

	mat4_translateA(transformation1, transformation1, 0, -M_SQRT1_2, 0);
	mat4_scaleA(transformation1, transformation1, 1, 2, 1);
	mat4_rotateA(transformation1, transformation1, 0,0,1, M_PI * 3 / 4);

	mat4_invert(transformation2, transformation1);

	mat4_translateA(transformation2, transformation2, 0, -M_SQRT1_2, 0);
	mat4_scaleA(transformation2, transformation2, 1, M_SQRT2, 1);
	mat4_rotateA(transformation2, transformation2, 1,0,0, M_PI_2);
	mat4_rotateA(transformation2, transformation2, 0,0,1, M_PI * 3 / 4);

	mat4_mulmm(r, transformation2, transformation1);

	TextureAtlas::initRender();
	TextureAtlas *texture = TextureAtlas::loadXML(&atlas, "../assets/tilesets/tiles_summer_1.xml", transformation1);
	texture->copySprite("tile_2x2_g_0_0_00", "my", transformation2);
	texture->finish();
	texture = TextureAtlas::loadXML(&atlas, "../assets/buildings/buildings_01.xml", transformation1);
	texture->copySprite("tile_2x2_g_0_0_00", "my", transformation2);
	texture->finish();

	testRender = new RenderableSprite*[4] {
		new RenderableSprite(atlas["tile_2x2_g_0_0_00"], 0, 0, 0),
		new RenderableSprite(atlas["tile_2x2_g_0_0_00"], 2, 2, 2),
		new RenderableSprite(atlas["my"], 0, 0, 0),
		nullptr
	};
}

void onGLResize(int width, int height)
{
	glViewport(0,0, width, height);
	float x, y;
	if(width < height) {
		x = 1;
		y = height / width;
	} else {
		x = width / height;
		y = 1;
	}
	cam->setViewport(x, -x, y, -y);

	float scale = 3.0f;
	cam->scaleViewport(scale, scale);
}

void onGLRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthMask(1);
	// cam->rotateViewRelative(0, 1, 0, 0.01f);
	TextureAtlas::beginRender(cam);
	for(RenderableSprite **rs = testRender; *rs; rs++) {
		(*rs)->render();
	}
}
