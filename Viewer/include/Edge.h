#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
class Edge
{
private:
	glm::vec2 P1, P2;
	glm::vec3 color;

public:
	Edge();
	Edge(glm::vec2 p1, glm::vec2 p2, glm::vec3 color);
	~Edge() {}

	glm::vec2 getP1() const { return this->P1; }
	glm::vec2 getP2() const { return this->P2; }
};
