#include "nrandom.h"

#include <chrono>
#include <random>
#include <thread>

thread_local static std::mt19937 gen;

float RandomValue() { return (float)gen() / (float)gen.max(); };

void RandomInit() {
  gen.seed((std::mt19937::result_type)std::chrono::high_resolution_clock::now()
               .time_since_epoch()
               .count());
}

inline float rnvalue() {
  float theta = 2.0f * 3.1415926f * RandomValue();
  float rho = sqrt(-2.0f * log(RandomValue()));
  return rho * cos(theta);
}

glm::vec3 RandomDirection()
{
    return {rnvalue(), rnvalue(), rnvalue()};
}
