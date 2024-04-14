
#include "view/rectf.hpp"

RectF::RectF()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

RectF::RectF(float l, float t, float r, float b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

RectF::RectF(RectF *ref)
{
	left = ref->left;
	top = ref->top;
	right = ref->right;
	bottom = ref->bottom;
}

void RectF::set(float l, float t, float r, float b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}