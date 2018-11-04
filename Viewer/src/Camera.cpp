#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

#define PI 3.14159265352 

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) :
	zoom(1.0)
{
	SetCameraLookAt(eye, at, up);
}

Camera::~Camera()
{
}
Camera::Camera()
{
	SetPerspectiveProjection(45, 2, 200, 500); 
	SetCameraLookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
}

glm::mat4 Camera::getViewTransformation() {
	return viewTransformation;
}
glm::mat4 Camera::getProjectionTformation()
{
	return glm::mat4();
}
void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	//make the vectors be homogenous
	glm::vec4 eye4 = glm::vec4(eye.x, eye.y, eye.z, 0);
	glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 0);
	glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 0);
	//calculate vector n which is the new z axis
	glm::vec4 n = glm::normalize(eye4 - at4);
	//calculate u by cross product n with our "up" vector u will be the x axis
	glm::vec3 crossVecUpN = glm::cross(up, glm::vec3(n.x, n.y, n.z));
	glm::vec4 cvun = glm::vec4(crossVecUpN.x, crossVecUpN.y, crossVecUpN.z, 0);
	glm::vec4 u = glm::normalize(cvun);
	//calculate v by cross product n with u. v will be the new y axis. all vectors are normalized
	glm::vec3 crossVecNU = glm::cross(glm::vec3(n.x, n.y, n.z), glm::vec3(u.x, u.y, u.z));
	glm::vec4 cvnu = glm::vec4(crossVecNU.x, crossVecNU.y, crossVecNU.z, 0);
	glm::vec4 v = glm::normalize(cvnu);
	glm::vec4 t = glm::vec4(0.0, 0.0, 0.0, 1.0);
	//this creates our c matrix which actually rotated our camera to how it is.
	//we transpose it to turn it back.
	glm::mat4 c = glm::mat4(u, v, n, t);
	c = glm::transpose(c);
	//then we create translation matrix to move camera to origin.
	glm::mat4 IdenMinusEye = glm::mat4(1);
	IdenMinusEye[3] = IdenMinusEye[3] - eye4;
	//multiply matrices and return the output as viewTransformation
	this->viewTransformation = c * IdenMinusEye;

}

void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{

}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	float nearHeight = 2 * near * _CMATH_::tanf(fovy * PI / 180); 
	float nearWidth = aspectRatio * nearHeight; 
	float t = 0.5 * nearHeight;
	float b = -0.5 * nearHeight;
	float l = -0.5 * nearWidth;
	float r = 0.5 * nearWidth;
	glm::vec4 v1(2 * near / (r - l), 0, 0, 0);
	glm::vec4 v2(0, 2 * near / (t - b), 0, 0);
	glm::vec4 v3((r + l) / (r - l), (t + b) / (t - b), -1 * (far + near) / (far - near), -1);
	glm::vec4 v4(0, 0, -2 * far * near / (far - near), 0);  
	this->projectionTransformation = glm::mat4x4(v1, v2, v3, v4); 
}

void Camera::SetZoom(const float zoom)
{

}

void Camera::scaleTransform(glm::vec3& vect) {
	vect.x = vect.x * 50;
	vect.x = vect.y * 50;
}

glm::mat4x4 Camera::getProjectionTransformation() {
	return this->projectionTransformation; 
}