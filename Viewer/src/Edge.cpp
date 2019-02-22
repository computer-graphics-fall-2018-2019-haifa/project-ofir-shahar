#include "Edge.h"

//ctors
Edge::Edge() {}
Edge::Edge(glm::vec2 p1, glm::vec2 p2, glm::vec3 color) : color(color)
{
	P1.setPointx(p1.x);
	P1.setPointy(p1.y);
	P2.setPointx(p2.x);
	P2.setPointy(p2.y);

	if (P1.getPoint().y < P2.getPoint().y)
	{
		P1.setPointx(p1.x);
		P1.setPointy(p1.y);
		P2.setPointx(p2.x);
		P2.setPointy(p2.y);
	}
	else //y1 >= y2
	{
		P1.setPointx(p2.x);
		P1.setPointy(p2.y);
		P2.setPointx(p1.x);
		P2.setPointy(p1.y);
	}
}