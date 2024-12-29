#ifndef FIGURES_H
#define FIGURES_H

#include <string>

#include "aabb.hpp"
#include "hit.hpp"
#include "material.hpp"
#include "ray.hpp"

/*
 * Default figure interface
 *
 * float Intersect(Ray ray) const;
 *
 * glm::vec2 GetUV( glm::vec3 coord ) const;
 * glm::vec3 GetNormal( glm::vec3 coord ) const;
 * Material GetMaterial() const;
 *
 */

class Object {
 public:
  Object(Material* material) : m_material(material), m_box{AABB::Empty()} {}

  virtual float Intersect(const Ray& ray, Ray& normal_ray,
                          glm::vec2& uv) const = 0;
  bool AABBIntersect(const Ray& ray) const { 
    float tn, tf;
    return m_box.Intersect(ray, tn, tf);
  };

  Material* GetMaterial() const { return m_material; }

 protected:
  AABB m_box;

 private:
  Material* m_material;
};

class Sphere final : public Object {
 public:
  Sphere(glm::vec3 position, float scale, Material* material);

  float Intersect(const Ray& ray, Ray& normal_ray, glm::vec2& uv) const override;

 private:
  glm::vec3 m_position;
  float m_scale;
};

class Plane final : public Object {
 public:
  Plane(glm::vec3 position, glm::vec3 normal, Material* material);

  float Intersect(const Ray &ray, Ray& normal_ray, glm::vec2& uv) const override;

 private:
  glm::vec3 m_position;
  glm::vec3 m_normal;
};

class Triangle final : public Object {
 public:
  Triangle(Material* material, glm::vec3 v0,
           glm::vec3 v1, glm::vec3 v2, glm::vec3 n);

  float Intersect(const Ray &ray, Ray& normal_ray, glm::vec2& uv) const override;

 private:
  glm::vec3 v0;
  glm::vec3 v1;
  glm::vec3 v2;
  glm::vec3 n;
};

class Mesh final : public Object {
 public:
  Mesh(glm::vec3 position, float scale, Material* material,
       std::string file_name);

  float Intersect(const Ray& ray, Ray& normal_ray, glm::vec2& uv) const override;

 private:
  glm::vec3 m_position;
  float m_scale;
};

#endif
