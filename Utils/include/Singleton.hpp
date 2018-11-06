#ifndef SINGLETON_HPP
#define SINGLETON_HPP

// 饿汉式Type-1
class SingletonType1
{
private:
  SingletonType1() {}
  SingletonType1(const SingletonType1&) {}
  SingletonType1& operator=(const SingletonType1&) {}
  ~SingletonType1() {}
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
  SingletonType2() {}
  SingletonType2(const SingletonType1&) {}
  SingletonType2& operator=(const SingletonType1&) {}
  ~SingletonType2() {}

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

#endif