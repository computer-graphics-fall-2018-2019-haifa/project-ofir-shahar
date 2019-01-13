#include "Vertex.h"


//ctors
Vertex::Vertex() {}
Vertex::Vertex(glm::vec3 p, glm::vec3 c, int d) : point(p), color(c), depth(d) {}

//dtor
Vertex::~Vertex()
{
}
