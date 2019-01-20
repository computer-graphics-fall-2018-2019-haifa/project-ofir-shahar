#include "Edge.h"

//ctors
Edge::Edge(){}
Edge::Edge(glm::vec2 p1, glm::vec2 p2, glm::vec3 color) : P1(p1), P2(p2), color(color) 
{
	if (P1.y < P2.y)
	{
		P1.x = p1.x;
		P1.y = p1.y;
		P2.x = p2.x;
		P2.y = p2.y;
	}
	else //y1 >= y2
	{
		P1.x = p2.x;
		P1.y = p2.y;
		P2.x = p1.x;
		P2.y = p1.y;
	}
}