#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

/*

class ThreadPool {
 public:
  ThreadPool(std::function<void(int, int)> function);
  ~ThreadPool();

  void Run();

 private:
  void ThreadCode(size_t i, std::function<void(void)> function);

 private:
  std::mutex m_mutex;
  std::condition_variable m_variable;
  std::atomic<uint64_t> m_flags = 0;

  std::atomic<bool> m_exit = false;

  std::vector<std::unique_ptr<std::thread>> m_threads;
};
*/

#endif
