#include "hitinfo.h"

HitInfo::HitInfo() {}

HitInfo::HitInfo(glm::vec3 position, glm::vec3 normal, const Material *material)
    : position_(position), normal_(normal), material_(material) {}

glm::vec3 HitInfo::GetPosition() const { return position_; }

glm::vec3 HitInfo::GetNormal() const { return normal_; }

const Material *HitInfo::GetMaterial() const { return material_; }
