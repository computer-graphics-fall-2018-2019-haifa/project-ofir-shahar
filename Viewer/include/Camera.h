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
class Camera
{
private:
	glm::mat4x4 viewWorldTransform;
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;
	glm::mat4x4 orthographicTransformation;
	glm::mat4x4 scalingTransformation;
	glm::vec3 oldeye;
	glm::vec3 oldat;
	float zoom;
	bool isCurrent;


public:
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	~Camera();
	Camera();
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
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
	void setCurrent(const bool &b) { this->isCurrent = b; }
	bool getCurrent() const { return this->isCurrent; } 
	const glm::mat4 getViewTransformation();
	const glm::mat4 getProjectionTransformation();
	const glm::mat4 getOrthographicTransformation(); 
	const glm::mat4x4 getViewWorldTransform() const { return this->viewWorldTransform; }
	void setCameraViewWorldTransform(glm::vec4&);
};
