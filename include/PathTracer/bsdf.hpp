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
  glm::vec3 diffuse_v = glm::normalize(Random::SpherePoint() + normal);
  if (glm::dot(diffuse_v, normal) < 0.0) diffuse_v = -diffuse_v;

  auto specular_v = glm::reflect(from, normal);
  // auto reflected_angle = glm::dot(reflected_v, o_normal);

  // float ra = specular + (1 - specular) * pow(1 - reflected_angle, 5);
  // bool is_specular = (rvalue() < ra);

  to = glm::mix(diffuse_v, specular_v, m_material.glossiness_);
  // to = diffuse_v;
  //  if (is_specular)
  //    new_ray = reflected_v;
  //  else
  //  new_ray = glm::normalize(glm::mix(reflected_v, diffuse_v, glossiness));

  // if (is_specular)
  //   color = light;
  // else
  // return RandomDirection() * 0.5f + glm::vec3{1, 1, 1};
  return diffuse;
}

#endif
