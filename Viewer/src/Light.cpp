#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

float Light::TotalLight() const
{
	return this->DifusiveIntensity + this->AmbientIntensity + this->SpecularIntensity;
}

float Light::Difusive(const float & Kd, const float & Ld, glm::vec3 &l, glm::vec3 &n) 
{
	return DifusiveIntensity = Kd * glm::dot(l, n) * Ld;
}

float Light::Specular(const float & Ks, const float & r, const float & v, const int & alpha, const float & Ls) 
{
	return SpecularIntensity = Ks * pow(glm::dot(r,v), alpha) * Ls;
}
