#pragma once
#include <glm/glm.hpp>

typedef enum LightTYpe { ambient, specular, spot };

class Light
{

private:
	int type;
	bool isActive;
	glm::vec3 pos;
	glm::vec3 color;

public:
	Light();
	~Light();

	//getters-setters
	void setPosition(const glm::vec3 &p) { this->pos = p; }
	void setColor(const glm::vec3 &c) { this->color = c; }
	void setActive(const bool &b) { this->isActive = b; }

};
