#pragma once
#include <glm/glm.hpp>
#include <math.h>

typedef enum LightType { ambient, specular, spot };

class Light
{

private:
	int type;
	bool isActive;
	float AmbientIntensity;
	float DifusiveIntensity;
	float SpecularIntensity;

	glm::vec3 pos;
	glm::vec3 color;

public:
	Light();
	~Light();

	//getters-setters
	void setPosition(const glm::vec3 &p) { this->pos = p; }
	void setColor(const glm::vec3 &c) { this->color = c; }
	void setActive(const bool &b) { this->isActive = b; }
	void setType(const int &i) { this->type = i; }

	float Ambient(const float &AmbientLightIntensity, const float &AmbientFraction)  { return AmbientLightIntensity * AmbientFraction; }
	float Difusive(const float &Kd, const float &Ld, glm::vec3 &l, glm::vec3 &n)  ;
	float Specular(const float &Ks, const float &r, const float &v, const int &alpha, const float &Ls)  ;
	float TotalLight() const ;
};
