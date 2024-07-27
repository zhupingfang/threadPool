cmake_minimum_required(VERSION 3.28)
project(thread)

set(CMAKE_CXX_STANDARD 20)

# 将源文件 都用一个变量包含
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

# 生成静态库，并输出到lib文件中
add_library(threadPool STATIC ${SRC})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

# 添加头文件
include_directories(${CMAKE_SOURCE_DIR}/include)
# 链接库目录
link_directories(${CMAKE_SOURCE_DIR}/lib)
# 链接库名
link_libraries(threadPool)

add_executable(thread main.cpp)
