#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "frandom.hpp"

unsigned int fov_ = 90;
float focus_distance_ = 8;
float blue_power = 0.05;

glm::vec3 point_ = {0, -2, -9};
glm::vec3 direction_ = {0, 0, 1};
const glm::vec3 vertical_direction_ = {0, 1, 0};

Camera::Camera() { m_camera_matrix = glm::mat4(1.0f); }

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

  auto d = glm::normalize(plane_point - start_point);
  return {glm::vec4(start_point,1) * m_camera_matrix, glm::vec4(d,0) * m_camera_matrix};
}

void Camera::Rotate(glm::vec2 angles) {
  m_rotation_angles += angles;
  m_rotation_angles.y = glm::clamp(m_rotation_angles.y, -1.3f, 1.3f);

  m_camera_matrix = glm::mat4(1.0f);
  m_camera_matrix = glm::rotate(m_camera_matrix, m_rotation_angles.y, glm::vec3{1, 0, 0});
  m_camera_matrix = glm::rotate(m_camera_matrix, m_rotation_angles.x, glm::vec3{0, 1, 0});
}
