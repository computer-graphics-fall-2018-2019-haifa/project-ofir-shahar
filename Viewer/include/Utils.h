#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"

/*
 * Utils class.
 * This class is consisted of static helper methods that can have many clients across the code.
 */
class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static MeshModel LoadMeshModel(const std::string& filePath);

	static void createGrid();

	static glm::vec4 Centeroid(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3); 

private:
	static std::string GetFileName(const std::string& filePath);
};
