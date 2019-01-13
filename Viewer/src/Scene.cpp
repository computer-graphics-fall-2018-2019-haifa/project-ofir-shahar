#include "Scene.h"
#include "MeshModel.h"
#include "Utils.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(0),
	activeModelIndex(0)
{
	Utils::createGrid();
	AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel("../Data/obj_examples/grid.obj")));
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model)
{
	models.push_back(model);
}

const int Scene::GetModelCount() const
{
	return models.size();
}

void Scene::AddCamera(const Camera& camera)
{
	cameras.push_back(camera);
}

const int Scene::GetCameraCount() const
{
	return cameras.size();
}

void Scene::SetActiveCameraIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < cameras.size())
	{
		activeCameraIndex = index;
	}
}

const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}

void Scene::SetActiveModelIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < models.size())
	{
		activeModelIndex = index;
	}
}

const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}

const std::vector<std::shared_ptr<MeshModel>> Scene::getModels() const{
	return this->models;
}

const std::vector<Camera> Scene::getCameras() const {
	return this->cameras;
}