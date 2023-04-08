#include "render.h"
#include "figures.h"

#include <array>
#include <random>
#include <fstream>
#include <chrono>
#include <iostream>

glm::vec3 environment = glm::vec3(0.01, 0.01, 0.01);


Material red( glm::vec3(1, 1, 1), false, 0.5 );
Material mate( glm::vec3(1, 0.9, 1), false, 1 );
Material mirror( glm::vec3(1, 1, 1), false, 0.05 );
Material light2( glm::vec3(1, 1, 0.2), true, 0 );
Material light3( glm::vec3(1, 1, 1), true, 0 );
Material light4( glm::vec3(1, 0, 0.2), true, 0 );
Material light5( glm::vec3(0, 1, 1), true, 0 );
Material light6( glm::vec3(1,1 , 1), true, 0.5 );

std::mt19937_64 random;

std::vector<Sphere> objects{
Sphere(glm::vec3{-0.5,0,2} , 0.3 , mate),
Sphere(glm::vec3{0,-0.5,2} , 0.3 , mate),
Sphere(glm::vec3{-0.5,-0.5,2} , 0.4 , mate),
Sphere(glm::vec3{+0.5,+0.5,2} , 0.4 , mate),
Sphere(glm::vec3{0.5,-0.5,2} , 0.4 , mate),
Sphere(glm::vec3{-0.5,0.5,2} , 0.4 , mate),
Sphere(glm::vec3{0.5,0,2} , 0.3 , mate),
Sphere(glm::vec3{0,0.5,2} , 0.3 , mate),
Sphere(glm::vec3{0,0,2} , 0.2 , red),
Sphere(glm::vec3{0,-9,3} , 6 , light2),
Sphere(glm::vec3{0,9,3} , 6 , light3),
Sphere(glm::vec3{-9,0,3} , 6 , light4),
Sphere(glm::vec3{9,0,3} , 6 , light5),
Sphere(glm::vec3{-0.18,-0.18,1.8} , 0.06 , light6)
};

uint32_t vectocolor(glm::vec3 color){
	uint8_t r = 255*color.x;
	uint8_t g = 255*color.y;
	uint8_t b = 255*color.z;
	return ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b << 0);
}

glm::vec3 tonemapping(glm::vec3 color)
{
	return { sqrt(color.x),sqrt(color.y),sqrt(color.z)};
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

glm::vec3 RayTrace(glm::vec3 ray_point , glm::vec3 ray_normal , size_t depth) 
{
	glm::vec3 point, normal;
	size_t obj_index;
	bool intersect = find_interception(ray_point, ray_normal, point, normal, obj_index);

	if (intersect == false) {
		return environment;
	}

	if (objects[obj_index].m_material.is_light())
	{
		return objects[obj_index].m_material.Light();
	}

	if (depth == 0) {
		return { 0,0,0 };
	}
	//glm::vec3 reflected = glm::reflect(ray_normal,normal);
	glm::vec3 reflected = objects[obj_index].m_material.reflect(normal, ray_normal);
	glm::vec3 reflected_color = 0.7f * RayTrace(point, reflected, depth - 1);
	return objects[obj_index].m_material.Color(reflected_color);
}

glm::vec3 buffer[WIDTH][HEIGHT] = {};
size_t iteration = 1;

void task(size_t current , size_t max) {
	glm::vec3 projection_center{ 0, 0, -999999 };
	size_t delta = WIDTH / max;
	for (size_t x = current; x < WIDTH; x+=max) {
		for (size_t y = 0; y < HEIGHT; y++) {
	
			glm::vec3 ray_point{ -2.0 * x / WIDTH +1 , -2.0 * y / HEIGHT + 1 , -2 };
			glm::vec3 ray_normal = glm::normalize(ray_point-projection_center);

			auto color = RayTrace(ray_point, ray_normal, 4);

			buffer[x][y] = (color+buffer[x][y]*(float)iteration)/(float)(iteration+1);
		}
	}
}

std::vector < std::function<void(void)>> tasks{
	std::bind(&task,0,5),
	std::bind(&task,1,5),
	std::bind(&task,2,5),
	std::bind(&task,3,5),
	std::bind(&task,4,5)
//	std::bind(&task,4,5)
};

ThreadPool pool(tasks);

void render(uint32_t* scene) {
	auto begin = std::chrono::high_resolution_clock::now();
	
	pool.Run();

//		task(0, 2);
//		task(1, 2);

	for (size_t x = 0; x < WIDTH; x++) {
		for (size_t y = 0; y < HEIGHT; y++) {
			scene[WIDTH * y + x] = vectocolor(tonemapping(buffer[x][y]));
		}
	}
	iteration++;

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
}
