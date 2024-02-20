#ifndef HITINFO_H
#define HITINFO_H

#include "material.hpp"
#include "pch.hpp"

struct Hit {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_uv;
  Material* material;
};

#endif
