#include "render.h"

#include <array>
#include <chrono>
#include <fstream>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "bsdf.h"
#include "nrandom.h"
#include "search.h"
#include "skybox.h"

glm::vec3 normal_color(glm::vec3 color) {
  return glm::clamp(color, glm::vec3{0, 0, 0}, glm::vec3{1, 1, 1});
}

uint32_t vectocolor(glm::vec3 color) {
  color = normal_color(color);
  uint8_t r = 255 * color.x;
  uint8_t g = 255 * color.y;
  uint8_t b = 255 * color.z;
  return ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b << 0);
}

SkyBox skybox;

glm::vec3 RayTrace(glm::vec3 ray_point, glm::vec3 ray_normal) {
  glm::vec3 light = {0, 0, 0};
  glm::vec3 color = {1, 1, 1};
  HitInfo info;

  for (size_t i = 0; i < 3; i++) {
    bool intersect = FindInterception(ray_point, ray_normal, info);

    if (intersect == false) {
      light += color * skybox.GetColor(glm::normalize(ray_normal));
      return light;
    }

    const Material& m_material = *info.GetMaterial();

    auto light_power = m_material.light_power_;
    // if (light_power <= 0) light_power = -light_power;

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
size_t iteration = 1;

size_t iterations_per_frame = 5;

float screen_ratio = (float)WIDTH / HEIGHT;

void task(size_t current, size_t max) {
  RandomInit();

  glm::vec3 projection_center{0, 0, -7};
  for (size_t x = current; x < WIDTH; x += max) {
    for (size_t y = 0; y < HEIGHT; y++) {
      glm::vec3 ray_point{(-2.0 * x / WIDTH + 1) * screen_ratio,
                          -2.0 * y / HEIGHT + 1, -6};
      glm::vec3 ray_normal = glm::normalize(ray_point - projection_center);

      auto delta = RandomDirection();
      delta /= 4000;

      buffer[x][y] *= iteration;

      for (size_t i = 0; i < iterations_per_frame; i++) {
        auto color =
            RayTrace(projection_center, glm::normalize(ray_normal + delta));
        buffer[x][y] += color;
      }

      buffer[x][y] /= (iteration + iterations_per_frame);
    }
  }
}

std::vector<std::function<void(void)>> tasks{
    std::bind(&task, 0, 5), std::bind(&task, 1, 5), std::bind(&task, 2, 5),
    std::bind(&task, 3, 5), std::bind(&task, 4, 5)};

ThreadPool pool(tasks);

void render(uint32_t* scene) {
  auto begin = std::chrono::high_resolution_clock::now();

  pool.Run();

  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      scene[WIDTH * y + x] = vectocolor(buffer[x][y]);
    }
  }
  iteration += iterations_per_frame;

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     begin)
                   .count()
            << std::endl;
}
