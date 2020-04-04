#include "spinlock.hpp"

namespace fairlock {
  void SpinLock::lock() {
    const uint64_t ticket = next_ticket_.fetch_add(1);
    Spinner spinner;
    while (ticket != owner_ticket_.load()) {
      spinner.Spin();
    }
  }

  void SpinLock::unlock() {
    owner_ticket_.fetch_add(1);
  }

  bool SpinLock::try_lock() {
    uint64_t owner = owner_ticket_.load();
    return next_ticket_.compare_exchange_strong(owner, owner + 1);
    // if next == owner, then next = owner + 1
  }

  void Spinner::Spin() {
    if (iteration < max_iterations) {
      __asm __volatile("pause"); // Spin Wait Hint (1)
    } else {
      iteration = 0;
      std::this_thread::yield();
    }
  }
} // namespace fairlock

/* (1) https://c9x.me/x86/html/file_module_x86_id_232.html
 *  Volatile ~~ do not optimize this instruction.
 */