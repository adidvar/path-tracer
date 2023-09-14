#include "camera.h"

#include <cmath>

const Camera camera;

Camera::Camera() {}

std::pair<glm::vec3, glm::vec3> Camera::GetRayFromCamera(float x,
                                                         float y) const {
  glm::vec3 point = point_;
  glm::vec3 direction;

  glm::vec3 vertical_step = glm::normalize(vertical_direction_);
  glm::vec3 horizonatal_step =
      glm::normalize(glm::cross(vertical_step, direction_));

  glm::vec3 matrix_point = x * horizonatal_step + y * vertical_step;

  direction = glm::normalize(matrix_point +
                             direction_ / tanf((fov_ / 180.0f * 3.14f) / 2.0));

  return {point, direction};
}
