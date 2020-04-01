#include <atomic>
#include <cstdint>
#include <thread>

namespace fairlock { // TicketLock
  class SpinLock {
  public:
    void Lock();

    void Unlock();

    bool TryLock();

  private:
    std::atomic<uint64_t> owner_ticket_{0};
    std::atomic<uint64_t> next_ticket_{0};
  };

  class Spinner {
  public:
    void Spin();

  private:
    static const size_t max_iterations = 128;
    size_t iteration{0};
  };

} // namespace fairlock