#include <PathTracer/pathtracer.hpp>

unsigned int fov_ = 100;
float focus_distance_ = 8;
float blue_power = 0.01;

glm::vec3 point_ = {0, -2, -9};
glm::vec3 direction_ = {0, 0, 1};
const glm::vec3 vertical_direction_ = {0, 1, 0};

Camera::Camera() {}

Ray Camera::GetRayFromCamera(glm::vec2 screen_pos) const {
  glm::vec3 vertical_step = glm::normalize(vertical_direction_);
  glm::vec3 horizonatal_step =
      glm::normalize(glm::cross(vertical_step, direction_));

  glm::vec3 start_point =
      point_ + blue_power * (vertical_step * Random::NormalValue() +
                             horizonatal_step * Random::NormalValue());

  glm::vec3 matrix_point =
      screen_pos.x * horizonatal_step + screen_pos.y * vertical_step;

  glm::vec3 direction;
  direction = glm::normalize(matrix_point +
                             direction_ / tanf((fov_ / 180.0f * 3.14f) / 2.0));

  auto plane_point = focus_distance_ * direction + point_;

  return {start_point, glm::normalize(plane_point - start_point)};
}
