#ifndef FIGURES_H
#define FIGURES_H

#include "hit.hpp"
#include "material.hpp"
#include "pch.hpp"

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
  Object(glm::vec3 position, float scale, Material* material)
      : m_position(position), m_scale(scale), m_material(material) {}

  float GetScale() const { return m_scale; }
  Material* GetMaterial() const { return m_material; }
  glm::vec3 GetPosition() const { return m_position; }

  virtual float Intersect(Ray ray, Ray& normal_ray, glm::vec2& uv) const = 0;

 private:
  float m_scale;
  glm::vec3 m_position;
  Material* m_material;
};

class Sphere final : public Object {
 public:
  Sphere(glm::vec3 position, float scale, Material* material);

  float Intersect(Ray ray, Ray& normal_ray, glm::vec2& uv) const;
};

class Plane final : public Object {
 public:
  Plane(glm::vec3 position, glm::vec3 normal, Material* material);

  float Intersect(Ray ray, Ray& normal_ray, glm::vec2& uv) const;

 private:
  glm::vec3 m_normal;
};

class Triangle final : public Object {
 public:
  Triangle(glm::vec3 position, float scale, Material* material, glm::vec3 v0,
           glm::vec3 v1, glm::vec3 v2, glm::vec3 n);

  float Intersect(Ray ray, Ray& normal_ray, glm::vec2& uv) const;

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

  float Intersect(Ray ray, Ray& normal_ray, glm::vec2& uv) const;

 private:
};

#endif
