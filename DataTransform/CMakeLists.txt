cmake_minimum_required(VERSION 3.0.0)
project(DataTransform VERSION 0.1.0)

# add_definitions(-std=c++11 -g)
# find_package(Boost REQUIRED thread)

# include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/DataTransform/include)

# link_directories(${Boost_LIBRARIES})

# set(SRC_FOLDER src)
# file(GLOB_RECURSE SRC_FILES "${SRC_FOLDER}/*.cpp")

#message(${SRC_FILES})
# set(HEADER_FOLDER include)
# file(GLOB_RECURSE HEADER_FILES "${HEADER_FOLDER}/*.hpp" "${HEADER_FOLDER}/*.h")

file(GLOB_RECURSE LIBNAMES "DataTransform/include/*.hpp")
#aux_source_directory(src SRC_LIST)

foreach(SRCFILE IN LISTS SRC_LIST)
  message("src : ${SRCFILE}")
endforeach()
foreach(LIBNAME IN LISTS LIBNAMES)
  string(REGEX REPLACE "/(.*)/(.*).hpp$" "\\2" aName ${LIBNAME})
  add_library(${aName} SHARED DataTransform/src/${aName}.cpp)
endforeach()

add_executable(${PROJECT_NAME} DataTransform/src/main.cpp)
target_link_libraries(${PROJECT_NAME} UdpClient)
target_link_libraries(${PROJECT_NAME} Serial)
target_link_libraries(${PROJECT_NAME} jsoncpp)
# target_link_libraries(${PROJECT_NAME}
#                       ${Boost_LIBRARIES}
#                       )
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})