
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "gl.h"
#include "view/view.hpp"
#include "view/text_view.hpp"

#include "list.tcc"
#include "log.h"
#include "main.all.hpp"

extern "C" {

typedef enum TouchAction {
	ACTION_DOWN = 0,
	ACTION_UP = 1,
	ACTION_MOVE = 2,
	ACTION_POINTER_DOWN = 5,
	ACTION_POINTER_UP = 6,
} ToutchAction;

typedef struct Application Application;
struct Application {
	void (*setOrientation)(Application*, int);
	void (*setTheme)(Application*, const char*);
};

int onCreate(Application *app){
	app->setOrientation(app, 1);
	app->setTheme(app, "Theme_NoTitleBar");
	return 2;
}

void onInit() {
	onGLInit();
}

void render()
{
	onGLRender();
}

void onResize(int width, int height)
{
	onGLResize(width, height);
}


List<int> pointers;

float sx, sy, ux, uy, lx, ly;
float lastPointerDistance;
bool isValidClick;

int findPointerIndex(int count, int *ids, int id)
{
	for(int i=0; i<count; i++) {
		if(ids[i] == id) {
			return i;
		}
	}
	logp("id not found");
	return -1;
}

float getPointerDistance(int count, int *id, float *X, float *Y) {
	int index1 = findPointerIndex(count, id, pointers[0]);
	int index2 = findPointerIndex(count, id, pointers[1]);
	return hypotf(X[index1] - X[index2], Y[index1] - Y[index2]);
}

bool onTouch(int action, int count, float *X, float *Y, int actionIdx, int *id)
{
	switch(action) {
	case ACTION_MOVE:
		if(pointers.size() > 1) {
			float pointerDistance = getPointerDistance(count, id, X, Y);
			rootView->onScale(*X, *Y, lastPointerDistance / pointerDistance);
			lastPointerDistance = pointerDistance;
		} else {
			ux += *X - lx;
			uy += ly - *Y;
			isValidClick &= hypotf(*X - sx, *Y - sy) * 32 < mwidth;
			if(!isValidClick) {
				rootView->onScroll(sx, sy, *X, *Y, *X - lx, *Y - ly);
			}
		}
		break;
	case ACTION_DOWN:
		sx = *X;
		sy = *Y;
		isValidClick = true;
	case ACTION_POINTER_DOWN:
		pointers.push(id[actionIdx]);
		if(pointers.size() > 1){
			isValidClick = false;
			lastPointerDistance = getPointerDistance(count, id, X, Y);
		}
		break;
	case ACTION_UP:
		if(isValidClick) {
			rootView->onClick(sx, sy);
		}
	case ACTION_POINTER_UP:
		assert_true(pointers.removeItem(id[actionIdx]));
		if(pointers.size() == 1) {
			int index = findPointerIndex(count, id, pointers[0]);
			*X = X[index];
			*Y = Y[index];
		} else if(pointers.size() > 1) {
			lastPointerDistance = getPointerDistance(count, id, X, Y);
		}
		break;
	}
	lx = *X;
	ly = *Y;
	return true;
}

} // extern "C"
