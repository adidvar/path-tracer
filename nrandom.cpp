#include "nrandom.h"

#include <chrono>
#include <random>
#include <thread>

//thread_local static std::mt19937 gen;

thread_local static uint64_t x{}; /* The state can be seeded with any value. */
uint64_t next()
{
    uint64_t z = (x += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
}

float RandomValue() { return (float)next() / (float)std::numeric_limits<uint64_t>::max(); };


/*
void RandomInit() {
  x = std::chrono::high_resolution_clock::now()
               .time_since_epoch()
               .count();

}
*/

inline float rnvalue() {
  float theta = 2.0f * 3.1415926f * RandomValue();
  float rho = sqrt(-2.0f * log(RandomValue()));
  return rho * cos(theta);
}

const size_t size = 2048;
thread_local float hashed[size];
thread_local int index = 0;

void RandomInit() {
  x = std::chrono::high_resolution_clock::now()
               .time_since_epoch()
               .count();
  for (size_t i = 0; i < size; i++) hashed[i] = rnvalue();
}

inline float rnvalue_hashed() {
  index++;
  if (index > size) index = 0;
  return hashed[index];
}

glm::vec3 RandomDirection() {
  return {rnvalue(), rnvalue(), rnvalue()};
  // return {rnvalue_hashed(), rnvalue_hashed(), rnvalue_hashed()};
}
