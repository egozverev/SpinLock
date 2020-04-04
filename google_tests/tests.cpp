#include "gtest/gtest.h"
#include "../spinlock.hpp"

#include <chrono>
#include <mutex>

TEST(LockTests, LockUnlock) {
  fairlock::SpinLock spinlock;
  spinlock.lock();
  spinlock.unlock();
}

TEST(LockTests, LockUnlockKTimes) {
  fairlock::SpinLock spinlock;
  const size_t iter_num = 16;
  for (size_t i = 0; i < iter_num; ++i) {
    spinlock.lock();
    spinlock.unlock();
  }
}

TEST(TryLockTests, TryAfterTry) {
  fairlock::SpinLock spinlock;
  ASSERT_TRUE(spinlock.try_lock());
  ASSERT_FALSE(spinlock.try_lock());
  spinlock.unlock();
  ASSERT_TRUE(spinlock.try_lock());
  spinlock.unlock();
}

TEST(TryLockTests, TryAfterLock) {
  fairlock::SpinLock spinlock;
  spinlock.lock();
  ASSERT_FALSE(spinlock.try_lock());
  spinlock.unlock();
}

TEST(Concurrent, TwoThreads) {
  fairlock::SpinLock spinlock;
  int x = 0;
  std::thread thread1([&x, &spinlock]() {
    std::lock_guard<fairlock::SpinLock> guard(spinlock);
    x = 1;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ASSERT_TRUE(x == 1);
  });
  std::thread thread2([&x, &spinlock]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_TRUE(x == 1);
    std::lock_guard<fairlock::SpinLock> guard(spinlock);
    x = 0;
    ASSERT_TRUE(x == 0);
  });
  thread1.join();
  thread2.join();
  ASSERT_TRUE(x == 0);
}

TEST(Concurrent, ArraySum) {
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
    ASSERT_TRUE(elem == 0);
  }
}