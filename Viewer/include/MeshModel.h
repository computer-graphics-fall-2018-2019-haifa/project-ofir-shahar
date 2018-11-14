#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <memory>
#include "Face.h"

typedef struct Cube {
	float front, back, right, left, top, bottom;
	glm::vec4 cPoints[8];  
	glm::vec4 fbl, fbr, ftl, ftr, bbl, bbr, btl, btr;
	glm::vec4 color; 
}Cube;

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 */
class MeshModel
{
private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4 localTransform;
	glm::mat4 scaleTransform;
	glm::mat4 rotationTransform;
	glm::mat4 translationTransform, worldTranslation, xRotationWorld, yRotationWorld, zRotationWorld;

	glm::mat4 worldRotation;
	glm::mat4 xRotation;
	glm::mat4 yRotation;
	glm::mat4 zRotation;
	glm::mat4x4 worldTransform;
	glm::vec4 color;
	std::string modelName;
	bool drawCube;
	void createCube();
	Cube cube; 


public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();
	MeshModel();
	void SetWorldTransformation(const glm::mat4& worldTransform);
	const glm::mat4& GetWorldTransformation() const;
	const glm::mat4& GetLocalTransform() const;
	const glm::mat4& GetScaleTransform() const;
	const glm::mat4& GetTranslationTransform() const;
	const glm::mat4& GetRotationTransform() const;
	const glm::mat4& GetWorldTranslate() const;
	const glm::mat4& GetWorldRotation() const;
	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);

	const std::string& GetModelName();

	const std::vector<Face>& GetFaces();

	const std::vector<glm::vec3>& GetVertices();

	const std::vector<glm::vec3>& GetNormals();

	void setScaleTransform(float xFactor, float yFactor, float zFactor);

	void setRotationTransform(float xDegree, float yDegree, float zDegree);

	void setTranslationTransform(float x, float y, float z);

	void setWorldTranslation(float x, float y, float z);

	void setWorldRotation(float xDegree, float yDegree, float zDegree);

	const glm::mat4& getTranslationTransform() const;
	
	const Cube getCube() const { return this->cube; }

	void setCube(const Cube c) { this->cube = c; }

	const bool getDrawCube() { return this->drawCube; }

	void setDrawCube(const bool b) { this->drawCube = b; }

	void AdjustCube(const glm::mat4 scale, glm::mat4 rotate, glm::mat4 translate, glm::mat4 wrotate, glm::mat4 wtranslate, glm::mat4 cview, glm::mat4 cproj);



	void PrintCube(Cube cube) { std::cout << "front=" << cube.front << " back=" << cube.back << " right=" << cube.right << " left=" << cube.left << " top=" << cube.top << " bottom=" << cube.bottom << std::endl; }
};
