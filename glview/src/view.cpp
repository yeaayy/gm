#include "view/view.hpp"

#include <stdlib.h>
#include <string.h>

#include "shader_program.hpp"

#include "list.tcc"

extern "C" {
List<View*> uninitializedList;
}

namespace {

ShaderProgram program("view/simple");

float getSize(float pref, float max, ViewSize size, float start, float end)
{
	if(size == VIEW_SIZE_MAX) {
		if(start != -1) max -= start;
		if(end != -1) max -= end;
		return max;
	} else {
		return pref;
	}
}

// count auto margin and return max of a and b
float marginSelector(int *count, float a, float b)
{
	if(a == -1 && b == -1) {
		count[0]++;
		return 0;
	}
	if(a == -1) count[0]++;
	if(b == -1) count[0]++;
	if(a > b) return a;
	return b;
}

float marginSelector2(float a, float b, float autoSize)
{
	if(a == -1) a = autoSize;
	if(b == -1) b = autoSize;
	if(a > b) return a;
	return b;
}

// component size perpendicular to orientation
float getComponentSize(
	float oldCompSize,
	float marginStart,
	float compSize,
	float marginEnd,
	float parentSize
) {
	float size;
	if(marginStart == -1 || marginEnd == -1) {
		size = parentSize;
	} else {
		size = compSize + marginStart + marginEnd;
	}
	if(size > oldCompSize) return size;
	return oldCompSize;
}

} // namespace

View::View(View *parent,
		   ViewSize sizeX,
		   ViewSize sizeY)
{
	uninitializedList.push(this);
	this->id = rand();
	this->parent = parent;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	mrenderBackground = false;
	mLeft = 0;
	mTop = 0;
	mRight = 0;
	mBottom = 0;
	orientation = VIEW_ORIENTATION_VERTICAL;
	x = 0;
	y = 0;
	width = 1;
	height = 1;
	dx = 0;
	dy = 0;
	visible = true;
	xscrollable = false;
	yscrollable = false;
	mrenderBackground = false;
	toggleCycle = 1;
	toggleState = 0;
	onClickListener = nullptr;
	onToggledListener = nullptr;
	if(parent != nullptr) {
		parent->childs.push(this);
	}
}

View::~View()
{
	if(initialized) return;
	uninitializedList.removeItem(this);
}

void View::setId(int id)
{
	this->id = id;
}

int View::getId()
{
	return id;
}

void View::setOnClickListener(View::OnClickListener *listener)
{
	this->onClickListener = listener;
}

void View::setToggleCycle(int count)
{
	this->toggleCycle = count;
}

void View::setToggleState(int state)
{
	this->toggleState = state;
}

void View::setOnToggledListener(OnToggledListener *listener)
{
	this->onToggledListener = listener;
}

void View::attachView(View *parent)
{
	if(this->parent != nullptr) {
		detachView();
	}
	this->parent = parent;
	if(parent != nullptr) {
		parent->childs.push(this);
	}
}

void View::detachView()
{
	if(parent == nullptr) {
		return;
	}
	parent->childs.removeItem(this);
	parent = nullptr;
}

void View::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void View::setPrefSize(float width, float height)
{
	prefWidth = width;
	prefHeight = height;
}

void View::setViewSize(ViewSize sizeX, ViewSize sizeY)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
}

void View::setMargin(float l, float r, float t, float b)
{
	mLeft = l;
	mTop = r;
	mRight = t;
	mBottom = b;
}

void View::setOrientation(ViewOrientation orientation)
{
	this->orientation = orientation;
}

void View::setHorizontallyScrollable(bool scroll)
{
	xscrollable = scroll;
}

void View::setVerticallyScrollable(bool scroll)
{
	yscrollable = scroll;
}

bool View::isHorizontallyScrollable()
{
	return xscrollable;
}

bool View::isVerticallyScrollable()
{
	return yscrollable;
}

void View::onInitGL()
{
	initialized = true;

	program.loadStringIfNeeded(
#include "shader/simple.vertex.glsl"
	,
#include "shader/simple.fragment.glsl"
	);
}

void View::onResize(float maxWidth, float maxHeight)
{
	width = getSize(prefWidth, maxWidth, sizeX, mLeft, mRight);
	height = getSize(prefHeight, maxHeight, sizeY, mTop, mBottom);

	if(childs.size() == 0) return;
	float sizeSum = 0;
	float sideSize = 0;
	int count = 0;
	float marginSum = 0;
	float mLast = 0;
	float autoSize = 0;
	for(int i=0; i<childs.size(); i++) {
		View *child = childs[i];
		if(orientation == VIEW_ORIENTATION_VERTICAL) {
			child->onResize(width, height - sizeSum - marginSum);
			sizeSum += child->height;
			marginSum += marginSelector(&count, mLast, child->mTop);
			mLast = child->mBottom;
			sideSize = getComponentSize(sideSize, child->mLeft, child->width, child->mRight, width);
		} else  if(orientation == VIEW_ORIENTATION_HORIZONTAL) {
			child->onResize(width - sizeSum - marginSum, height);
			sizeSum += child->width;
			marginSum += marginSelector(&count, mLast, child->mLeft);
			mLast = child->mRight;
			sideSize = getComponentSize(sideSize, child->mTop, child->height, child->mBottom, height);
		} else {
			child->onResize(width, height);
		}
	}
	marginSum += marginSelector(&count, mLast, 0);
	if(orientation == VIEW_ORIENTATION_VERTICAL) {
		if(count > 0) {
			autoSize = (height - sizeSum - marginSum) / count;
		}
	} else  if(orientation == VIEW_ORIENTATION_HORIZONTAL) {
		if(count > 0) {
			autoSize = (width - sizeSum - marginSum) / count;
		}
	}
	if(autoSize < 0) autoSize = 0;

	float absX = 0;
	float absY = 0;
	mLast = 0;
	for(int i=0; i<childs.size(); i++) {
		View *child = childs[i];
		child->x = absX;
		child->y = absY;
		if(orientation == VIEW_ORIENTATION_VERTICAL) {
			float shift = marginSelector2(mLast, child->mTop, autoSize);
			absY += child->height + shift;
			child->y += shift;
			mLast = child->mBottom;
			if(child->mLeft == -1) {
				if(child->mRight == -1) {
					child->x = (width - child->width) / 2;
				} else {
					child->x = width - child->width;
				}
			} else {
				child->x = child->mLeft;
			}
		} else if(orientation == VIEW_ORIENTATION_HORIZONTAL) {
			float shift = marginSelector2(mLast, child->mLeft, autoSize);
			absX += child->width + shift;
			child->x += shift;
			mLast = child->mRight;
			if(child->mTop == -1) {
				if(child->mBottom == -1) {
					child->y = (height - child->height) / 2;
				} else {
					child->y = height - child->height;
				}
			} else {
				child->y = child->mTop;
			}
		} else {
			if(child->mLeft == -1) {
				if(child->mRight == -1) {
					child->x = (width - child->width) / 2;
				} else {
					child->x = width - child->width - child->mRight;
				}
			} else {
				child->x = child->mLeft;
			}
			if(child->mTop == -1) {
				if(child->mBottom == -1) {
					child->y = (height - child->height) / 2;
				} else {
					child->y = height - child->height - child->mBottom;
				}
			} else {
				child->y = child->mTop;
			}
		}
	}
	if(orientation == VIEW_ORIENTATION_VERTICAL) {
		contentWidth = sideSize;
		contentHeight = absY;
	} else if(orientation == VIEW_ORIENTATION_HORIZONTAL) {
		contentWidth = absX;
		contentHeight = sideSize;
	}
}

void View::calculateClip(const RectF *box)
{
	cRegion.set(box->left + x, box->top + box->bottom - height - y, width, height);
}

void View::viewport()
{
	glViewport((int) cRegion.left, (int) cRegion.top, (int) cRegion.right, (int) cRegion.bottom);
}

void View::setRenderBackground(bool renderBackground)
{
	this->mrenderBackground = renderBackground;
}

void View::setBackgroundColor(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	mrenderBackground = true;
}

bool View::isRenderBackground()
{
	return mrenderBackground;
}

void View::renderBackground()
{
	program.use();
	static const float buff [8] {-1,-1, -1,1, 1,1, 1,-1};
	glVertexAttribPointer(program.vertexPositionIndex, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) buff);
	glUniform4f(program.uniformColorIndex, r, g, b, a);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void View::onRender()
{
	if(mrenderBackground) {
		renderBackground();
	}

	RectF shadow(&cRegion);
	shadow.left += dx;
	shadow.top -= dy;
	for(int i=0; i<childs.size(); i++) {
		View *child = childs[i];
		if(!child->visible) continue;
		child->calculateClip(&shadow);
		glScissor((int) cRegion.left, (int) cRegion.top, (int) cRegion.right, (int) cRegion.bottom);
		child->viewport();
		child->onRender();
	}
}

bool View::onClick(float x, float y)
{
	bool handled = false;
	if(onClickListener != nullptr) {
		handled = onClickListener->onClick(this, x, y);
	}
	if(!handled) {
		toggleState = (toggleState + 1) % toggleCycle;
		handled = onToggled(toggleState);
	}
	for(int i=childs.size() - 1; i>=0 && !handled; i--) {
		View *child = childs[i];
		if(!child->visible) continue;
		if(child->isInside(x - this->x - dx, y - this->y - dy)) {
			handled = child->onClick(x - this->x - dx, y - this->y -  dy);
		}
	}
	return handled;
}

bool View::onToggled(int state)
{
	if(onToggledListener != nullptr) {
		return onToggledListener->onToggled(this, toggleState);
	}
	return false;
}

bool View::onScroll(float sx, float sy, float ax, float ay, float dx, float dy)
{
	bool handled = false;
	if(xscrollable) {
		if(contentWidth > width) {
			this->dx += dx;
			if(this->dx > 0) this->dx = 0;
			if(this->dx < (width - contentWidth)) {
				this->dx = width - contentWidth;
			}
		} else {
			this->dx = 0;
		}
		handled = true;
	}
	if(yscrollable) {
		this->dy += dy;
		if(this->dy > 0) this->dy = 0;
		if(this->dy < (height - contentHeight)) {
			this->dy = height - contentHeight;
		}
		handled = true;
	}
	float shiftX = - this->x - this->dx;
	float shiftY = - this->y - this->dy;
	for(int i=childs.size() - 1; i>=0 && !handled; i--) {
		View *child = childs[i];
		if(!child->visible) continue;
		if(child->isInside(sx + shiftX, sy + shiftY)) {
			handled = child->onScroll(sx + shiftX, sy + shiftY, ax + shiftX, ay + shiftY, dx, dy);
		}
	}
	return handled;
}

bool View::onBlock(KeyModifier mod, float sx, float sy, float ex, float ey)
{
	bool handled = false;
	float shiftX = - this->x - this->dx;
	float shiftY = - this->y - this->dy;
	for(int i=childs.size() - 1; i>=0 && !handled; i--) {
		View *child = childs[i];
		if(!child->visible) continue;
		if(child->isInside(sx + shiftX, sy + shiftY)) {
			handled = child->onBlock(mod, sx + shiftX, sy + shiftY, ex + shiftX, ey + shiftY);
		}
	}
	return handled;
}

bool View::isInside(float px, float py)
{
	float relX = px - x;
	float relY = py - y;
	return relX >= 0 && relX <= width &&
		   relY >= 0 && relY <= height;
}

bool View::onScale(float x, float y, float scale)
{
	bool handled = false;
	for(int i=childs.size() - 1; i>=0 && !handled; i--) {
		View *child = childs[i];
		if(!child->visible) continue;
		if(child->isInside(x - this->x - dx, y - this->y - dy)) {
			handled = child->onScale(x - this->x - dx, y - this->y - dy, scale);
		}
	}
	return handled;
}

bool View::isVisible()
{
	if(parent != nullptr) {
		if(!parent->visible) {
			return false;
		}
	}
	return visible;
}

bool View::onShortcutTriggered(KeyModifier mod, Keycode key)
{
	if(!isVisible()) return false;
	return onClick(width / 2, height / 2);
}

void View::log(int depth)
{
	char *str = (char*) malloc(1);
	str[0] = '\0';
	for(int j=0; j<depth; j++) {
		char *tmp = str;
		asprintf(&str, "|%s", str);
		free(tmp);
	}
	char *tmp;
	asprintf(&tmp, "%s0x%08x", str, id);
	logp(tmp);
	free(tmp);
	free(str);
	for(int i=0; i<childs.size(); i++) {
		childs[i]->log(depth + 1);
	}
}
