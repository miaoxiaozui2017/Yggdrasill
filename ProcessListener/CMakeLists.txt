cmake_minimum_required(VERSION 3.0.0)
project(ProcessListener VERSION 0.1.0)

# include(CTest)
# enable_testing()

add_executable(ProcessListener ProcessListener/src/main.cpp)
add_executable(SigProcess ProcessListener/src/SigProcess.cpp)
add_executable(MultiSignal ProcessListener/src/MultiSignal.cpp)
add_executable(ListenProcess ProcessListener/src/ProcessListener.cpp)
target_link_libraries(ProcessListener jsoncpp UdpServer)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
