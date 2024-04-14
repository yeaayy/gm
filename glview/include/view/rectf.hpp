#ifndef RECTF_HPP
#define RECTF_HPP

class RectF
{
  public:
	float left;
	float top;
	float right;
	float bottom;
	RectF();
	RectF(float l, float t, float r, float b);
	RectF(RectF *ref);
	void set(float l, float t, float r, float b);
};

#endif /* RECTF_HPP */