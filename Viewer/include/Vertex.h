#pragma once
#include <stdio.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Vertex
{
private:
	int depth;
	glm::vec3 point;
	glm::vec3 color;
	std::vector<glm::vec3> vertexVectors;
public:
	Vertex();
	Vertex(glm::vec3 p, glm::vec3 c, int d);
	~Vertex();

	//class methods
	void addNormal(const glm::vec3 &v) { this->vertexVectors.push_back(v); }
	void print() const { std::cout << "vertex x:" << this->point.x << " y:" << this->point.y << " z:" << this->point.z << std::endl; }

	//getters
	glm::vec3 getPoint() const { return this->point; }
	int getDepth() const { return this->depth; }
	glm::vec3 getColor() const { return this->color; }
	std::vector<glm::vec3> getVectors() const { return this->vertexVectors; }

	//setters
	void setDepth(const int &d) { this->depth = d; }
	void setPoint(const glm::vec3 &p) { this->point = p; }
	void setColor(const glm::vec3 &c) { this->color = c; }

};