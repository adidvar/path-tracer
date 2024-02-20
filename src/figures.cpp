#include <PathTracer/pathtracer.hpp>

Sphere::Sphere(glm::vec3 pos, float radius, Material &mat)
    : m_position(pos), m_radius(radius), m_material(&mat) {}

float Sphere::Intersect(Ray ray) const {
  glm::vec3 oc = ray.point - m_position;
  float a = glm::dot(ray.direction, ray.direction);
  float b = 2.0f * glm::dot(oc, ray.direction);
  float c = glm::dot(oc, oc) - m_radius * m_radius;
  float discriminant = b * b - 4 * a * c;
  // float t1 = (-b - discriminant) / 2 * a, t2 = (-b - discriminant) / 2 * a;

  if (discriminant < 0)
    return -1.0f;
  else
    return (-b - sqrt(discriminant)) / (2.0f * a);
}

glm::vec2 Sphere::GetUV(glm::vec3 coord) const { return {0, 0}; }

glm::vec3 Sphere::GetNormal(glm::vec3 coord) const {
  return glm::normalize(coord - m_position);
}

Material *Sphere::GetMaterial() const { return m_material; }

Plane::Plane(glm::vec3 normal, glm::vec3 position, Material &mat)
    : m_normal(glm::normalize(normal)),
      m_position(position),
      m_material(&mat) {}

float Plane::Intersect(Ray ray) const {
  float p = glm::dot(ray.direction, m_normal);

  if (p >= 0) return -1.0f;

  float d = glm::dot(m_position - ray.point, m_normal) / p;

  return d;
}

glm::vec2 Plane::GetUV(glm::vec3 coord) const { return {0, 0}; }

glm::vec3 Plane::GetNormal(glm::vec3 coord) const { return m_normal; }

Material *Plane::GetMaterial() const { return m_material; }
