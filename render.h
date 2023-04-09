#pragma once
#include <inttypes.h>
#include "materials.h"
#include "figures.h"

#include <functional>
#include <mutex>
#include <condition_variable>

#define WIDTH 800
#define HEIGHT 800

extern std::vector<Sphere> objects;

void render(uint32_t* scene);
bool find_interception(glm::vec3 ray_point, glm::vec3 ray_normal, glm::vec3& point, glm::vec3& normal, size_t& obj_index);

class ThreadPool {
	std::mutex mutex;
	std::condition_variable variable;
	std::atomic<bool> working = false;
	bool exit = false;
	std::atomic<size_t> counter = 0;
	std::vector <std::function<void(void)>> tasks;
	std::vector <std::thread*> threads;
public:
	ThreadPool(std::vector <std::function<void(void)>> tasks) :
	tasks(tasks)
	{
		for (size_t i = 0; i < tasks.size();  i++) {
			threads.push_back(new std::thread(std::bind(&ThreadPool::thread_code, this, i)));
		}
	}
	~ThreadPool() {
		exit = true;
		for (size_t i = threads.size() - 1; i >= 0; i--) {
			if(threads[i]->joinable())
				threads[i]->join();
			delete threads[i];
			threads.pop_back();
		}
	}
	void Run() {

		working = true;
		variable.notify_all();
		while (counter != ((1<<tasks.size()))-1)
			std::this_thread::yield();
		working = false;
		counter = 0;
	}
private:
	void thread_code(size_t i) {
		while (exit == false) {
			{
				std::unique_lock guard(mutex);
				while (working == false)
					variable.wait(guard);
			}
			tasks[i]();
			counter |= (1ULL << i);
		}
	}
};
