cmake_minimum_required(VERSION 3.16)

# Qt6学习项目 - 顶层CMake配置
project(Qt6LearningProject VERSION 1.0.0 LANGUAGES CXX)

# 设置全局C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置全局构建类型（如果未指定）
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 查找Qt6 - 在顶层查找，子项目可以继承
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 设置全局输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 打印项目信息
message(STATUS "=== Qt6 学习项目配置 ===")
message(STATUS "项目名称: ${PROJECT_NAME}")
message(STATUS "项目版本: ${PROJECT_VERSION}")
message(STATUS "Qt6 版本: ${Qt6_VERSION}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ 标准: ${CMAKE_CXX_STANDARD}")
message(STATUS "输出目录: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# 添加子项目
message(STATUS "=== 添加子模块 ===")

# 1. 元对象系统模块
add_subdirectory(01_metaobject_system)
message(STATUS "✓ 01_metaobject_system - Qt元对象系统学习")

# 2. 事件系统模块
add_subdirectory(02_event_system)
message(STATUS "✓ 02_event_system - Qt事件系统学习")

# 未来可以添加更多模块，例如：

message(STATUS "=== 配置完成 ===")