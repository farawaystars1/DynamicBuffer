# CMakeList.txt: DynamicBuffer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
include_directories("/home/xingxing/boost/BOOST/include")
link_directories("/home/xingxing/boost/BOOST/lib")
link_libraries("boost_json")
# 将源代码添加到此项目的可执行文件。
add_executable (DynamicBuffer "DynamicBuffer.cpp" "DynamicBuffer.h" "Buffer/Endian.h" "Buffer/Buffer.h" "Buffer/Buffer.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DynamicBuffer PROPERTY CXX_STANDARD 20)
endif()
set(EXECUTABLE_OUTPUT_PATH "/home/xingxing/bin")

# TODO: 如有需要，请添加测试并安装目标。
