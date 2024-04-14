#ifndef __MATH_CAMERA_HPP
#define __MATH_CAMERA_HPP

#include "ray3.hpp"

class Camera {
  public:
	float viewProjectionMatrix[16];
	float projectionMatrix[16];
	float viewMatrix[16];
	bool frustum;
	float left;
	float right;
	float top;
	float bottom;
	float near;
	float far;

	Camera();
	void updateViewProjectionMatrix();

	void setProjection(float right, float left, float top, float bottom, float near, float far);
	void setViewport(float right, float left, float top, float bottom);
	void setNearFar(float near, float far);
	void scaleViewport(float xscale, float yscale);

	void rotateView(float ax, float ay, float az, float rad);
	void translateView(float x, float y, float z);
	void scaleView(float x, float y, float z);
	void translateViewRelative(float x, float y, float z);
	void rotateViewRelative(float ax, float ay, float az, float rad);

	Ray3 *getWorldRay(float screenX, float screenY);

private:
	void updateProjection();
};

#endif /* __MATH_CAMERA_HPP */