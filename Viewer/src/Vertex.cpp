#include "Vertex.h"


//ctors
Vertex::Vertex() { this->normal = glm::vec3(0, 0, 0); }
Vertex::Vertex(glm::vec3 p, glm::vec3 c, int d) : point(p), color(c), depth(d), normal(glm::vec3(0, 0, 0)) { }
Vertex::Vertex(glm::vec3 p, glm::vec3 c) : point(p), color(c), depth(UINT32_MAX), normal(glm::vec3(0, 0, 0)) { }

//dtor
Vertex::~Vertex()
{
}
