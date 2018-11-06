#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
using namespace std;
template<typename T>
void tbLog(const T& t)
{
  cout << t << endl;
}

template<typename T, typename... Args>
void tbLog(const T& t, Args&&... args)
{
  cout << t;
  tbLog(args...);
}

#endif