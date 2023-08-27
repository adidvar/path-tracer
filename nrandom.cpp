#include "nrandom.h"

#include <chrono>
#include <random>
#include <thread>

static std::mt19937 gen;
float rvalue() { return (float)gen() / (float)gen.max(); }

inline float rnvalue()
{
    float theta = 2 * 3.1415926 * rvalue();
    float rho = sqrt(-2 * log(rvalue()));
    return rho * cos(theta);
}

glm::vec3 RandomDirection()
{
    return {rnvalue(), rnvalue(), rnvalue()};
}
