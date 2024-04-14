#include "math/ray3.hpp"

Ray3::Ray3(float sx, float sy, float sz, float dx, float dy, float dz){
	this->sx = sx;
	this->sy = sy;
	this->sz = sz;
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;
}

float Ray3::planeIntersection(float a, float b, float c, float d){
	return -(d + a * sx + b * sy + c * sz)/(a * dx + b * dy + c * dz);
}

void Ray3::at(float *dst, float t){
	dst[0] = this->sx + t * this->dx;
	dst[1] = this->sy + t * this->dy;
	dst[2] = this->sz + t * this->dz;
}

Ray3 *Ray3::fromLine(float *from, float *to){
	return new Ray3(from[0], from[1], from[2], to[0] - from[0], to[1] - from[1], to[2] - from[2]);
}