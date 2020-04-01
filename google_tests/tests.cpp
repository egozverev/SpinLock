#include "gtest/gtest.h"
#include "../spinlock.hpp"

TEST(LockTests, LockUnlock) {
  fairlock::SpinLock spinlock;
  spinlock.Lock();
  spinlock.Unlock();
}

TEST(LockTests, LockUnlockKTimes) {
  fairlock::SpinLock spinlock;
  const size_t iter_num = 16;
  for (int i = 0; i < iter_num; ++i) {
    spinlock.Lock();
    spinlock.Unlock();
  }
}

TEST(TryLockTests, TryAfterTry) {
  fairlock::SpinLock spinlock;
  ASSERT_TRUE(spinlock.TryLock());
  ASSERT_FALSE(spinlock.TryLock());
  spinlock.Unlock();
  ASSERT_TRUE(spinlock.TryLock());
  spinlock.Unlock();
}

TEST(TryLockTests, TryAfterLock) {
  fairlock::SpinLock spinlock;
  spinlock.Lock();
  ASSERT_FALSE(spinlock.TryLock());
  spinlock.Unlock();
}