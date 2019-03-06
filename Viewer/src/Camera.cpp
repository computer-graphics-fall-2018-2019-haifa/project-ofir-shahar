#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

#define PI 3.14159265358

//ctor
Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) :
	zoom(1.0)
{
	//SetPerspectiveProjection(45, 1, 200, 500);
	oldeye = eye;
	oldat = at;
	FOV = 45.0f;
	aspectRatio = 0.5f;
	_near = 100;
	_far = 500;
	SetCameraLookAt(eye, at, up);
	SetPerspectiveProjection(FOV , aspectRatio , _near, _far);
	setCameraViewWorldTransform(glm::vec4(0, 0, 0, 1)); 
}

Camera::~Camera()
{
}
Camera::Camera()
{
	//SetPerspectiveProjection(45, 280, 1, 10);
	SetCameraLookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
}

const glm::mat4 Camera::getViewTransformation() {
	return viewTransformation;
}
const glm::mat4 Camera::getProjectionTransformation()
{
	return this->projectionTransformation;
}
const glm::mat4 Camera::getOrthographicTransformation()
{
	return this->orthographicTransformation;
}
void Camera::setCameraViewWorldTransform(glm::vec4 &v)
{
	this->viewWorldTransform = glm::mat4x4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), v); 
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 z = glm::normalize(eye - at);
	glm::vec3 x = glm::normalize(glm::cross(up, z));
	glm::vec3 y = glm::normalize(glm::cross(z, x));

	glm::vec4 t = glm::vec4(0, 0, 0, 1);
	glm::vec4 x4 = glm::vec4(x, 0);
	glm::vec4 y4 = glm::vec4(y, 0);
	glm::vec4 z4 = glm::vec4(z, 0);

	glm::mat4 c(x4, y4, z4, t);
	glm::mat4 iden(1);
	iden[3] = iden[3] - glm::vec4(eye,0);

	this->viewTransformation = c * iden;

}

//Function create the Normalized projection matrix by (P=ST)
//1. T = translate the center of the volume to the origin
//2. S = Scale to the volume of unit cube
void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{
	glm::vec4 v1, v2, v3, v4;
	float right, left, bottom, top, width = height * aspectRatio;

	right = 0.5 * width;
	left = -0.5 * width;
	top = 0.5 * height;
	bottom = -0.5 * height;

	v1 = glm::vec4(2 / (right - left), 0, 0, 0);
	v2 = glm::vec4(0, 2 / (top - bottom), 0, 0);
	v3 = glm::vec4(0, 0, 2 / (near - far), 0);
	v4 = glm::vec4(-(right + left) / (right - left), -(top + bottom) /(top - bottom), -(far + near) / (far - near), 1);

	this->orthographicTransformation = glm::mat4(v1, v2, v3, v4);
}

void Camera::SetPerspectiveProjection(float &fovy, float &aspectRatio, float &near, float &far)
{
	float nearHeight = 2 * near * tan(0.5*fovy * PI / 180);
	float nearWidth = aspectRatio * nearHeight;
	float t = 0.5 * nearHeight;
	float b = -0.5 * nearHeight;
	float l = -0.5 * nearWidth;
	float r = 0.5 * nearWidth;
	if (near == far || t == b) return;
	std::cout << "near " << near << std::endl;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
			std::cout << this->projectionTransformation[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	glm::vec4 v1(2 * near / (r - l), 0, (r + l) / (r - l), 0);
	glm::vec4 v2(0, 2 * near / (t - b), (t + b) / (t - b), 0);
	glm::vec4 v3(0, 0, -1 * (far + near) / (far - near), -2 * (far * near) / (far - near)); 
	glm::vec4 v4(0, 0, -2 *( far * near )/ (far - near), 0);
	this->projectionTransformation = glm::mat4(v1, v2, v3, v4);
	this->projectionTransformation = glm::mat4(glm::vec4(2 * near / (r - l), 0, (r + l) / (r - l), 0),
											   glm::vec4(0, 2 * near / (t - b), (t + b) / (t - b), 0),
		                                       glm::vec4(0, 0, -1 * (far + near) / (far - near), -2 * (far * near) / (far - near)),
		                                       glm::vec4(0, 0, -1, 0));
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
			std::cout << this->projectionTransformation[i][j] << " ";
		std::cout << std::endl;
	}
}

void Camera::SetZoom(const float zoom)
{

}

void Camera::scaleTransform(glm::vec3& vect) {
	vect.x = vect.x * 50;
	vect.x = vect.y * 50;
}

