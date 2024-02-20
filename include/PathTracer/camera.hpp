#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "pch.hpp"
#include "ray.hpp"

class Camera {
 public:
  Camera();
  Ray GetRayFromCamera(glm::vec2 screen_pos) const;
};

#endif
