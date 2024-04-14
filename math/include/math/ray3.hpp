
#ifndef __MATH_RAY3_H
#define __MATH_RAY3_H

class Ray3 {
  public:
	float sx, sy, sz;
	float dx, dy, dz;
	Ray3(float sx, float sy, float sz, float dx, float dy, float dz);
	float planeIntersection(float a, float b, float c, float d);
	void at(float *dst, float t);
	static Ray3 *fromLine(float *from, float *to);
};

#endif /* __MATH_RAY3_H */