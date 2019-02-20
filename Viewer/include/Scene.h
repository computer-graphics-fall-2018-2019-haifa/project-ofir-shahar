#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"
#include "Light.h"

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<Light> Lights;
	std::vector<Camera> cameras;

	int activeCameraIndex;
	int activeModelIndex;

public:
	Scene();
	~Scene(){}
	void AddModel(const std::shared_ptr<MeshModel>& model);
	void addLight(const Light &l) { this->Lights.push_back(l); }

	const int GetModelCount() const;

	void AddCamera(const Camera& camera);
	const int GetCameraCount() const;

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;

	const std::vector<std::shared_ptr<MeshModel>> getModels() const;

	const std::vector<Camera> getCameras() const;
	
	const std::vector<Light> getLights() const { return this->Lights; }
};
