#include <PathTracer/pathtracer.hpp>

/*
struct GLight {
  glm::vec3 color;
  glm::vec3 direction;
  float edge;
  float power;
};

std::vector<GLight> kglights{
    {{1, 1, 1}, glm::normalize(glm::vec3{0, 1, 0}), 0.97, 7},
    {{1, 1, 1}, glm::normalize(glm::vec3{0, 1, 0}), 0.3, 0.3},
    //{{1, 1, 1}, glm::normalize(glm::vec3{-1, 0, +0.3}), 0.0, 0.1},
    //{{1, 1, 1}, glm::normalize(glm::vec3{1, 0, -0.3}), 0.0, 0.1},
};

SkyBox::SkyBox()
{
    std::vector<unsigned char> buffer;
    //lodepng::load_file(buffer, "skybox.png"); //load the image file with given
    filename unsigned error = lodepng::decode(image_buffer_, width_, height_,
    buffer); //decode the png

    //stop if there is an error
    if (error) {
        std::cout << "decoder error " << error << ": " <<
    lodepng_error_text(error) << std::endl; exit(0);
    }
}

const float pi = 3.14159265f;

glm::vec3 SkyBox::GetColor(glm::vec3 coord) {
  return GetGLight(coord);  // + GetSkyBox(coord);
    // return {0, 0, 0};
}

glm::vec3 SkyBox::GetGLight(glm::vec3 coord)
{
    glm::vec3 gillumination = {0, 0, 0};
    for (auto light : kglights) {
        auto cosine = glm::dot(coord, light.direction);
        if (cosine > light.edge)
            gillumination += (cosine - light.edge) * (1 / (1 - light.edge)) *
light.power
                             * light.color;
    }
    return gillumination;
}

glm::vec3 SkyBox::GetSkyBox(glm::vec3 coord)
{
    float u, v;
    u = 0.5 + atan2(coord.z, coord.x) / 2 / pi;
    v = 0.5 - asin(coord.y) / pi;

    u *= width_ - 1;
    v *= height_ - 1;

    unsigned int index = (unsigned int) v * width_ + (unsigned int) u;
    index *= 4;

    glm::vec3 skybox_color = {image_buffer_[index + 0],
                              image_buffer_[index + 1],
                              image_buffer_[index + 2]};
    skybox_color.x /= 255.0;
    skybox_color.y /= 255.0;
    skybox_color.z /= 255.0;

    return skybox_color;
}
*/

Skybox::Skybox() {}

glm::vec3 Skybox::Get(glm::vec3 direction) { return {1, 1, 1}; }
