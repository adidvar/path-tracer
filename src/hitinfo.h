#ifndef HITINFO_H
#define HITINFO_H

#include <glm/glm.hpp>

#include "materials.h"

class HitInfo {
 public:
  HitInfo();
  HitInfo(glm::vec3 position, glm::vec3 normal_, const Material* material);
  HitInfo& operator=(const HitInfo&) = default;

  glm::vec3 GetPosition() const;
  glm::vec3 GetNormal() const;
  const Material* GetMaterial() const;

 private:
  glm::vec3 position_;
  glm::vec3 normal_;
  const Material* material_;
};

#endif
