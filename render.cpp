#include "render.h"
#include "figures.h"
#include "glm/gtx/transform.hpp"
#include "skybox.h"

#include <array>
#include <random>
#include <fstream>
#include <chrono>
#include <iostream>

#include "nrandom.h"

glm::vec3 color = { 0.76,0,0 };

Material mm00( color, 0, 1,0 );
Material mm10( color, 0, 0.8,0 );
Material mm20( color, 0, 0.2,0 );
Material mm30( color, 0, 0, 0.0 );
Material mm01( color, 0, 1, 0.2 );
Material mm11( color, 0, 0.8, 0.2);
Material mm21( color, 0, 0.8, 0.2 );
Material mm31( color, 0, 0,  0.2 );
Material mm02( color, 0, 1, 0.8);
Material mm12( color, 0, 0.2, 0.8 );
Material mm22( color, 0, 0.22, 0.8);
Material mm32( color, 0, 0, 0.8);
Material mm03( color, 0, 1, 1);
Material mm13( color, 0, 0.8, 1 );
Material mm23( color, 0, 0.2, 1);
Material mm33( color, 0, 0,  1);

Material light({0.5, 0.1, 0.1}, 0, 0.7, 0.04);
Material light1({0.1, 0.1, 0.1}, 0, 0.05, 0.1);
Material light2({0.05, 0.05, 0.1}, 0, 0.01, 0.95);

std::mt19937_64 random;

std::vector<Sphere> objects{Sphere(glm::vec3{2, 1, 0}, 1, light1),
                            Sphere(glm::vec3{0, 1, 0}, 1, light),
                            Sphere(glm::vec3{-2, 1, 0}, 1, light2),
                            Sphere(glm::vec3{2, -1, 0}, 1, light1),
                            Sphere(glm::vec3{2, -1, 0}, 1, light2),
                            Sphere(glm::vec3{0, -1, 0}, 1, light),
                            Sphere(glm::vec3{-2, 0, 0}, 1, light),
                            Sphere(glm::vec3{0, 0, 0}, 1, light2),
                            Sphere(glm::vec3{-2, 0, 0}, 1, light)};

glm::vec3 normal_color(glm::vec3 color) {
	color.x = std::min(color.x, 1.0f);
	color.y = std::min(color.y, 1.0f);
	color.z = std::min(color.z, 1.0f);
	color.x = std::max(color.x, 0.0f);
	color.y = std::max(color.y, 0.0f);
	color.z = std::max(color.z, 0.0f);
	return color;
}

uint32_t vectocolor(glm::vec3 color){
	color = normal_color(color);
	uint8_t r = 255*color.x;
	uint8_t g = 255*color.y;
	uint8_t b = 255*color.z;
	return ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b << 0);
}

glm::vec3 tonemapping(glm::vec3 color)
{
	//return { sqrt(color.x),sqrt(color.y),sqrt(color.z)};
	return color;
//	return color;
}

bool find_interception(glm::vec3 ray_point, glm::vec3 ray_normal, glm::vec3 &point, glm::vec3 &normal , size_t &obj_index) 
{
/*
	auto test = [&](size_t obj, glm::vec3& current_point, glm::vec3& current_normal) {
		return objects[obj].InterSect(ray_point, ray_normal, current_point, current_normal) == true &&
			(glm::dot(ray_normal, (current_point - ray_point)) > 0);
	};
	*/

	size_t i = 0;

	glm::vec3 min_point;
	glm::vec3 min_normal;

	while ( i < objects.size() && !(objects[i].InterSect(ray_point, ray_normal, min_point, min_normal) == true &&
			(glm::dot(ray_normal, (min_point - ray_point)) > 0)) )
		i++;
	obj_index = i;

	if (i == objects.size())
		return false;

	for (;i<objects.size();++i) {
		glm::vec3 current_point, current_normal;
		if ( objects[i].InterSect(ray_point, ray_normal, current_point, current_normal) == true &&
			(glm::dot(ray_normal, (current_point - ray_point)) > 0) ) {
			if (glm::distance(min_point, ray_point) > glm::distance(current_point, ray_point)) {
				min_point = current_point;
				min_normal = current_normal;
				obj_index = i;
			}
		}
	}
	point = min_point;
	normal = min_normal;
	return true;
}

SkyBox skybox;

glm::vec3 RayTrace(glm::vec3 ray_point, glm::vec3 ray_normal, size_t depth)
{
    glm::vec3 o_point, o_normal;
    size_t obj_index;
    bool intersect = find_interception(ray_point, ray_normal, o_point, o_normal, obj_index);

    if (intersect == false) {
        /*
*/
        return skybox.GetColor(glm::normalize(ray_normal));
    }

    if (depth == 0) {
        return {0, 0, 0};
    }

    auto light_power = objects[obj_index].m_material.light_power_;
    if (light_power > 0) {
        auto own_light = objects[obj_index].m_material.light_power_
                         * objects[obj_index].m_material.diffuse_;
        return own_light;
    }

    auto specular = objects[obj_index].m_material.specular_;
    auto glossiness = objects[obj_index].m_material.glossiness_;
    auto diffuse = objects[obj_index].m_material.diffuse_;

    auto reflected_v = glm::reflect(ray_normal, o_normal);
    auto reflected_angle = glm::dot(reflected_v, o_normal);
    glm::vec3 diffuse_v = glm::normalize(RandomDirection() + o_normal);
    if (glm::dot(diffuse_v, o_normal) < 0.0)
        diffuse_v = -diffuse_v;
    diffuse_v = glm::normalize(diffuse_v + o_normal);

    auto ra = specular + (1 - specular) * pow(1 - reflected_angle, 5);

    auto new_ray = glm::normalize(glm::mix(reflected_v, diffuse_v, glossiness));

    auto in_light = RayTrace(o_point, new_ray, depth - 1);

    auto light = in_light;

    auto color = glm::mix(light * diffuse, light, ra);

    return color;
}

glm::vec3 buffer[WIDTH][HEIGHT] = {};
size_t iteration = 1;

size_t iterations_per_frame = 5;

float screen_ratio = (float)WIDTH / HEIGHT;
float scale_factor = 1;

void task(size_t current, size_t max)
{
    glm::vec3 projection_center{0, 0, -4};
    size_t delta = WIDTH / max;
    for (size_t x = current; x < WIDTH; x += max) {
        for (size_t y = 0; y < HEIGHT; y++) {
            glm::vec3 ray_point{(-2.0 * x / WIDTH + 1) * screen_ratio, -2.0 * y / HEIGHT + 1, -2};
            ray_point *= scale_factor;
            glm::vec3 ray_normal = glm::normalize(ray_point - projection_center);

            auto delta = RandomDirection();
            delta /= 1000;

            ray_point += delta;

            buffer[x][y] *= iteration;

            for (size_t i = 0; i < iterations_per_frame; i++) {
                auto color = RayTrace(ray_point, ray_normal, 10);

                buffer[x][y] += color;
            }

            buffer[x][y] /= iteration + iterations_per_frame;
        }
    }
}

std::vector < std::function<void(void)>> tasks{
	std::bind(&task,0,5),
	std::bind(&task,1,5),
	std::bind(&task,2,5),
	std::bind(&task,3,5),
	std::bind(&task,4,5)
};

ThreadPool pool(tasks);

void render(uint32_t* scene) {
	auto begin = std::chrono::high_resolution_clock::now();

	/*
	for (size_t x = 0; x < WIDTH; x++) {
		for (size_t y = 0; y < HEIGHT; y++) {
			buffer[x][y] = {};
		}
	}
	*/
	using namespace std;
	
	pool.Run();

//		task(0, 2);
//		task(1, 2);

	for (size_t x = 0; x < WIDTH; x++) {
		for (size_t y = 0; y < HEIGHT; y++) {
			scene[WIDTH * y + x] = vectocolor(tonemapping(buffer[x][y]));
		}
	}
	iteration+=iterations_per_frame;

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
}
