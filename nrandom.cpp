#include "nrandom.h"

#include <random>

extern std::mt19937 gen;

inline float rvalue()
{
    return (float) rand() / RAND_MAX;
}

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
