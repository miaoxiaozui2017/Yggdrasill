cmake_minimum_required(VERSION 3.0.0)
project(Yggdrasill VERSION 0.1.0)

include(CTest)
enable_testing()

include(DataTransform/CMakeLists.txt)
include(ProcessListener/CMakeLists.txt)
include(Utils/CMakeLists.txt)


add_executable(Yggdrasill main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
