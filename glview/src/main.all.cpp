#include "main.all.hpp"

#include <time.h>

#include "gl.h"
#include "list.tcc"
#include "log.h"

extern "C" {
extern List<View*> uninitializedList;
}

namespace {

bool needResize = true;


RectF screen;

typedef struct TimeDiff {
	struct timespec start, end;
} TimeDiff;

void time_diff_start(TimeDiff *td) {
	clock_gettime(CLOCK_MONOTONIC_RAW, &td->start);
}

float time_diff_end(TimeDiff *td) {
	clock_gettime(CLOCK_MONOTONIC_RAW, &td->end);
	int ns = td->end.tv_nsec - td->start.tv_nsec;
	int us = ns / 1000;
	int ms = us / 1000;
	return (td->end.tv_sec - td->start.tv_sec) + (float) ms / 1000.0f;
}

TimeDiff timeDiff;

} // namespace

void View::requestResize()
{
	needResize = true;
}

void View::show()
{
	using namespace glview;
	if(parent == nullptr) {
		if(rootView != nullptr) {
			rootView->visible = false;
		}
		rootView = this;
	}
	visible = true;
}

void View::hide()
{
	using namespace glview;
	if(this == rootView) {
		rootView = nullptr;
	}
	visible = false;
}

namespace glview {

View *rootView = nullptr;
int mwidth;
int mheight;

void onGLResize(int width, int height)
{
	if(mwidth == width && mheight == height) return;
	mwidth = width;
	mheight = height;
	screen.set(0, 0, width, height);
	needResize = true;
}

void onGLRender()
{
	if(uninitializedList.size() > 0) {
		for(int i=0; i<uninitializedList.size(); i++) {
			uninitializedList[i]->onInitGL();
		}
		uninitializedList.clear();
	}
	if(needResize) {
		needResize = false;
		if(rootView != nullptr) {
			rootView->onResize(mwidth, mheight);
		}
	}
	glDisable(GL_SCISSOR_TEST);
	glDepthMask(GL_TRUE);
	glViewport(0,0, mwidth, mheight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	glEnable(GL_SCISSOR_TEST);
	if(rootView != nullptr) {
		rootView->calculateClip(&screen);
	}
	glScissor((int) screen.left, (int) screen.top, (int) screen.right, (int) screen.bottom);
	if(rootView !=nullptr) {
		rootView->viewport();
		rootView->onRender();
	}

	// view frame rate
	static float frate = 0;
	float sec = time_diff_end(&timeDiff);
	if(sec > 0) {
		frate = frate * 0.8f + 0.2f / sec;
	}
	time_diff_start(&timeDiff);
}

} // namespace glview
