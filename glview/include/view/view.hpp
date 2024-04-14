#ifndef VIEW_VIEW_HPP
#define VIEW_VIEW_HPP

#include "keymap.hpp"
#include "rectf.hpp"
#include "list.hpp"

class View;

typedef enum ViewSize {
	VIEW_SIZE_MAX = -1,
	VIEW_SIZE_MIN,
} ViewSize;

typedef enum ViewOrientation {
	VIEW_ORIENTATION_NONE,
	VIEW_ORIENTATION_VERTICAL,
	VIEW_ORIENTATION_HORIZONTAL,
} ViewOrientation;

class View: public Shortcut
{
  public:
	class OnClickListener {
		public: virtual bool onClick(View *src, float x, float y) = 0;
	};
	class OnToggledListener {
		public: virtual bool onToggled(View *src, int state) = 0;
	};

  public:
	View(View *parent = 0,
		 ViewSize sizeX = VIEW_SIZE_MIN,
		 ViewSize sizeY = VIEW_SIZE_MIN);
	virtual ~View();
	void setId(int id);
	int getId();
	void setToggleCycle(int count);
	void setToggleState(int state);
	void setOnClickListener(OnClickListener *listener);
	void setOnToggledListener(OnToggledListener *listener);
	void show();
	void hide();
	bool isVisible();
	bool setVisible(bool = true);
	void setPosition(float x, float y);
	void setPrefSize(float width, float height);
	void setViewSize(ViewSize sizeX, ViewSize sizeY);
	void setMargin(float l, float r, float t, float b);
	void setOrientation(ViewOrientation orientation);
	void attachView(View *parent);
	void detachView();
	void calculateClip(const RectF *box);
	void setHorizontallyScrollable(bool);
	void setVerticallyScrollable(bool);
	bool isHorizontallyScrollable();
	bool isVerticallyScrollable();
	void viewport();
	void setBackgroundColor(float r, float g, float b, float a);
	void setRenderBackground(bool);
	bool isRenderBackground();
	void renderBackground();
	void log(int depth = 0);
	virtual void onInitGL();
	virtual void onResize(float maxWidth, float maxHeight);
	virtual void onRender();
	virtual bool onClick(float x, float y);
	virtual bool onToggled(int state);
	virtual bool onScroll(float sx, float sy, float ax, float ay, float dx, float dy);
	virtual bool onBlock(KeyModifier mod, float sx, float sy, float ex, float ey);
	virtual bool onScale(float x, float y, float scale);
	virtual bool isInside(float px, float py);
    virtual bool onShortcutTriggered(KeyModifier mod, Keycode key) override;
	static void requestResize();

  protected:
	int id;
	View *parent;
	// relative position
	float x, y;
	// rendered dimension
	float width, height;
	RectF cRegion;

	// Dimension the view want
	ViewOrientation orientation;
	float prefWidth, prefHeight;
	float mLeft, mTop, mRight, mBottom;
	float pLeft, pTop, pRight, pBottom;
	ViewSize sizeX, sizeY;
	List<View*> childs;

	// scrolling
	float dx, dy;
	bool xscrollable;
	bool yscrollable;
	float contentWidth;
	float contentHeight;

	bool visible;
	int toggleCycle;
	int toggleState;
	OnClickListener *onClickListener;
	OnToggledListener *onToggledListener;
	float r, g, b, a;
	bool mrenderBackground;

  private:
	bool initialized = false;
};

#endif /* VIEW_VIEW_HPP */