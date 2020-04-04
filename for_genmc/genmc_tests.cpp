#include "../spinlock.hpp"

#include <cassert>

#include <mutex>

#include <vector>

int main(){
  const size_t sz = 1024;
  const size_t iterations = 4096;
  std::vector<int> array(sz, 0);
  std::vector<fairlock::SpinLock> spinlocks(sz);
  std::thread thread1([&]() {
    const size_t magic_const = 9679;
    for (size_t iter = 0; iter < iterations; ++iter) {
      size_t start = (iter * magic_const) % sz;
      for (size_t i = 0; i < sz; ++i) {
        size_t index = (start + i) % sz;
        std::lock_guard<fairlock::SpinLock> guard(spinlocks[index]);
        ++array[index];
      }
    }
  });
  std::thread thread2([&]() {
    const size_t magic_const = 139;
    for (size_t iter = 0; iter < iterations; ++iter) {
      size_t start = (iter * magic_const) % sz;
      for (int i = 0; i < sz; ++i) {
        size_t index = (start + i) % sz;
        std::lock_guard<fairlock::SpinLock> guard(spinlocks[index]);
        --array[index];
      }
    }
  });
  thread1.join();
  thread2.join();
  for (int elem: array) {
    assert(elem == 0);
  }
}