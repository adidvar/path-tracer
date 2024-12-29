#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "camera.hpp"
#include "scene.hpp"
#include "skybox.hpp"

class Renderer {
 public:
  Renderer(size_t width, size_t height);
  ~Renderer();

  void Trace(size_t iteration);
  void Export(uint32_t* buffer);
  void Clear();

 private:
  bool PathTrace(Ray& ray, glm::vec3& color);
  glm::vec3 TracePixel(unsigned x, unsigned y);
  glm::vec3& Pixel(unsigned x, unsigned y);

  std::unique_ptr<glm::vec3[]> m_screen_buffer;
  size_t m_iterations;
  size_t m_width;
  size_t m_height;
  Scene m_scene;
  Skybox m_skybox;

 public:
  Camera m_camera;
};

#endif
