#ifndef FIGURES_H
#define FIGURES_H

#include <glm/glm.hpp>

#include "materials.h"

class Sphere {
 public:
  glm::vec3 m_position;
  float m_radius;
  Material& m_material;

  Sphere(glm::vec3 pos, float radius, Material& mat)
      : m_position(pos), m_radius(radius), m_material(mat) {}

  float InterSect(glm::vec3 ray_point, glm::vec3 ray_direction) const {
    glm::vec3 oc = ray_point - m_position;
    float a = dot(ray_direction, ray_direction);
    float b = 2.0f * dot(oc, ray_direction);
    float c = dot(oc, oc) - m_radius * m_radius;
    float discriminant = b * b - 4 * a * c;
    float t1 = (-b - discriminant) / 2 * a, t2 = (-b - discriminant) / 2 * a;

    if (discriminant < 0)
      return -1.0;
    else
      return (-b - sqrt(discriminant)) / (2.0f * a);
  }

  glm::vec3 GetSurfaceNormal(glm::vec3 point) const {
    return glm::normalize(point - m_position);
  }
};

class Plane {
 public:
  glm::vec3 m_normal;
  glm::vec3 m_position;
  Material& m_material;

  Plane(glm::vec3 normal, glm::vec3 position, Material& mat)
      : m_normal(glm::normalize(normal)),
        m_position(position),
        m_material(mat) {}

  float InterSect(glm::vec3 ray_point, glm::vec3 ray_direction) const {
    float p = glm::dot(ray_direction, m_normal);

    if (p >= 0) return -1.0f;

    float d = glm::dot(m_position - ray_point, m_normal) / p;

    return d;
  } 
  glm::vec3 GetSurfaceNormal(glm::vec3 point) const { return m_normal; }

};

#endif
