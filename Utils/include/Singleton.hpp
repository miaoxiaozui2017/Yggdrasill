#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <mutex>
#include "SpinLock.hpp"
// 饿汉式Type-1
class SingletonType1
{
private:
  SingletonType1() = default;
  SingletonType1(const SingletonType1&) = default;
  SingletonType1& operator=(const SingletonType1&) = default;
  ~SingletonType1() = default;
public:
  static SingletonType1* GetInstance()
  {
    static SingletonType1 myInstance;
    return &myInstance;
  }
};
// 饿汉式Type-2
class SingletonType2
{
private:
  SingletonType2() = default;
  SingletonType2(const SingletonType2&) = default;
  SingletonType2& operator=(const SingletonType2&) = default;
  ~SingletonType2() = default;

  static SingletonType2* myInstance;

public:
  static SingletonType2* GetInstance()
  {
    if (nullptr == myInstance)
    {
      myInstance = new SingletonType2();
    }
    return myInstance;
  }

// release
private:
  class Garbo
  {
  public:
    Garbo() {}
    ~Garbo()
    {
      if (nullptr != myInstance)
      {
        delete myInstance;
        myInstance = nullptr;
      }
    }
  };
  static Garbo m_garbo;
};

// final
template <typename Derived>
class Singleton
{
public:
  template <typename... Args>
  static inline Derived &GetInstance(Args &&...args)
  {
    return *GetInstancePointer(std::forward<Args>(args)...);
  }

  template <typename... Args>
  static inline Derived *GetInstancePointer(Args &&...args)
  {
    static Derived *instancePointer = CreateInstance(std::forward<Args>(args)...);
    return instancePointer;
  }

protected:
  using Access = Singleton<Derived>;
  
  Singleton() = default;
  Singleton(Singleton const &) = default;
  Singleton(Singleton &&) = default;
  Singleton &operator=(Singleton const &) = default;
  Singleton &operator=(Singleton &&) = default;

private:
  static Derived *InstancePointer;
  static SpinLock Lock;

  template <typename... Args>
  static inline Derived *CreateInstance(Args &&...args)
  {
    if (Singleton::InstancePointer == nullptr)
    {
      std::lock_guard<decltype(Singleton::Lock)> lock(Singleton::Lock);
      if (Singleton::InstancePointer == nullptr)
      {
        void *data = static_cast<void *>(GetData());
        new (data) Derived(std::forward<Args>(args)...);
        Singleton::InstancePointer = reinterpret_cast<Derived *>(data);
        std::atexit(&Singleton::DestroyInstance);
      }
    }
    return Singleton::InstancePointer;
  }

  static inline void DestroyInstance()
  {
    reinterpret_cast<Derived *>(GetData())->~Derived();
  }

  static inline unsigned char *GetData()
  {
    static unsigned char data[sizeof(Derived)];
    return data;
  }
};

template <typename Derived>
Derived *Singleton<Derived>::InstancePointer = nullptr;

template <typename Derived>
SpinLock Singleton<Derived>::Lock;


#endif