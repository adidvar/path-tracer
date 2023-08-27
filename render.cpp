#include "render.h"

#include <array>
#include <chrono>
#include <fstream>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "figures.h"
#include "nrandom.h"
#include "search.h"
#include "skybox.h"

glm::vec3 normal_color(glm::vec3 color) {
  color.x = std::min(color.x, 1.0f);
  color.y = std::min(color.y, 1.0f);
  color.z = std::min(color.z, 1.0f);
  color.x = std::max(color.x, 0.0f);
  color.y = std::max(color.y, 0.0f);
  color.z = std::max(color.z, 0.0f);
  return color;
}

uint32_t vectocolor(glm::vec3 color) {
  color = normal_color(color);
  uint8_t r = 255 * color.x;
  uint8_t g = 255 * color.y;
  uint8_t b = 255 * color.z;
  return ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b << 0);
}

glm::vec3 tonemapping(glm::vec3 color) {
  // return { sqrt(color.x),sqrt(color.y),sqrt(color.z)};
  return color;
  //	return color;
}

SkyBox skybox;

glm::vec3 RayTrace(glm::vec3 ray_point, glm::vec3 ray_normal, size_t depth) {
  HitInfo info;
  bool intersect = FindInterception(ray_point, ray_normal, info);

  if (intersect == false) return skybox.GetColor(glm::normalize(ray_normal));

  if (depth == 0) return {0, 0, 0};

  auto o_normal = info.GetNormal();
  auto o_point = info.GetPosition();

  const Material& m_material = *info.GetMaterial();

  auto light_power = m_material.light_power_;
  if (light_power <= 0) light_power = -light_power;
  auto own_light = m_material.light_power_ * m_material.diffuse_;

  auto specular = m_material.specular_;
  auto glossiness = m_material.glossiness_;
  auto diffuse = m_material.diffuse_;

  auto reflected_v = glm::reflect(ray_normal, o_normal);
  // auto reflected_angle = glm::dot(reflected_v, o_normal);
  glm::vec3 diffuse_v = glm::normalize(RandomDirection() + o_normal);
  if (glm::dot(diffuse_v, o_normal) < 0.0) diffuse_v = -diffuse_v;
  diffuse_v = glm::normalize(diffuse_v + o_normal);

  // float ra = specular + (1 - specular) * pow(1 - reflected_angle, 5);
  // bool is_specular = (rvalue() < ra);

  glm::vec3 new_ray;
  // if (is_specular)
  //   new_ray = reflected_v;
  // else
  new_ray = glm::normalize(glm::mix(reflected_v, diffuse_v, glossiness));

  auto in_light = RayTrace(o_point, new_ray, depth - 1);

  glm::vec3 color;
  // if (is_specular)
  //   color = light;
  // else
  color = in_light * diffuse;
  // return RandomDirection() * 0.5f + glm::vec3{1, 1, 1};

  return color + own_light;
}

glm::vec3 buffer[WIDTH][HEIGHT] = {};
size_t iteration = 1;

size_t iterations_per_frame = 5;

float screen_ratio = (float)WIDTH / HEIGHT;

void task(size_t current, size_t max) {
  RandomInit();
  glm::vec3 projection_center{0, 0, 0};
  for (size_t x = current; x < WIDTH; x += max) {
    for (size_t y = 0; y < HEIGHT; y++) {
      glm::vec3 ray_point{(-2.0 * x / WIDTH + 1) * screen_ratio,
                          -2.0 * y / HEIGHT + 1, 1};
      glm::vec3 ray_normal = glm::normalize(ray_point - projection_center);

      auto delta = RandomDirection();
      delta /= 1000;

      buffer[x][y] *= iteration;

      for (size_t i = 0; i < iterations_per_frame; i++) {
        auto color =
            RayTrace(projection_center, glm::normalize(ray_normal + delta), 4);
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

	/*
	for (size_t x = 0; x < WIDTH; x++) {
		for (size_t y = 0; y < HEIGHT; y++) {
			buffer[x][y] = {};
		}
	}
	*/
	using namespace std;
	
	pool.Run();

//		task(0, 2);
//		task(1, 2);

	for (size_t x = 0; x < WIDTH; x++) {
		for (size_t y = 0; y < HEIGHT; y++) {
			scene[WIDTH * y + x] = vectocolor(tonemapping(buffer[x][y]));
		}
	}
	iteration+=iterations_per_frame;

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
}
