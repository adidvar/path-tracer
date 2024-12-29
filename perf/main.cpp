#include <aabb.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <figures.hpp>
#include <glm/glm.hpp>
#include <material.hpp>

TEST_CASE("Object Intersection Tests", "[object]") {
  Material material(glm::vec3(0.5f, 0.5f, 0.5f), 10.0f, 0.8f, 0.5f);

  SECTION("Sphere Intersection") {
    Sphere sphere(glm::vec3(1.0f, 0.0f, 0.0f), 0.1f, &material);
    Ray ray{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};
    Ray normal_ray;
    glm::vec2 uv;

    float distance = sphere.Intersect(ray, normal_ray, uv);
    REQUIRE(distance > 0.0f);  // Перевірка, чи є перетин
  }

  SECTION("Plane Intersection") {
    Plane plane(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                &material);
    Ray ray{{0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}};
    Ray normal_ray;
    glm::vec2 uv;

    float distance = plane.Intersect(ray, normal_ray, uv);
    REQUIRE(distance > 0.0f);  // Перевірка, чи є перетин
  }

  SECTION("Triangle Intersection") {
    Triangle triangle(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, &material,
                      glm::vec3(-1.0f, -1.0f, 0.0f),
                      glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                      glm::vec3(0.0f, 0.0f, 1.0f));
    Ray ray{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}};
    Ray normal_ray;
    glm::vec2 uv;

    float distance = triangle.Intersect(ray, normal_ray, uv);
    REQUIRE(distance > 0.0f);  // Перевірка, чи є перетин
  }
}

TEST_CASE("Intersection Benchmarks", "[benchmark]") {
  Material material(glm::vec3(0.5f, 0.5f, 0.5f), 10.0f, 0.8f, 0.5f);

  Ray ray{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};
  Ray normal_ray;
  glm::vec2 uv;

  SECTION("Sphere Intersection Benchmark") {
    Sphere sphere(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, &material);

    BENCHMARK("Sphere Intersect") {
      return sphere.Intersect(ray, normal_ray, uv);
    };
  }

  SECTION("Plane Intersection Benchmark") {
    Plane plane(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                &material);

    BENCHMARK("Plane Intersect") {
      return plane.Intersect(ray, normal_ray, uv);
    };
  }

  SECTION("Triangle Intersection Benchmark") {
    Triangle triangle(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, &material,
                      glm::vec3(-1.0f, -1.0f, 0.0f),
                      glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                      glm::vec3(0.0f, 0.0f, 1.0f));
    Ray ray{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}};

    BENCHMARK("Triangle Intersect") {
      return triangle.Intersect(ray, normal_ray, uv);
    };
  }
}

TEST_CASE("AABB Benchmarks", "[benchmark][AABB]") {
  AABB aabb1(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
  AABB aabb2(glm::vec3(-2.0f, -2.0f, -2.0f), glm::vec3(2.0f, 2.0f, 2.0f));
  Ray ray{glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f)};
  glm::vec3 testPoint(0.5f, 0.5f, 0.5f);
  float t_min, t_max;

  BENCHMARK("AABB Intersect") { return aabb1.Intersect(ray, t_min, t_max); };

  BENCHMARK("AABB DistanceToCenter") {
    return aabb1.DistanceToCenter(testPoint);
  };

  BENCHMARK("AABB Merge") { return AABB::Merge(aabb1, aabb2); };
}
