#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Vertex.h"

class Edge
{
private:
	Vertex P1, P2;
	glm::vec3 color;

public:
	Edge();
	Edge(glm::vec2 p1, glm::vec2 p2, glm::vec3 color);
	~Edge() {}

	glm::vec2 getP1() const { return glm::vec2(this->P1.getPoint().x, this->P1.getPoint().y); }
	glm::vec2 getP2() const { return glm::vec2(this->P2.getPoint().x, this->P2.getPoint().y); }
};
