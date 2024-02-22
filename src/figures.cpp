#include <PathTracer/pathtracer.hpp>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "PAthTracer/tiny_obj_loader.h"

Sphere::Sphere(glm::vec3 position, float scale, Material *material)
    : Object(position, scale, material) {}

float Sphere::Intersect(Ray ray, Ray &normal_ray, glm::vec2 &uv) const {
  glm::vec3 oc = ray.point - GetPosition();
  float a = glm::dot(ray.direction, ray.direction);
  float b = 2.0f * glm::dot(oc, ray.direction);
  float c = glm::dot(oc, oc) - GetScale() * GetScale();
  float discriminant = b * b - 4 * a * c;
  // float t1 = (-b - discriminant) / 2 * a, t2 = (-b - discriminant) / 2 * a;

  if (discriminant < 0)
    return -1.0f;
  else {
    float r = (-b - sqrt(discriminant)) / (2.0f * a);
    normal_ray.point = ray.point + r * ray.direction;
    normal_ray.direction = glm::normalize(ray.point - GetPosition());
    uv = {0, 0};
    return r;
  }
}

Plane::Plane(glm::vec3 position, glm::vec3 normal, Material *material)
    : Object(position, 1, material), m_normal(normal) {}

float Plane::Intersect(Ray ray, Ray &normal_ray, glm::vec2 &uv) const {
  float p = glm::dot(ray.direction, m_normal);

  if (p >= 0) return -1.0f;

  float d = glm::dot(GetPosition() - ray.point, m_normal) / p;

  normal_ray.point = ray.point + d * ray.direction;
  normal_ray.direction = m_normal;
  uv = {0, 0};

  return d;
}

typedef struct {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> texcoords;
  std::vector<int> v_indices;
  std::vector<int> vn_indices;
  std::vector<int> vt_indices;

  std::vector<tinyobj::material_t> materials;

} MyMesh;

void vertex_cb(void *user_data, float x, float y, float z, float w) {
  MyMesh *mesh = reinterpret_cast<MyMesh *>(user_data);

  mesh->vertices.push_back({x, y, z});
  // Discard w
}

void normal_cb(void *user_data, float x, float y, float z) {
  MyMesh *mesh = reinterpret_cast<MyMesh *>(user_data);

  mesh->normals.push_back({x, y, z});
}

void texcoord_cb(void *user_data, float x, float y, float z) {
  MyMesh *mesh = reinterpret_cast<MyMesh *>(user_data);

  mesh->texcoords.push_back({x, y, z});
}

void index_cb(void *user_data, tinyobj::index_t *indices, int num_indices) {
  // NOTE: the value of each index is raw value.
  // For example, the application must manually adjust the index with offset
  // (e.g. v_indices.size()) when the value is negative(whic means relative
  // index).
  // Also, the first index starts with 1, not 0.
  // See fixIndex() function in tiny_obj_loader.h for details.
  // Also, 0 is set for the index value which
  // does not exist in .obj
  MyMesh *mesh = reinterpret_cast<MyMesh *>(user_data);

  for (int i = 0; i < num_indices; i++) {
    tinyobj::index_t idx = indices[i];

    if (idx.vertex_index != 0) {
      mesh->v_indices.push_back(idx.vertex_index);
    }
    if (idx.normal_index != 0) {
      mesh->vn_indices.push_back(idx.normal_index);
    }
    if (idx.texcoord_index != 0) {
      mesh->vt_indices.push_back(idx.texcoord_index);
    }
  }
}

void usemtl_cb(void *user_data, const char *name, int material_idx) {
  MyMesh *mesh = reinterpret_cast<MyMesh *>(user_data);
  if ((material_idx > -1) && (material_idx < mesh->materials.size())) {
    printf("usemtl. material id = %d(name = %s)\n", material_idx,
           mesh->materials[material_idx].name.c_str());
  } else {
    printf("usemtl. name = %s\n", name);
  }
}

void mtllib_cb(void *user_data, const tinyobj::material_t *materials,
               int num_materials) {
  MyMesh *mesh = reinterpret_cast<MyMesh *>(user_data);
  printf("mtllib. # of materials = %d\n", num_materials);

  for (int i = 0; i < num_materials; i++) {
    mesh->materials.push_back(materials[i]);
  }
}

void group_cb(void *user_data, const char **names, int num_names) {
  // MyMesh *mesh = reinterpret_cast<MyMesh*>(user_data);
  printf("group : name = \n");

  for (int i = 0; i < num_names; i++) {
    printf("  %s\n", names[i]);
  }
}

void object_cb(void *user_data, const char *name) {
  // MyMesh *mesh = reinterpret_cast<MyMesh*>(user_data);
  printf("object : name = %s\n", name);
}

MyMesh cube;

Mesh::Mesh(glm::vec3 position, float scale, Material *material,
           std::string file_name)
    : Object(position, scale, material) {
  tinyobj::callback_t cb;
  cb.vertex_cb = vertex_cb;
  cb.normal_cb = normal_cb;
  cb.texcoord_cb = texcoord_cb;
  cb.index_cb = index_cb;
  cb.usemtl_cb = usemtl_cb;
  cb.mtllib_cb = mtllib_cb;
  cb.group_cb = group_cb;
  cb.object_cb = object_cb;

  std::string warn;
  std::string err;
  std::string filename = file_name;
  std::ifstream ifs(filename.c_str());

  if (ifs.fail()) {
    std::cerr << "file not found." << std::endl;
    std::terminate();
  }

  tinyobj::MaterialFileReader mtlReader("../../models/");

  bool ret = tinyobj::LoadObjWithCallback(ifs, cb, &cube, &mtlReader, &err);

  if (!warn.empty()) {
    std::cout << "WARN: " << warn << std::endl;
  }

  if (!err.empty()) {
    std::cerr << err << std::endl;
  }

  if (!ret) {
    std::cerr << "Failed to parse .obj" << std::endl;
    std::terminate();
  }

  printf("# of vertices         = %ld\n", cube.vertices.size());
  printf("# of normals          = %ld\n", cube.normals.size());
  printf("# of texcoords        = %ld\n", cube.texcoords.size());
  printf("# of vertex indices   = %ld\n", cube.v_indices.size());
  printf("# of normal indices   = %ld\n", cube.vn_indices.size());
  printf("# of texcoord indices   = %ld\n", cube.vt_indices.size());
  printf("# of materials = %ld\n", cube.materials.size());

  for (auto &figure : cube.vertices) {
    figure *= GetScale();
    figure += GetPosition();
  }
}

float rayTriangleIntersect(const Ray &ray, const glm::vec3 &v0,
                           const glm::vec3 &v1, const glm::vec3 &v2,
                           const glm::vec3 n) {
  float p = glm::dot(ray.direction, n);

  if (p >= 0) return -1.0f;

  float d = glm::dot(v0 - ray.point, n) / p;

  // compute the intersection point using equation 1
  glm::vec3 P = ray.point + d * ray.direction;

  // Step 2: inside-outside test
  glm::vec3 C;  // vector perpendicular to triangle's plane

  // edge 0
  glm::vec3 edge0 = v1 - v0;
  glm::vec3 vp0 = P - v0;
  C = glm::cross(edge0, vp0);
  if (glm::dot(n, C) < 0) return -1.0f;  // P is on the right side

  // edge 1
  glm::vec3 edge1 = v2 - v1;
  glm::vec3 vp1 = P - v1;
  C = glm::cross(edge1, vp1);
  if (glm::dot(n, C) < 0) return -1.0f;  // P is on the right side

  // edge 2
  glm::vec3 edge2 = v0 - v2;
  glm::vec3 vp2 = P - v2;
  C = glm::cross(edge2, vp2);
  if (glm::dot(n, C) < 0) return -1.0f;  // P is on the right side;

  return d;
}

float Mesh::Intersect(Ray ray, Ray &normal_ray, glm::vec2 &uv) const {
  float min_t = 999999;
  glm::vec3 min_normal;

  bool find = false;
  for (size_t i = 0; i < cube.v_indices.size(); i += 3) {
    glm::vec3 v0 = cube.vertices[cube.v_indices[i] - 1];
    glm::vec3 v1 = cube.vertices[cube.v_indices[i + 1] - 1];
    glm::vec3 v2 = cube.vertices[cube.v_indices[i + 2] - 1];
    glm::vec3 n = cube.normals[cube.vn_indices[i] - 1];

    float t = rayTriangleIntersect(ray, v0, v1, v2, n);
    if (t > 0 && min_t > t) {
      min_t = t;
      find = true;
      min_normal = n;
    }
  }
  if (find) {
    normal_ray.point = ray.point + min_t * ray.direction;
    normal_ray.direction = glm::normalize(min_normal);
    uv = {0, 0};
    return min_t;
  }
  return -1;
}

Triangle::Triangle(glm::vec3 position, float scale, Material *material,
                   glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 n)
    : Object(position, scale, material), v0(v0), v1(v1), v2(v2), n(n) {}

float Triangle::Intersect(Ray ray, Ray &normal_ray, glm::vec2 &uv) const {
  float p = glm::dot(ray.direction, n);

  if (p >= 0) return -1.0f;

  float d = glm::dot(v0 - ray.point, n) / p;

  // compute the intersection point using equation 1
  glm::vec3 P = ray.point + d * ray.direction;

  // Step 2: inside-outside test
  glm::vec3 C;  // vector perpendicular to triangle's plane

  // edge 0
  glm::vec3 edge0 = v1 - v0;
  glm::vec3 vp0 = P - v0;
  C = glm::cross(edge0, vp0);
  if (glm::dot(n, C) < 0) return -1.0f;  // P is on the right side

  // edge 1
  glm::vec3 edge1 = v2 - v1;
  glm::vec3 vp1 = P - v1;
  C = glm::cross(edge1, vp1);
  if (glm::dot(n, C) < 0) return -1.0f;  // P is on the right side

  // edge 2
  glm::vec3 edge2 = v0 - v2;
  glm::vec3 vp2 = P - v2;
  C = glm::cross(edge2, vp2);
  if (glm::dot(n, C) < 0) return -1.0f;  // P is on the right side;

  return d;
}
