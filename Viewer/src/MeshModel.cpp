#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define PI 3.14159265358


//augmented ctor
MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<Vertex>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	localTransform(glm::mat4(1)),
	worldTransform(glm::mat4(1)),
	worldTranslation(glm::mat4(1)),
	worldRotation(glm::mat4(1)),
	scaleTransform(glm::mat4(1500)),
	rotationTransform(glm::mat4(1)),
	xRotation(glm::mat4(1)),
	yRotation(glm::mat4(1)),
	zRotation(glm::mat4(1)),
	xRotationWorld(glm::mat4(1)),
	yRotationWorld(glm::mat4(1)),
	zRotationWorld(glm::mat4(1)),
	translationTransform(glm::mat4(1)),
	faces(faces),
	vertexs(vertices),
	normals(normals)
{
	this->drawCube = false;
	this->isCurrentModel = false;
	this->createCube();
	this->createCenterLines();
	setScaleTransform(1500, 1500, 1500);
}
//ctor
MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	localTransform(glm::mat4(1)),
	worldTransform(glm::mat4(1)),
	worldTranslation(glm::mat4(1)),
	worldRotation(glm::mat4(1)),
	scaleTransform(glm::mat4(1500)),
	rotationTransform(glm::mat4(1)),
	xRotation(glm::mat4(1)),
	yRotation(glm::mat4(1)),
	zRotation(glm::mat4(1)),
	xRotationWorld(glm::mat4(1)),
	yRotationWorld(glm::mat4(1)),
	zRotationWorld(glm::mat4(1)),
	translationTransform(glm::mat4(1)),
	faces(faces),
	vertices(vertices),
	normals(normals)
{
	this->drawCube = false;
	this->isCurrentModel = false;
	this->createCube();
	this->createCenterLines();
	setScaleTransform(1500, 1500, 1500);
}

MeshModel::~MeshModel()
{

}

void MeshModel::createCenterLines()
{
	for (std::vector<Face>::iterator f = this->faces.begin(); f != this->faces.end(); f++)
	{ 
		glm::vec3 point1 = this->vertexs.at((*f).GetVertexIndex(0) - 1).getPoint(); 
		glm::vec3 point2 = this->vertexs.at((*f).GetVertexIndex(1) - 1).getPoint();
		glm::vec3 point3 = this->vertexs.at((*f).GetVertexIndex(2) - 1).getPoint();
		glm::vec3 center = glm::vec3((point1.x + point2.x + point3.x)/3, (point1.y + point2.y + point3.y)/3, (point1.z + point2.z + point3.z)/3); 
			
		f->SetCenter(center);
	}
}

void MeshModel::createCube()
{
	std::cout << "cube created" << std::endl;
	this->cube.back = this->cube.front = this->cube.right = this->cube.left = this->cube.top = this->cube.bottom = 0; 
	this->cube.color = glm::vec4(1, 0, 0, 1); 

	//find minimums and maximums of the cube faces
	for (std::vector<glm::vec3>::iterator it = this->vertices.begin(); it != this->vertices.end(); it++)
	{
		if (this->cube.back >= (*it).z)  this->cube.back = (*it).z; 
		if (this->cube.front < (*it).z)  this->cube.front = (*it).z;

		if (this->cube.right <= (*it).x) this->cube.right = (*it).x;
		if (this->cube.left > (*it).x) this->cube.left = (*it).x;

		if (this->cube.bottom >= (*it).y) this->cube.bottom = (*it).y;
		if (this->cube.top < (*it).y) this->cube.top = (*it).y;
	}

	//calculate the cube corners verices
	this->cube.cPoints[0] = glm::vec4(this->getCube().left, this->getCube().bottom, this->getCube().front, 1);
	this->cube.cPoints[1] = glm::vec4(this->getCube().right, this->getCube().bottom, this->getCube().front, 1);
	this->cube.cPoints[2] = glm::vec4(this->getCube().left, this->getCube().top, this->getCube().front, 1);
	this->cube.cPoints[3] = glm::vec4(this->getCube().right, this->getCube().top, this->getCube().front, 1);
	this->cube.cPoints[4] = glm::vec4(this->getCube().left, this->getCube().bottom, this->getCube().back, 1);
	this->cube.cPoints[5] = glm::vec4(this->getCube().right, this->getCube().bottom, this->getCube().back, 1);
	this->cube.cPoints[6] = glm::vec4(this->getCube().left, this->getCube().top, this->getCube().back, 1);
	this->cube.cPoints[7] = glm::vec4(this->getCube().right, this->getCube().top, this->getCube().back, 1);
	
}

void MeshModel::createNormals()
{
	glm::vec3 lines[2];
	for (std::vector<glm::vec3>::iterator it = this->vertices.begin(); it != this->normals.end(); it++)
	{

	}
}

void MeshModel::SetWorldTransformation(const glm::mat4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4& MeshModel::getTranslationTransform() const {
	return this->translationTransform;
}


const glm::mat4& MeshModel::GetWorldTransformation() const
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

const glm::mat4& MeshModel::GetRotationTransform() const {
	return this->rotationTransform;
}
const glm::mat4& MeshModel::GetTranslationTransform() const {
	return this->translationTransform;
}

const glm::mat4& MeshModel::GetWorldTranslate() const {
	return this->worldTranslation;
}
const glm::mat4& MeshModel::GetWorldRotation() const {
	return this->worldRotation;
}

void MeshModel::setScaleTransform(float xFactor, float yFactor, float zFactor) {
	glm::vec4 xVec(xFactor, 0, 0, 0);
	glm::vec4 yVec(0, yFactor, 0, 0);
	glm::vec4 zVec(0, 0, zFactor, 0);
	glm::vec4 lastVec(0, 0, 0, 1);
	scaleTransform = glm::mat4(xVec, yVec, zVec, lastVec);
	localTransform = this->translationTransform * (this->rotationTransform * this->scaleTransform);
}
void MeshModel::setTranslationTransform(float x, float y, float z) {
	glm::vec4 xVec = translationTransform[0];
	glm::vec4 yVec = translationTransform[1];
	glm::vec4 zVec = translationTransform[2];
	glm::vec4 lVec(x, y, z, 1);
	translationTransform = glm::mat4(xVec, yVec, zVec, lVec);
	localTransform = this->translationTransform * (this->rotationTransform * this->scaleTransform);
}

void MeshModel::setWorldTranslation(float x, float y, float z) {
	glm::vec4 xVec = worldTranslation[0];
	glm::vec4 yVec = worldTranslation[1];
	glm::vec4 zVec = worldTranslation[2];
	glm::vec4 lVec(x, y, z, 1);
	worldTranslation = glm::mat4(xVec, yVec, zVec, lVec);
	worldTransform = this->worldTranslation * this->worldRotation;
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
		localTransform = this->translationTransform * (this->rotationTransform * this->scaleTransform);
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
		localTransform = this->translationTransform*(this->rotationTransform * this->scaleTransform);
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
		localTransform = this->translationTransform*(this->rotationTransform * this->scaleTransform);
	}
}

void MeshModel::setWorldRotation(float xDegree, float yDegree, float zDegree) {
	if (yDegree == 1 && zDegree == 1) {
		auto rad = xDegree * PI / 180;
		glm::vec4 xVec(1, 0, 0, 0);
		glm::vec4 yVec(0, cos(rad), sin(rad), 0);
		glm::vec4 zVec(0, -sin(rad), cos(rad), 0);
		glm::vec4 lastVec(0, 0, 0, 1);
		xRotationWorld = glm::mat4(xVec, yVec, zVec, lastVec);
		this->worldRotation = this->yRotationWorld*xRotationWorld;
		this->worldRotation = this->zRotationWorld*worldRotation;
		worldTransform = this->worldTranslation * this->worldRotation;
	}
	else if (xDegree == 1 && zDegree == 1) {
		auto rad = yDegree * PI / 180;
		glm::vec4 xVec(cos(rad), 0, sin(rad), 0);
		glm::vec4 yVec(0, 1, 0, 0);
		glm::vec4 zVec(-sin(rad), 0, cos(rad), 0);
		glm::vec4 lastVec(0, 0, 0, 1);
		yRotationWorld = glm::mat4(xVec, yVec, zVec, lastVec);
		this->worldRotation = this->yRotationWorld*xRotationWorld;
		this->worldRotation = this->zRotationWorld*worldRotation;
		worldTransform = this->worldTranslation * this->worldRotation;
	}
	else if (xDegree == 1 && yDegree == 1) {
		auto rad = zDegree * PI / 180;
		glm::vec4 xVec(cos(rad), sin(rad), 0, 0);
		glm::vec4 yVec(-sin(rad), cos(rad), 0, 0);
		glm::vec4 zVec(0, 0, 1, 0);
		glm::vec4 lastVec(0, 0, 0, 1);
		zRotationWorld = glm::mat4(xVec, yVec, zVec, lastVec);
		this->worldRotation = this->yRotationWorld*xRotationWorld;
		this->worldRotation = this->zRotationWorld*worldRotation;
		worldTransform = this->worldTranslation * this->worldRotation;
	}
}
const glm::mat4& MeshModel::GetScaleTransform() const {
	return this->scaleTransform;
}
MeshModel::MeshModel() {
}