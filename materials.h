#pragma once
#include <glm/glm.hpp>
#include <random>

struct Material
{
    Material(glm::vec3 diffuse, float light_power, float glossiness, float specular)
        : diffuse_(diffuse)
        , light_power_(light_power)
        , glossiness_(glossiness)
        , specular_(specular)
    {}

    glm::vec3 diffuse_;
    float light_power_;
    float glossiness_;
    float specular_;
};
