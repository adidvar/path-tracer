#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>
#include <lodepng.h>

class SkyBox
{
public:
    SkyBox();
    glm::vec3 GetColor(glm::vec3 coord);
    glm::vec3 GetGLight(glm::vec3 coord);
    glm::vec3 GetSkyBox(glm::vec3 coord);

private:
    std::vector<unsigned char> image_buffer_;
    unsigned int height_;
    unsigned int width_;
};

#endif
