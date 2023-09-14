#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <tuple>

class Camera {
 public:
  Camera();

  std::pair<glm::vec3, glm::vec3> GetRayFromCamera(float x, float y) const;

 private:
  unsigned int fov_ = 90;

  bool enable_depth_ = false;
  float focus_distance_ = 10;

  glm::vec3 point_ = {0, -2, -9};
  glm::vec3 direction_ = {0, 0, 1};
  const glm::vec3 vertical_direction_ = {0, 1, 0};
};

extern const Camera camera;

#endif
