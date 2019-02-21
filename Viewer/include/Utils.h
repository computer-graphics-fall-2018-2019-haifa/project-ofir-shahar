#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"
#include "Vertex.h"

/*
 * Utils class.
 * This class is consisted of static helper methods that can have many clients across the code.
 */
class Utils
{
public:
	static glm::vec3 barycentric(glm::vec3 &point, std::vector<glm::vec3> &triangle); 
	static float zDepth(glm::vec3 &point, std::vector<glm::vec3> &face);
	static Vertex VertexFromStream(std::istream& issLine);
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static MeshModel LoadMeshModel(const std::string& filePath);
	static const std::shared_ptr<MeshModel> LoadMeshModelPtr(const std::string& filePath); 

	static glm::vec4 Centeroid(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3); 
	static glm::vec3 interpolate(glm::vec3 p1, glm::vec3 p2, float param) { return param * p1 + (1 - param) * p2; } 

private:
	static std::string GetFileName(const std::string& filePath);
};
