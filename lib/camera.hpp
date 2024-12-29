#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

#include "ray.hpp"

class Camera {
 public:
  Camera();
  Ray GetRayFromCamera(glm::vec2 screen_pos) const;
  void Rotate(glm::vec2 angles);

  private:
  glm::mat4 m_camera_matrix;
  glm::mat4 m_projection_matrix;
  glm::vec2 m_rotation_angles;
};

#endif
