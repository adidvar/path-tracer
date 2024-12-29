#include "scene.hpp"

glm::vec3 color = {0.76, 0, 0};

Material plane_m_r({0.7, 0.1, 0.1}, 0, 0, 0.04);
Material plane_m_g({0.1, 0.7, 0.1}, 0, 0, 0.04);
Material plane_m_b({0.1, 0.1, 0.7}, 0, 0, 0.04);
Material plane_m_k({0.4, 0.4, 0.4}, 0, 0.4, 0);

Material plane_m_wl({1, 1, 1}, 1.7, 0.95, 1);
Material plane_m_rl({0.3, 0.8, 1}, 2.3, 0.95, 1);
Material plane_m_w({1, 1, 1}, 0, 0, 1);

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

std::vector<Sphere> objects_s{
    Sphere({-6, -6, 2}, 4, &plane_m_w),  Sphere({-6, -1, 2}, 1, &plane_m_w),
    Sphere({0, -3, 5.5}, 3, &plane_m_w), Sphere({6, -6, 6}, 4, &plane_m_w),
    Sphere({0, 0, 3}, 1, &plane_m_w),
};

std::vector<Plane> objects_p{Plane({0, -10, 0}, {0, 1, 0}, &plane_m_w),
                             Plane({10, 0, 10}, {-1, 0, 0}, &plane_m_r),
                             Plane({-10, 0, 0}, {1, 0, 0}, &plane_m_g),
                             Plane({0, 0, 10}, {0, 0, -1}, &plane_m_w),
                             Plane({10, 0, -10}, {0, 0, 1}, &plane_m_b),
                             Plane({0, 10, 0}, {0, -1, 0}, &plane_m_wl)};

Scene::Scene()
    : m_spheres(objects_s)
    , m_plains(objects_p)
    , m_meshes{Mesh({0, -7, 0}, 2, &plane_m_w, "mesh.obj")}
{}

const static float kmax_search_distance = 99999;

std::optional<Hit> Scene::Intersect(Ray ray) const {
  Hit hit;
  float r_max = kmax_search_distance;

  auto find = [ray, &r_max, &hit]<typename T>(const std::vector<T> &elements) {
    bool founded = false;
    for (auto it = elements.begin(); it != elements.end(); ++it) {
      Ray normal;
      glm::vec2 uv;

      if (!it->AABBIntersect(ray)) continue;
      float r = it->Intersect(ray, normal, uv);
      if (r > 0 && r < r_max) {
        r_max = r;
        hit.position = normal.point;
        hit.normal = normal.direction;
        hit.material = it->GetMaterial();
        hit.texture_uv = uv;
      }
    }
    return founded;
  };

  find(m_spheres);
  find(m_plains);
  find(m_meshes);
  // find(m_triangles);

  if (r_max < kmax_search_distance) return hit;
  return {};
}
