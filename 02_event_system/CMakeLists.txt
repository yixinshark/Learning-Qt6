cmake_minimum_required(VERSION 3.16)
project(Qt6EventSystemDemo VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt6组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Test)

# 启用Qt的MOC、UIC和RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 包含目录
include_directories(src)

# 收集主应用程序源文件（排除测试文件）
file(GLOB_RECURSE MAIN_SOURCES
    "src/*.cpp"
    "src/*.h"
)
list(FILTER MAIN_SOURCES EXCLUDE REGEX "src/tests/.*")

# 收集测试源文件
file(GLOB_RECURSE TEST_SOURCES
    "src/tests/*.cpp"
)

# 收集核心库源文件（用于测试）
file(GLOB_RECURSE CORE_SOURCES
    "src/core/*.cpp"
    "src/core/*.h"
)

# 收集示例源文件（用于测试）
file(GLOB_RECURSE EXAMPLE_SOURCES
    "src/examples/*.cpp"
    "src/examples/*.h"
)

# 创建主应用程序可执行文件
add_executable(Qt6EventSystemDemo ${MAIN_SOURCES})

# 链接Qt6库到主应用程序
target_link_libraries(Qt6EventSystemDemo
    Qt6::Core
    Qt6::Widgets
)

# 创建测试运行器可执行文件
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_runner.cpp")
    add_executable(TestRunner test_runner.cpp ${CORE_SOURCES} ${EXAMPLE_SOURCES})
    target_link_libraries(TestRunner
        Qt6::Core
        Qt6::Widgets
    )
endif()

# 创建测试可执行文件（如果有测试文件）
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS AND TEST_SOURCES)
    enable_testing()
    # 为每个测试文件创建单独的可执行文件
    foreach(TEST_FILE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        if(TEST_NAME MATCHES "^test_.*")
            set(TEST_TARGET_NAME "${TEST_NAME}_unit")
            add_executable(${TEST_TARGET_NAME} ${TEST_FILE} ${CORE_SOURCES} ${EXAMPLE_SOURCES})
            target_link_libraries(${TEST_TARGET_NAME}
                Qt6::Core
                Qt6::Widgets
                Qt6::Test
            )
            
            # 添加测试
            add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})
        endif()
    endforeach()
endif()

# 编译选项
if(MSVC)
    target_compile_options(Qt6EventSystemDemo PRIVATE /W4)
else()
    target_compile_options(Qt6EventSystemDemo PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 设置调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Qt6EventSystemDemo PRIVATE DEBUG_BUILD)
endif()