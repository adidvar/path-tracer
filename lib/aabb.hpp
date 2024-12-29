#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <limits>

#include "ray.hpp"

class AABB final {
 public:
  glm::vec3 min;
  glm::vec3 max;

  AABB()
      : min(std::numeric_limits<float>::max()),
        max(std::numeric_limits<float>::lowest()) {}
  // AABB() : min(0.0f), max(0.0f) {}
  AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

  static AABB Empty() { return AABB(glm::vec3{0.0}, glm::vec3{0.0}); }
  static AABB Full() {
    return AABB(glm::vec3{std::numeric_limits<float>::max()},
                glm::vec3{std::numeric_limits<float>::lowest()});
  }

  bool Intersect(const Ray& ray, float& tNear, float& tFar) const {
    glm::vec3 tMin = (min - ray.point) / ray.direction;
    glm::vec3 tMax = (max - ray.point) / ray.direction;

    // Обчислення мінімальних і максимальних значень t
    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    tNear = std::max({t1.x, t1.y, t1.z});
    tFar = std::min({t2.x, t2.y, t2.z});

    // Перевірка на наявність перетину
    return tNear <= tFar && tFar >= 0.0f;
  }

  void AddPoint(glm::vec3 point) {
    *this = AABB::Merge(*this, AABB(point, point));
  }

  float DistanceToCenter(const glm::vec3& point) const {
    glm::vec3 center = (min + max) * 0.5f;
    return glm::length(center - point);
  }

  static AABB Merge(const AABB& a, const AABB& b) {
    glm::vec3 newMin = glm::min(a.min, b.min);
    glm::vec3 newMax = glm::max(a.max, b.max);
    return AABB(newMin, newMax);
  }
};
