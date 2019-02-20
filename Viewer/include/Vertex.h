#pragma once

#include <glm/glm.hpp>
#include <vector>

class Vertex
{
private:
	glm::vec3 point;
	glm::vec3 color;
	glm::vec3 normal;
	int depth;
	std::vector<glm::vec3> normals;
public:
	Vertex();
	Vertex(glm::vec3 p, glm::vec3 c, int d);
	Vertex::Vertex(glm::vec3, glm::vec3);
	~Vertex();


	glm::vec3 getPoint() const { return this->point; }
	void setPoint(const glm::vec3 &p) { this->point = p; }

	glm::vec3 getColor() const { return this->color; }
	void setColor(const glm::vec3 &c) { this->color = c; }

	int getDepth() const { return this->depth; }
	void setPoint(const int &d) { this->depth = d; }

	void addNormal(glm::vec3 &v) { this->normal += v; this->normals.push_back(v); }
	glm::vec3 getNormal() const { return this->normal; }
	void setNormal(const glm::vec3 &v) {  this->normal = v; }
	
};
