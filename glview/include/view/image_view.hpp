#ifndef GLVIEW_IMAGE_VIEW_HPP
#define GLVIEW_IMAGE_VIEW_HPP

#include "view.hpp"

class ImageView : public View
{
  public:
	ImageView(View *parent = 0,
			  ViewSize sizeX = VIEW_SIZE_MIN,
			  ViewSize sizeY = VIEW_SIZE_MIN);
	void setImage(const char *name);
	void setImage(int idx, const char *name);

	virtual void onInitGL() override;
	virtual void onRender() override;

  protected:
	char **imagesName;
	unsigned int *imagesId;
	int count;
};

#endif /* GLVIEW_IMAGE_VIEW_HPP */