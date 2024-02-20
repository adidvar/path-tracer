#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "pch.hpp"

class Skybox {
 public:
  Skybox();
  glm::vec3 Get(glm::vec3 direction);

 private:
  std::vector<unsigned char> image_buffer_;
  unsigned int height_;
  unsigned int width_;
};

#endif
