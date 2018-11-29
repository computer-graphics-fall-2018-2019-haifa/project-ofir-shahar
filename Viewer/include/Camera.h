#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"

/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */
class Camera : public MeshModel
{
private:
	glm::vec3 eye, at, up;
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;
	glm::mat4x4 orthographicTransformation;
	glm::mat4x4 scalingTransformation;
	glm::vec3 oldeye;
	glm::vec3 oldat;
	float zoom;

public:
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	~Camera();
	Camera();
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	glm::mat4x4 getProjectionTransformation(); 
	void scaleTransform(glm::vec3& vect);
	void SetOrthographicProjection(
		const float height,
		const float aspectRatio,
		const float near,
		const float far);

	void SetPerspectiveProjection(
		const float fovy,
		const float aspect,
		const float near,
		const float far);

	void SetZoom(const float zoom);
	const glm::mat4 getViewTransformation();
	const glm::mat4 getProjectionTformation();
	const glm::mat4 getOrthographicTransformation(); 

	void setCameraEye(const glm::vec3& v) { this->eye = v; }
	void setCameraAt(const glm::vec3& v) { this->at = v; }
	void setCameraUp(const glm::vec3& v) { this->up = v; }

	const glm::vec3& getCameraEye() const { return this->eye; }
	const glm::vec3& getCameraAt() const { return this->at; }
	const glm::vec3& getCameraUp() const { return this->up; }
};
