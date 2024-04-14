
#include "math/camera.hpp"
#include "math/mat4.h"

Camera::Camera(){
	mat4_setIdentity(this->viewMatrix);
	this->frustum = false;
	this->setProjection(1, -1, 1, -1, 1, 50);
}

void Camera::updateProjection()
{
	if(this->frustum){
		mat4_setFrustum(this->projectionMatrix, this->right, this->left, this->top, this->bottom, this->near, this->far);
	}else{
		mat4_setOrtho(this->projectionMatrix, this->right, this->left, this->top, this->bottom, this->near, this->far);
	}
	this->updateViewProjectionMatrix();
}

void Camera::updateViewProjectionMatrix()
{
	mat4_mulmm(this->viewProjectionMatrix, this->projectionMatrix, this->viewMatrix);
}

void Camera::setProjection(float right, float left, float top, float bottom, float near, float far)
{
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->near = near;
	this->far = far;
	this->updateProjection();
}

void Camera::setViewport(float right, float left, float top, float bottom)
{
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->updateProjection();
}

void Camera::setNearFar(float near, float far)
{
	this->near = near;
	this->far = far;
	this->updateProjection();
}

void Camera::scaleViewport(float xscale, float yscale)
{
	this->left *= xscale;
	this->right *= xscale;
	this->top *= yscale;
	this->bottom *= yscale;
	this->updateProjection();
}

void Camera::rotateView(float ax, float ay, float az, float rad)
{
	float tmp[16];
	mat4_setRotateA(tmp, ax, ay, az, rad);
	mat4_mulmm(this->viewMatrix, this->viewMatrix, tmp);
	this->updateViewProjectionMatrix();
}

void Camera::translateView(float x, float y, float z)
{
	mat4_postTranslateA(this->viewMatrix, this->viewMatrix, x, y, z);
	this->updateViewProjectionMatrix();
}

void Camera::scaleView(float x, float y, float z){
	mat4_scaleA(this->viewMatrix, this->viewMatrix, x, y, z);
	this->updateViewProjectionMatrix();
}

void Camera::translateViewRelative(float x, float y, float z)
{
	mat4_translateA(this->viewMatrix, this->viewMatrix, x, y, z);
	this->updateViewProjectionMatrix();
}

void Camera::rotateViewRelative(float ax, float ay, float az, float rad)
{
	mat4_rotateA(this->viewMatrix, this->viewMatrix, ax, ay, az, rad);
	this->updateViewProjectionMatrix();
}

Ray3 *Camera::getWorldRay(float screenX, float screenY)
{
	float m[16];
	float a[3] = {screenX, screenY, -1};
	float b[3] = {screenX, screenY,  1};
	mat4_invert(m, this->viewProjectionMatrix);
	mat4_mulmv3_normalize(a, m, a);
	mat4_mulmv3_normalize(b, m, b);
	return Ray3::fromLine(a, b);
}