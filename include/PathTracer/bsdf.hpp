#ifndef BSDF_H
#define BSDF_H

#include "frandom.hpp"
#include "material.hpp"
#include "pch.hpp"

inline glm::vec3 bsdf(glm::vec3 from, glm::vec3 &to, glm::vec3 normal,
                      const Material &m_material) {
  // auto specular = m_material.specular_;
  // auto glossiness = m_material.glossiness_;
  auto diffuse = m_material.diffuse_;
  glm::vec3 cosine_distributed_ray =
      glm::normalize(Random::SpherePoint() + normal);

  to = cosine_distributed_ray;

  glm::vec3 lambert = m_material.diffuse_;

  return lambert;
}

#endif
