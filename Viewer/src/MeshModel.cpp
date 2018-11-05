#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define PI 3.14159265358
MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	localTransform(glm::mat4(1)),
	worldTransform(glm::mat4x4(1)),
	scaleTransform(glm::mat4(1)),
	rotationTransform(glm::mat4(1)),
	xRotation(glm::mat4(1)),
	yRotation(glm::mat4(1)),
	zRotation(glm::mat4(1)),
	translationTransform(glm::mat4(1)),
	faces(faces),
	vertices(vertices),
	normals(normals)
{

}

MeshModel::~MeshModel()
{

}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const
{
	return worldTransform;
}

const glm::mat4& MeshModel::GetLocalTransform() const
{
	return localTransform;
}
void MeshModel::SetColor(const glm::vec4& color)
{
	this->color = color;
}

const glm::vec4& MeshModel::GetColor() const
{
	return color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}

const std::vector<Face>& MeshModel::GetFaces() {
	return this->faces;
}

const std::vector<glm::vec3>& MeshModel::GetVertices() {
	return this->vertices;
}

const std::vector<glm::vec3>& MeshModel::GetNormals() {
	return this->normals;
}

void MeshModel::setScaleTransform(float xFactor, float yFactor, float zFactor) {
	glm::vec4 xVec(xFactor, 0, 0, 0);
	glm::vec4 yVec(0, yFactor, 0, 0);
	glm::vec4 zVec(0, 0, zFactor, 0);
	glm::vec4 lastVec(0, 0, 0, 1);
	this->scaleTransform = glm::mat4(xVec, yVec, zVec, lastVec);
	localTransform = this->rotationTransform * this->scaleTransform;
}

void MeshModel::setRotationTransform(float xDegree, float yDegree, float zDegree) {
	if (yDegree == 1 && zDegree == 1) {
		auto rad = xDegree * PI / 180;
		glm::vec4 xVec(1, 0, 0, 0);
		glm::vec4 yVec(0, cos(rad), sin(rad), 0);
		glm::vec4 zVec(0, -sin(rad), cos(rad), 0);
		glm::vec4 lastVec(0, 0, 0, 1);
		xRotation = glm::mat4(xVec, yVec, zVec, lastVec);
		this->rotationTransform = this->yRotation*xRotation;
		this->rotationTransform = this->zRotation*rotationTransform;
		localTransform = this->rotationTransform * this->scaleTransform;
	}
	else if (xDegree == 1 && zDegree == 1) {
		auto rad = yDegree * PI / 180;
		glm::vec4 xVec(cos(rad), 0, sin(rad), 0);
		glm::vec4 yVec(0, 1, 0, 0);
		glm::vec4 zVec(-sin(rad), 0, cos(rad), 0);
		glm::vec4 lastVec(0, 0, 0, 1);
		yRotation = glm::mat4(xVec, yVec, zVec, lastVec);
		this->rotationTransform = this->yRotation*xRotation;
		this->rotationTransform = this->zRotation*rotationTransform;
		localTransform = this->rotationTransform * this->scaleTransform;
	}
	else if (xDegree ==1 && yDegree == 1) {
		auto rad = zDegree * PI / 180;
		glm::vec4 xVec(cos(rad), sin(rad), 0, 0);
		glm::vec4 yVec(-sin(rad), cos(rad), 0, 0);
		glm::vec4 zVec(0, 0, 1, 0);
		glm::vec4 lastVec(0, 0, 0, 1);
		zRotation = glm::mat4(xVec, yVec, zVec, lastVec);
		this->rotationTransform = this->yRotation*xRotation;
		this->rotationTransform = this->zRotation*rotationTransform;
		localTransform = this->rotationTransform * this->scaleTransform;
	}
}
MeshModel::MeshModel() {

}