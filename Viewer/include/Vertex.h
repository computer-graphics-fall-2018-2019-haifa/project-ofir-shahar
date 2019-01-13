#pragma once

#include <glm/glm.hpp>

class Vertex
{
private:
	glm::vec3 point;
	glm::vec3 color;
	int depth;
public:
	Vertex();
	Vertex(glm::vec3 p, glm::vec3 c, int d);
	~Vertex();
	glm::vec3 getPoint() const { return this->point; }
	void setPoint(const glm::vec3 &p) { this->point = p; }

	glm::vec3 getColor() const { return this->color; }
	void setColor(const glm::vec3 &c) { this->color = c; }

	int getDepth() const { return this->depth; }
	void setPoint(const int &d) { this->depth = d; }
};