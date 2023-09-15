#include "render.h"

#include <array>
#include <chrono>
#include <fstream>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "bsdf.h"
#include "camera.h"
#include "nrandom.h"
#include "search.h"
#include "skybox.h"
#include "threadpool.h"

glm::vec3 clamp_color(glm::vec3 color) {
  return glm::clamp(color, glm::vec3{0, 0, 0}, glm::vec3{1, 1, 1});
}

uint32_t process_and_convert_color(glm::vec3 color) {
  color = clamp_color(color);
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

  for (size_t i = 0; i < 10; i++) {
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
size_t cycles_peer_pixel = 5;

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

  for (size_t x = 0; x < WIDTH; x++) {
    for (size_t y = 0; y < HEIGHT; y++) {
      scene[WIDTH * y + x] =
          process_and_convert_color(buffer[x][y] / float(iteration));
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     begin)
                   .count()
            << std::endl;
}
