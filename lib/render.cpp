#include "render.hpp"

#include "bsdf.hpp"

#include <omp.h>
#include <iostream>

/*
glm::vec3 clamp_color(glm::vec3 color) {
  return glm::clamp(color, glm::vec3{0, 0, 0}, glm::vec3{1, 1, 1});
}

uint32_t process_and_convert_color(glm::vec3 color) {
  color = clamp_color(color);
  uint8_t r = 255.0f * color.x;
  uint8_t g = 255.0f * color.y;
  uint8_t b = 255.0f * color.z;
  return ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b << 0);
}

SkyBox skybox;

glm::vec3 RayTrace(glm::vec3 ray_point, glm::vec3 ray_normal) {
  glm::vec3 light = {0, 0, 0};
  glm::vec3 color = {1, 1, 1};
  Hit info;

  for (size_t i = 0; i < 10; i++) {
    bool intersect = FindInterception(ray_point, ray_normal, info);

    if (intersect == false) {
      light += color * skybox.GetColor(glm::normalize(ray_normal));
      return light;
    }

    const Material& m_material = *info.GetMaterial();

    auto light_power = m_material.light_power_;
    if (glm::length(light_power) > 0) {
      light += color * light_power;
      return light;
    }

    glm::vec3 to;
    auto bsdf_result = bsdf(ray_normal, to, info.GetNormal(), m_material);
    color *= bsdf_result;

    light += color * light_power;

    ray_point = info.GetPosition();
    ray_normal = to;
  }
  return light;
}

glm::vec3 buffer[WIDTH][HEIGHT] = {};
size_t iteration = 0;
size_t cycles_peer_pixel = 100;

float screen_ratio = (float)WIDTH / HEIGHT;

void task(int current, int max) {
  RandomInit();

  for (size_t x = current; x < WIDTH; x += max) {
    for (size_t y = 0; y < HEIGHT; y++) {
      glm::vec2 screen_point{(-2.0 * x / WIDTH + 1) * screen_ratio,
                             -2.0 * y / HEIGHT + 1};

      auto [point, direction] =
          camera.GetRayFromCamera(screen_point.x, screen_point.y);

      for (size_t i = 0; i < cycles_peer_pixel; ++i) {
        auto color = RayTrace(point, direction);
        buffer[x][y] += color;
      }
    }
  }
}

ThreadPool pool(&task);

void render(uint32_t* scene) {
  auto begin = std::chrono::high_resolution_clock::now();

  pool.Run();
  iteration += cycles_peer_pixel;

  for (int x = 1; x < WIDTH-1; x++) {
    for (int y = 1; y < HEIGHT-1; y++) {

      //convolution

      glm::vec3 sum = buffer[x][y] * (1.0f);

      for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
          if ( ! (x==0 && y==0))
          sum += buffer[x + dx][y + dy] * (1.0f/16);

scene[WIDTH * y + x] = process_and_convert_color(sum / float(iteration));
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     begin)
                   .count()
            << std::endl;
}
*/
glm::vec3 ClampVec3(glm::vec3 color) {
  return glm::clamp(color, glm::vec3{0, 0, 0}, glm::vec3{1, 1, 1});
}

uint32_t ToRBGA(glm::vec3 color) {
  color = ClampVec3(color);
  uint8_t r = 255.0f * color.x;
  uint8_t g = 255.0f * color.y;
  uint8_t b = 255.0f * color.z;
  return ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b << 0);
}

Renderer::Renderer(size_t width, size_t height)
    : m_width(width),
      m_height(height),
      m_iterations(0),
      m_screen_buffer(new glm::vec3[width * height]) {}

Renderer::~Renderer() {}

void Renderer::Trace(size_t iteration) {
#pragma omp parallel for
  for (int x = 0; x < m_width; ++x)
    for (int y = 0; y < m_height; ++y)
      for (size_t it = 0; it < iteration; ++it) Pixel(x, y) += TracePixel(x, y);
  m_iterations += iteration;
}

void Renderer::Export(uint32_t *buffer) {
  float scale_factor = 1.0 * 1.0 / m_iterations;

  for (auto i = 0; i < m_width * m_height; i++)
    buffer[i] = ToRBGA(scale_factor * m_screen_buffer[i]);
}

void Renderer::Clear() {
  for (int x = 0; x < m_width; ++x)
    for (int y = 0; y < m_height; ++y) Pixel(x, y) = glm::vec3(0, 0, 0);
  m_iterations = 1;
}

bool Renderer::PathTrace(Ray &ray, glm::vec3 &color) {
  /*
  auto hit = m_scene.Intersect(ray);

  if (!hit.has_value()) {
    color = m_skybox.Get(ray.direction);
    return false;
  }

  Hit info = hit.value();

  const Material *m_material = info.material;

  auto distance = glm::distance(info.position, ray.point);

  color = m_material->diffuse_ * (float)(1.0 / distance + 0.5);

  return false;
*/

  auto hit = m_scene.Intersect(ray);

  if (!hit.has_value()) {
    color *= m_skybox.Get(ray.direction);
    return false;
  }

  Hit info = hit.value();

  const Material *m_material = info.material;

  if (glm::length(m_material->light_power_) > 0) {
    color *= m_material->diffuse_ * m_material->light_power_;
    return false;
  }

  glm::vec3 to;
  auto bsdf_result = bsdf(ray.direction, to, info.normal, *m_material);
  color *= bsdf_result;

  ray.point = info.position;
  ray.direction = to;

  return true;
}

glm::vec3 Renderer::TracePixel(unsigned int x, unsigned int y) {
  float aspect_ratio = (float)m_width / m_height;

  glm::vec2 screen_coord{aspect_ratio * ((float)x / m_width * 2.0 - 1),
                         (float)y / m_height * 2.0 - 1};
  auto ray = m_camera.GetRayFromCamera(screen_coord);

  glm::vec3 color{1, 1, 1};

  int max_bounce = 6;
  int i = 0;
  for (; i < max_bounce && PathTrace(ray, color); i++) {
  }

  if (i == max_bounce) return {0, 0, 0};

  return color;
}

glm::vec3 &Renderer::Pixel(unsigned int x, unsigned int y) {
  return m_screen_buffer[(m_height - y - 1) * m_width + x];
}
