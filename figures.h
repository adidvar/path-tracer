#pragma once
#include <glm/glm.hpp>
#include "materials.h"

class Sphere {
public:
	glm::vec3 m_position;
	float m_radius;
	Material& m_material;

	Sphere(glm::vec3 pos, float radius, Material & mat) :
		m_position(pos),
		m_radius(radius),
		m_material(mat)
	{}

	bool InterSect(glm::vec3 ray_point, glm::vec3 ray_normal , glm::vec3 &point , glm::vec3& normal ) {
		ray_normal = glm::normalize(ray_normal);

			glm::vec3 to_center = m_position - ray_point;
			glm::vec3 center_to_ray = (glm::dot(to_center,ray_normal) * ray_normal) - to_center;

			if (glm::dot(center_to_ray,center_to_ray) < m_radius*m_radius) 
			{
				float delta_len = sqrt(m_radius * m_radius - glm::dot(center_to_ray,center_to_ray));
				glm::vec3 normal_direction = center_to_ray - delta_len * ray_normal;
				point = m_position + normal_direction;
				normal = glm::normalize(normal_direction);
				return true;
			}
			else 
				return false;
	}
};
