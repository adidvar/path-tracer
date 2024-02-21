#ifndef FIGURES_H
#define FIGURES_H

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

class Sphere {
 public:
  Sphere(glm::vec3 pos, float radius, Material& mat);

  float Intersect(Ray ray) const;

  glm::vec2 GetUV(glm::vec3 coord) const;
  glm::vec3 GetNormal(glm::vec3 coord) const;
  Material* GetMaterial() const;

 private:
  glm::vec3 m_position;
  float m_radius;
  Material* m_material;
};

class Plane {
 public:
  Plane(glm::vec3 normal, glm::vec3 position, Material& mat);

  float Intersect(Ray ray) const;

  glm::vec2 GetUV(glm::vec3 coord) const;
  glm::vec3 GetNormal(glm::vec3 coord) const;
  Material* GetMaterial() const;

 private:
  glm::vec3 m_normal;
  glm::vec3 m_position;
  Material* m_material;
};

class Triangle {
 public:
  Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 n,
           Material& material);

  float Intersect(Ray ray) const;

  glm::vec2 GetUV(glm::vec3 coord) const;
  glm::vec3 GetNormal(glm::vec3 coord) const;
  Material* GetMaterial() const;

 private:
  glm::vec3 v0;
  glm::vec3 v1;
  glm::vec3 v2;
  glm::vec3 n;

  Material* m_material;
};

class Mesh {
 public:
  Mesh(std::string mesh, Material* material, float scale, glm::vec3 pos);

  float Intersect(Ray ray) const;

  glm::vec2 GetUV(glm::vec3 coord) const;
  glm::vec3 GetNormal(glm::vec3 coord) const;
  Material* GetMaterial() const;

 private:
  Material* m_material;
};

#endif
