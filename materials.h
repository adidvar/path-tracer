#pragma once
#include <glm/glm.hpp>
#include <random>

extern glm::vec3 environment;

extern std::mt19937 gen;

class Material {
public:
	glm::vec3 color;
	bool lighting;
	float dullness;

	Material(glm::vec3 color, bool lighting, float dullness) :
		color(color),
		lighting(lighting),
		dullness(dullness)
	{

	}

	glm::vec3 reflect(glm::vec3 n, glm::vec3 vec) {
		vec = glm::reflect(vec, n);
		glm::vec3 random(rand()-RAND_MAX/2, rand()-RAND_MAX/2, rand()-RAND_MAX/2);
		random = glm::normalize(random);

		if(glm::dot(random,n) < 0.0)
			random = -random;

		return glm::normalize((1-dullness)*vec + random * dullness);
	}
	glm::vec3 Color(glm::vec3 lightness) 
	{
		return glm::vec3(lightness.x * color.x, lightness.y * color.y, lightness.z * color.z);
	}
	bool is_light() {
		return lighting;
	};
	glm::vec3 Light() {
		return color;
	}
};
