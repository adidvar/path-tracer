#include "threadpool.h"

#include <cassert>

constexpr static uint64_t k_one = 1;

ThreadPool::ThreadPool(std::function<void(int, int)> function) {
  int threads_count = std::max<int>(std::thread::hardware_concurrency() - 1, 1);
  assert(threads_count >= 63);

  for (int i = 0; i < threads_count; i++) {
    std::function<void(void)> task = std::bind(function, i, threads_count);

    m_threads.push_back(std::make_unique<std::thread>(
        std::bind(&ThreadPool::ThreadCode, this, i, task)));
  }
}

ThreadPool::~ThreadPool() {
  m_exit = true;
  while (m_threads.size()) {
    if (m_threads.back()->joinable()) m_threads.back()->join();
    m_threads.pop_back();
  }
}

void ThreadPool::Run() {
  m_flags = (k_one << m_threads.size()) - 1;
  m_variable.notify_all();
  while (m_flags) std::this_thread::yield();
}

void ThreadPool::ThreadCode(size_t i, std::function<void()> function) {
  while (!m_exit) {
    {
      std::unique_lock guard(m_mutex);
      while ((m_flags & (k_one << i)) == 0) m_variable.wait(guard);
    }
    function();
    m_flags &= ~(k_one << i);
  }
}
