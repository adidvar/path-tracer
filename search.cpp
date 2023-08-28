#include "search.h"

#include "figures.h"
#include "materials.h"

glm::vec3 color = {0.76, 0, 0};

Material plane_m_r({0.7, 0.1, 0.1}, 0, 0.95, 0.04);
Material plane_m_g({0.1, 0.7, 0.1}, 0, 0.95, 0.04);
Material plane_m_b({0.1, 0.1, 0.7}, 0, 0.95, 0.04);
Material plane_m_k({0.4, 0.4, 0.4}, 0, 0.4, 0);

Material plane_m_wl({1, 1, 1}, 1.1, 0.95, 1);
Material plane_m_w({1, 1, 1}, 0, 0.95, 1);

Material mm00(color, 0, 1, 0);
Material mm10(color, 0, 0.8, 0);
Material mm20(color, 0, 0.2, 0);
Material mm30(color, 0, 0, 0.0);
Material mm01(color, 0, 1, 0.2);
Material mm11(color, 0, 0.8, 0.2);
Material mm21(color, 0, 0.8, 0.2);
Material mm31(color, 0, 0, 0.2);
Material mm02(color, 0, 1, 0.8);
Material mm12(color, 0, 0.2, 0.8);
Material mm22(color, 0, 0.22, 0.8);
Material mm32(color, 0, 0, 0.8);
Material mm03(color, 0, 1, 1);
Material mm13(color, 0, 0.8, 1);
Material mm23(color, 0, 0.2, 1);
Material mm33(color, 0, 0, 1);

Material light({0.5, 0.1, 0.1}, 0, 0.3, 0.02);
Material light1({0.1, 0.1, 0.1}, 0, 0.3, 0.02);
Material light2({0.05, 0.05, 0.1}, 0, 0.3, 0.02);

std::vector<Sphere> objects_s{Sphere(glm::vec3{2, 1, 0}, 1, plane_m_w),
                              Sphere(glm::vec3{0, 1, 0}, 1, light),
                              Sphere(glm::vec3{-2, 1, 0}, 1, light2),
                              Sphere(glm::vec3{2, -1, 0}, 1, light1),
                              Sphere(glm::vec3{2, -1, 0}, 1, light2),
                              Sphere(glm::vec3{0, -1, 0}, 1, light),
                              Sphere(glm::vec3{-2, 0, 0}, 1, light),
                              Sphere(glm::vec3{0, 0, 0}, 1, light2),
                              Sphere(glm::vec3{-2, 0, 0}, 1, light)};

std::vector<Plane> objects_p{Plane({0, 1, 0}, {0, -10, 0}, plane_m_w),
                             Plane({-1, 0, 0}, {10, 0, 10}, plane_m_r),
                             Plane({1, 0, 0}, {-10, 0, 0}, plane_m_g),
                             Plane({0, 0, -1}, {0, 0, 10}, plane_m_w),
                             Plane({0, 0, 1}, {10, 0, -10}, plane_m_b),
                             Plane({0, -1, 0}, {0, 10, 0}, plane_m_wl)};

template <typename T>
bool FindInterceptionForFigure(glm::vec3 ray_point, glm::vec3 ray_direction,
                               HitInfo& info, const std::vector<T>& objects) {
  size_t i = 0;
  size_t obj_index;

  glm::vec3 min_point;
  glm::vec3 min_normal;

  while (i < objects.size() &&
         !(objects[i].InterSect(ray_point, ray_direction, min_point,
                                min_normal) == true &&
           (glm::dot(ray_direction, (min_point - ray_point)) > 0)))
    i++;
  obj_index = i;

  if (i == objects.size()) return false;

  for (; i < objects.size(); ++i) {
    glm::vec3 current_point, current_normal;
    if (objects[i].InterSect(ray_point, ray_direction, current_point,
                             current_normal) == true &&
        (glm::dot(ray_direction, (current_point - ray_point)) > 0)) {
      if (glm::distance(min_point, ray_point) >
          glm::distance(current_point, ray_point)) {
        min_point = current_point;
        min_normal = current_normal;
        obj_index = i;
      }
    }
  }
  info = HitInfo(min_point, min_normal, &objects[obj_index].m_material);
  return true;
}

bool FindInterception(glm::vec3 ray_point, glm::vec3 ray_direction,
                      HitInfo& info) {
  return FindInterceptionForFigure(ray_point, ray_direction, info, objects_p);
}
