#pragma once
#include <glm/glm.hpp>
#include <random>

extern glm::vec3 environment;

extern std::mt19937 gen;

inline float rvalue() {
	return (float)rand() / RAND_MAX;
}

inline float rnvalue() {
	float theta = 2 * 3.1415926 * rvalue();
	float rho = sqrt(-2*log(rvalue()));
	return rho * cos(theta);
}

inline glm::vec3 rdirection() {
	return { rnvalue(),rnvalue(),rnvalue() };
}

struct Material {
	glm::vec3 color;
	float light_power;
	float smoothness;
	float appleness;

	Material(glm::vec3 color, float light_power, float smoothness, float appleness)  :
		color(color),
		light_power(light_power),
		smoothness(smoothness),
		appleness(appleness)
	{

	}
};
