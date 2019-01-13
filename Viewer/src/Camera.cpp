#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

#define PI 3.14159265358

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) :
	zoom(1.0)
{
	//SetPerspectiveProjection(45, 1, 200, 500);
	oldeye = eye;
	oldat = at;
	SetCameraLookAt(eye, at, up);
	SetPerspectiveProjection(45,0.5 , 100, 1000);

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
const glm::mat4 Camera::getProjectionTformation()
{
	return this->projectionTransformation;
}
const glm::mat4 Camera::getOrthographicTransformation()
{
	return this->orthographicTransformation;
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

	//make the vectors be homogenous
	//glm::vec4 eye4 = glm::vec4(eye.x, eye.y, eye.z, 0);
	//glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 0);
	//glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 0);
	////calculate vector z which is the new z axis
	//glm::vec4 z = glm::normalize(eye4 - at4);
	////calculate u by cross product n with our "up" vector u will be the x axis
	//glm::vec3 crossVecUpN = glm::cross(up, z));

	//calculate the normalized camera view
	/*glm::vec3 z = glm::vec3(glm::normalize(eye - at));
	glm::vec3 x = glm::vec3(glm::normalize(glm::cross(up, z)));
	glm::vec3 y = glm::vec3(glm::normalize(glm::cross(z, x)));*/

	//expand to 4 coordinates vectors
	/*glm::vec4 t = glm::vec4(-glm::dot(eye, x), -glm::dot(eye, y), -glm::dot(eye, z), 1.0);
	glm::vec4 x4 = glm::vec4(x, 0.0);
	glm::vec4 y4 = glm::vec4(y, 0.0);
	glm::vec4 z4 = glm::vec4(z, 0.0);*/

	//create the c matrix
	//glm::mat4 c(x4, y4, z4, t);
	//c = glm::transpose(c);

	//glm::mat4 iden(1);
	//iden[3] = iden[3] - glm::vec4(eye.x, eye.y, eye.z, 1);

	//this->viewTransformation = c * iden;
	//this->viewTransformation = c;
	////make the vectors be homogenous
	//glm::vec4 eye4 = glm::vec4(eye.x, eye.y, eye.z, 0);
	//glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 0);
	//glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 0);
	////calculate vector n which is the new z axis
	//glm::vec4 n = glm::normalize(eye4 - at4);
	////calculate u by cross product n with our "up" vector u will be the x axis
	//glm::vec3 crossVecUpN = glm::cross(up, glm::vec3(n.x, n.y, n.z));

	//glm::vec4 cvun = glm::vec4(crossVecUpN.x, crossVecUpN.y, crossVecUpN.z, 0);
	//glm::vec4 u = glm::normalize(cvun);
	////calculate v by cross product n with u. v will be the new y axis. all vectors are normalized
	//glm::vec3 crossVecNU = glm::cross(glm::vec3(n.x, n.y, n.z), glm::vec3(u.x, u.y, u.z));
	//glm::vec4 cvnu = glm::vec4(crossVecNU.x, crossVecNU.y, crossVecNU.z, 0);
	//glm::vec4 v = glm::normalize(cvnu);
	//glm::vec4 t = glm::vec4(0.0, 0.0, 0.0, 1.0);
	////this creates our c matrix which actually rotated our camera to how it is.
	////we transpose it to turn it back.
	//glm::mat4 c = glm::mat4(u, v, n, t);
	//c = glm::transpose(c);
	////then we create translation matrix to move camera to origin.
	//glm::mat4 IdenMinusEye = glm::mat4(1);
	//IdenMinusEye[3] = IdenMinusEye[3] - eye4;
	////multiply matrices and return the output as viewTransformation
	//this->viewTransformation = c * IdenMinusEye;

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

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	float nearHeight = 2*near * tan(0.5*fovy * PI / 180);
	//float nearHeight = 2*near * tan(0.5*fovy * PI / 180);
	float num = 1 / tan(0.5 * fovy * PI / 180);
	float nearWidth = aspectRatio * nearHeight;
	float t = 0.5 * nearHeight;
	float b = -0.5 * nearHeight;
	float l = -0.5 * nearWidth;
	float r = 0.5 * nearWidth;
	glm::vec4 v1(near / r, 0, 0, 0);
	glm::vec4 v2(0, near / t, 0, 0);
	glm::vec4 v3(0, 0, -1 * (far + near) / (far - near), -1);
	glm::vec4 v4(0, 0, -2 *( far * near )/ (far - near), 0);
	this->projectionTransformation = glm::mat4(v1, v2, v3, v4);
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
