#include "search.h"

#include "figures.h"
#include "materials.h"

#include <limits>

glm::vec3 color = {0.76, 0, 0};

Material plane_m_r({0.7, 0.1, 0.1}, 0, 0.95, 0.04);
Material plane_m_g({0.1, 0.7, 0.1}, 0, 0.95, 0.04);
Material plane_m_b({0.1, 0.1, 0.7}, 0, 0.95, 0.04);
Material plane_m_k({0.4, 0.4, 0.4}, 0, 0.4, 0);

Material plane_m_wl({1, 1, 1}, 1.9, 0.95, 1);
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

std::vector<Sphere> objects_s{Sphere(glm::vec3{0, 0, 4}, 6, plane_m_w)};

std::vector<Plane> objects_p{Plane({0, 1, 0}, {0, -10, 0}, plane_m_w),
                             Plane({-1, 0, 0}, {10, 0, 10}, plane_m_r),
                             Plane({1, 0, 0}, {-10, 0, 0}, plane_m_g),
                             Plane({0, 0, -1}, {0, 0, 10}, plane_m_w),
                             Plane({0, 0, 1}, {10, 0, -10}, plane_m_b),
                             Plane({0, -1, 0}, {0, 10, 0}, plane_m_wl)};

template <typename T>
bool FindInterceptionForFigure(glm::vec3 ray_point, glm::vec3 ray_direction,
                               HitInfo& info, float value,
                               const std::vector<T>& objects) {
  size_t obj_index = -1;

  for (size_t i = 0; i < objects.size(); i++) {
    float r = objects[i].InterSect(ray_point, ray_direction);
    if (r > 0 && r < value) {
      value = r;
      obj_index = i;
    }
  }

  if (obj_index == (size_t)-1)
    return false;
  else {
    glm::vec3 point = ray_point + ray_direction * value;
    info = HitInfo(point, objects[obj_index].GetSurfaceNormal(point), &objects[obj_index].m_material);
    return true;
  }
}

bool FindInterception(glm::vec3 ray_point, glm::vec3 ray_direction,
                      HitInfo& info) {
  float value = std::numeric_limits<float>::max();
  bool intersects = FindInterceptionForFigure(ray_point, ray_direction, info,
                                              value, objects_p);
  intersects = std::max(FindInterceptionForFigure(ray_point, ray_direction,
                                                  info, value, objects_s),
                        intersects);
  return intersects;
}
