#include "nrandom.h"

#include <chrono>
#include <random>
#include <thread>

static thread_local uint64_t x{}; /* The state can be seeded with any value. */
uint32_t next() {
  uint64_t z = (x += UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

#include <emmintrin.h>
#include <immintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>

class MyGen {
 public:
  using result_type = uint32_t;

  result_type max() const { return std::numeric_limits<result_type>::max(); };
  result_type min() const { return std::numeric_limits<result_type>::min(); };
  result_type operator()() const { return next(); };
} mygen;

float uni_real_dist() { return (float)mygen() / mygen.max(); }

std::normal_distribution<float> distribution;

glm::vec3 RandomDirection() {
  const thread_local auto cvect = _mm_set_ps1(2.0f * 3.1415926f);
  const thread_local auto vvect = _mm_set_ps1(-2.0);

  auto first = _mm_setr_ps(uni_real_dist(), uni_real_dist(),
                           uni_real_dist(), 0.0f);
  auto r1 = _mm_mul_ps(first, cvect);

  alignas(16) float vect[4];
  _mm_store_ps(vect, r1);
  vect[0] = std::cos(vect[0]);
  vect[1] = std::cos(vect[1]);
  vect[2] = std::cos(vect[2]);
  r1 = _mm_load_ps(vect);

  auto second =
      _mm_setr_ps(log(uni_real_dist()), log(uni_real_dist()),
                  log(uni_real_dist()), 0);
  auto mulled = _mm_mul_ps(second, vvect);
  auto r2 = _mm_sqrt_ps(mulled);

  auto result = _mm_mul_ps(r1, r2);

  auto dp = _mm_dp_ps(result, result, 0x7F);

  dp = _mm_rsqrt_ps(dp);

  result = _mm_mul_ps(result, dp);

  alignas(16) float res[4];
  _mm_store_ps(res, result);

  return {res[0], res[1], res[2]};
}

float RandomValue() { return distribution(mygen); }

void RandomInit() { x = rand(); }
