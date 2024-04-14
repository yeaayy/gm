#ifndef GLVIEW_TEXT_VIEW_HPP
#define GLVIEW_TEXT_VIEW_HPP

#include "view.hpp"

typedef enum TextAlign {
	TEXT_ALIGN_CENTER = 0,
	TEXT_ALIGN_LEFT  = 1,
	TEXT_ALIGN_RIGHT = 2,
	TEXT_ALIGN_START = 4,
	TEXT_ALIGN_END   = 8,

	TEXT_ALIGN_LEFT_START  = TEXT_ALIGN_LEFT  | TEXT_ALIGN_START,
	TEXT_ALIGN_LEFT_END    = TEXT_ALIGN_LEFT  | TEXT_ALIGN_END,
	TEXT_ALIGN_RIGHT_START = TEXT_ALIGN_RIGHT | TEXT_ALIGN_START,
	TEXT_ALIGN_RIGHT_END   = TEXT_ALIGN_RIGHT | TEXT_ALIGN_END
} TextAlign;

class TextView : public View {
  public:
	TextView(View *parent = 0,
			 ViewSize sizeX = VIEW_SIZE_MIN,
			 ViewSize sizeY = VIEW_SIZE_MIN);
	void setText(const char*);
	void setTextSize(float);
	void setTextAlign(TextAlign);
	void setColor(const float *rgba);
	void setColor(float r, float g, float b, float a);
	virtual void onResize(float maxWidth, float maxHeight) override;
	virtual void onInitGL() override;
	virtual void onRender() override;

  private:
	int drawCount;
	unsigned int vbo;
	float size;
	float color[4];

	char *text;
	float textWidth, textHeight;
	TextAlign align;

	bool needUpdateBuffer;
	void updateBuffer(float, float);
	void updateIBO(int);
};

#endif /* GLVIEW_TEXT_VIEW_HPP */