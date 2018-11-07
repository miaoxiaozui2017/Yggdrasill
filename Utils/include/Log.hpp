#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include "Singleton.hpp"

class Log
{
public:
  Log() = default;
  ~Log() = default;
  template <typename... Args>
  void WriteLog(const char* file, const long& line, const char* func, Args&&... args)
  {
    std::cerr << file << "(" << line << ")[" << func << "]: ";
    TextLog(args...);
  }
private:
  template <typename T, typename... Args>
  void TextLog(const T& t, Args&&... args)
  {
    std::cerr << t;
    TextLog(args...);
  }

  template <typename T>
  void TextLog(const T& t)
  {
    std::cerr << t << std::endl;
  }

};

#define LOG1(...) Singleton<Log>::GetInstancePointer()->WriteLog(__FILE__, __LINE__,__FUNCTION__, ##__VA_ARGS__)

#endif