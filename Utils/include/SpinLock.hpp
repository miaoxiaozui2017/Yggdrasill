#ifndef SPINLOCK_HPP
#define SPINLOCK_HPP

#include <atomic>

class SpinLock final
{
public:
  inline SpinLock()
    : m_atomicLock(false)
  {}
  SpinLock(SpinLock const &) = default;
  SpinLock(SpinLock &&) = default;
  SpinLock &operator=(SpinLock const &) = default;
  SpinLock &operator=(SpinLock &&) = default;
  ~SpinLock() = default;

  inline void lock()
  {
    while (m_atomicLock.exchange(true));
  }
  inline void unlock()
  {
    m_atomicLock = false;
  }
  inline bool isLocked() const
  {
    return m_atomicLock;
  }

private:
  std::atomic_bool m_atomicLock;
};

#endif