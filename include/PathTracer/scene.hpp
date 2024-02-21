#ifndef SCENE_HPP
#define SCENE_HPP

#include "figures.hpp"
#include "hit.hpp"
#include "pch.hpp"

class Scene {
 public:
  Scene();

  std::optional<Hit> Intersect(Ray ray) const;

 private:
  std::vector<Sphere> m_spheres;
  std::vector<Plane> m_plains;
  std::vector<Mesh> m_meshes;
  std::vector<Triangle> m_triangles;
};

#endif
