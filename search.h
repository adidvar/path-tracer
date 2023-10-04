#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <glm/glm.hpp>

#include "hitinfo.h"

bool FindInterception(glm::vec3 ray_point, glm::vec3 ray_direction,
                      HitInfo &info);

void GenerateScene();

#endif
