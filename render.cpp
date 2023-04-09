#include "render.h"
#include "figures.h"

#include <array>
#include <random>
#include <fstream>
#include <chrono>
#include <iostream>


glm::vec3 color = { 1,0,0 };

Material mm00( color, 0, 1,0 );
Material mm10( color, 0, 0.66,0 );
Material mm20( color, 0, 0.33,0 );
Material mm30( color, 0, 0, 0.0 );
Material mm01( color, 0, 1, 0.33 );
Material mm11( color, 0, 0.66, 0.33);
Material mm21( color, 0, 0.33, 0.33 );
Material mm31( color, 0, 0,  0.33 );
Material mm02( color, 0, 1, 0.66);
Material mm12( color, 0, 0.66, 0.66 );
Material mm22( color, 0, 0.33, 0.66);
Material mm32( color, 0, 0, 0.66);
Material mm03( color, 0, 1, 1);
Material mm13( color, 0, 0.66, 1 );
Material mm23( color, 0, 0.33, 1);
Material mm33( color, 0, 0,  1);
Material light({ 1,1,0.7 }, 8, 0, 0);

std::mt19937_64 random;


std::vector<Sphere> objects{
Sphere(glm::vec3{-0.6,0.6,-1.8} , 0.2 , mm00),
Sphere(glm::vec3{-0.2,0.6,-1.8} , 0.2 , mm10),
Sphere(glm::vec3{ 0.2,0.6,-1.8} , 0.2 , mm20),
Sphere(glm::vec3{ 0.6,0.6,-1.8} , 0.2 , mm30),

Sphere(glm::vec3{-0.6,0.2,-1.8} , 0.2 , mm01),
Sphere(glm::vec3{-0.2,0.2,-1.6} , 0.2 , mm11),
Sphere(glm::vec3{ 0.2,0.2,-1.6} , 0.2 , mm21),
Sphere(glm::vec3{ 0.6,0.2,-1.8} , 0.2 , mm31),

Sphere(glm::vec3{-0.6,-0.2,-1.8} , 0.2 , mm02),
Sphere(glm::vec3{-0.2,-0.2,-1.6} , 0.2 , mm12),
Sphere(glm::vec3{ 0.2,-0.2,-1.6} , 0.2 , mm22),
Sphere(glm::vec3{ 0.6,-0.2,-1.8} , 0.2 , mm32),

Sphere(glm::vec3{-0.6,-0.6,-1.8} , 0.2 , mm03),
Sphere(glm::vec3{-0.2,-0.6,-1.8} , 0.2 , mm13),
Sphere(glm::vec3{ 0.2,-0.6,-1.8} , 0.2 , mm23),
Sphere(glm::vec3{ 0.6,-0.6,-1.8} , 0.2 , mm33),


//Sphere(glm::vec3{-0.0,0.0,-1.8} , 0.1 , light),

};

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

struct global_light {
	glm::vec3 color;
	glm::vec3 direction;
	float edge;
	float power;
};

std::vector<global_light> glight{
	{{1,0.8,1},glm::normalize(glm::vec3{0,1,0}),0,0.03},
	{{1,0.8,1},glm::normalize(glm::vec3{0,-1,0}),0,0.03},
	{{1,1,0.8},glm::normalize(glm::vec3{0,1,-1}),0,0.5},
	{{1,1,0.8},glm::normalize(glm::vec3{0,1,-1}),0.90,10},
//	{{1,1,1},glm::normalize(glm::vec3{0,1,1}),0.8,0.2},
//	{{1,1,1},glm::normalize(glm::vec3{-1,0,0}),0,0.2},
//	{{0,0,1},glm::normalize(glm::vec3{0,-2,-1}),0.7,1},
//	{{1,1,1},glm::normalize(glm::vec3{-1,2,-2}),0.9,2},
//    {{1,1,1},glm::normalize(glm::vec3{1,-2,3}),0.9,2},
};


glm::vec3 RayTrace(glm::vec3 ray_point , glm::vec3 ray_normal , size_t depth) 
{
	glm::vec3 o_point, o_normal;
	size_t obj_index;
	bool intersect = find_interception(ray_point, ray_normal, o_point, o_normal, obj_index);

	if (intersect == false) {
		glm::vec3 gillumination = { 0,0,0 };
		for (auto light : glight) {
			auto cosine = glm::dot(ray_normal, light.direction);
			if (cosine > light.edge)
				gillumination += (cosine - light.edge) * (1/(1-light.edge)) * light.power * light.color;
		}
		return gillumination;
	}

	if (depth == 0) {
		return { 0,0,0 };
	}

	auto smoothness = objects[obj_index].m_material.smoothness;
	auto appleness = objects[obj_index].m_material.appleness;
	auto light = objects[obj_index].m_material.light_power * objects[obj_index].m_material.color;
	auto color = objects[obj_index].m_material.color;
	auto specular_color = glm::vec3{ 0.8,0.8,0.8 };

	auto reflected = glm::reflect(ray_normal, o_normal);

	glm::vec3 diffuse = rdirection();
	if (glm::dot(diffuse, o_normal) < 0.0)
		diffuse = -diffuse;

	float isSpecular = rvalue() < (1-appleness);

	auto new_ray = glm::normalize((1 - smoothness * isSpecular ) * reflected + diffuse * smoothness * isSpecular);
	auto incoming_power = glm::dot(new_ray, o_normal);

	glm::vec3 incoming_light = incoming_power * RayTrace(o_point, new_ray, depth - 1);

	auto col =  specular_color*(1-isSpecular) + (isSpecular)*color;
	auto reflected_light = glm::vec3(incoming_light.x * col.x, incoming_light.y * col.y, incoming_light.z * col.z);

	return (glm::length(reflected_light) > glm::length(light) ? reflected_light : light);
}

glm::vec3 buffer[WIDTH][HEIGHT] = {};
size_t iteration = 1;

void task(size_t current , size_t max) {
	glm::vec3 projection_center{ 0, 0, -3 };
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
