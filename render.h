#pragma once
#include <inttypes.h>
#include "materials.h"
#include "figures.h"

#include <functional>
#include <mutex>
#include <condition_variable>

#define WIDTH 1920 
#define HEIGHT 1080 

extern std::vector<Sphere> objects;

void render(uint32_t* scene);
bool find_interception(glm::vec3 ray_point, glm::vec3 ray_normal, glm::vec3& point, glm::vec3& normal, size_t& obj_index);

class ThreadPool {
	//sync
	std::mutex mutex;
	std::condition_variable variable;
	std::atomic<size_t> flags = 0;
	bool exit = false;

	size_t threads_counter;
	std::vector <std::function<void(void)>> tasks;
	std::vector <std::thread*> threads;
public:
	ThreadPool(const std::vector<std::function<void(void)>> &tasks) :
	tasks(tasks)
	{
		threads_counter = tasks.size();
		for (size_t i = 0; i < tasks.size();  i++) {
			threads.push_back(new std::thread(std::bind(&ThreadPool::thread_code, this, i)));
		}
	}
	~ThreadPool() {
		exit = true;
		while ( threads.size() ) 
		{
			if(threads.back()->joinable())
				threads.back()->join();
			delete threads.back();
			threads.pop_back();
		}
	}
	void Run() {
		flags = (1ULL << threads_counter)-1;
		variable.notify_all();
		while (flags)
			std::this_thread::yield();
	}
private:
	void thread_code(size_t i) {
		while (!exit) {
			{
				std::unique_lock guard(mutex);
				while ((flags & (1ULL << i)) == 0)
					variable.wait(guard);
			}
			tasks[i]();
			flags &= ~(1ULL << i);
		}
	}
};
