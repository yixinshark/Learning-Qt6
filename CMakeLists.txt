cmake_minimum_required(VERSION 3.16)

# 项目配置
project(Qt6MetaObjectDemo VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt6组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 启用Qt的自动处理功能
set(CMAKE_AUTOMOC ON)  # 自动运行MOC
set(CMAKE_AUTORCC ON)  # 自动处理资源文件
set(CMAKE_AUTOUIC ON)  # 自动处理UI文件

# 设置源文件
set(SOURCES
    src/main.cpp
    src/person.cpp
    src/company.cpp
    src/demo_runner.cpp
)

set(HEADERS
    src/person.h
    src/company.h
    src/demo_runner.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接Qt库
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets)

# 设置包含目录
target_include_directories(${PROJECT_NAME} PRIVATE src)

# 自定义函数：拷贝MOC文件到src目录
function(copy_moc_files)
    # 获取构建目录中的MOC文件
    file(GLOB MOC_FILES "${CMAKE_CURRENT_BINARY_DIR}/*_autogen/mocs_compilation.cpp")
    file(GLOB MOC_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/*_autogen/include/moc_*.cpp")
    
    # 创建自定义目标来拷贝MOC文件
    add_custom_target(copy_moc_files ALL
        COMMAND ${CMAKE_COMMAND} -E echo "正在拷贝MOC生成的文件..."
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/generated
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/include
                ${CMAKE_CURRENT_SOURCE_DIR}/src/generated
        DEPENDS ${PROJECT_NAME}
        COMMENT "拷贝MOC生成的文件到src/generated目录"
    )
endfunction()

# 调用拷贝函数
copy_moc_files()

# 添加自定义命令来显示MOC文件内容
add_custom_target(show_moc_content
    COMMAND ${CMAKE_COMMAND} -E echo "=== MOC生成的文件内容 ==="
    COMMAND ${CMAKE_COMMAND} -E echo "查看 src/generated/ 目录中的文件"
    DEPENDS copy_moc_files
    COMMENT "显示MOC生成的文件"
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)

# 如果是Debug模式，添加调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE)
endif()

# 打印配置信息
message(STATUS "Qt6 版本: ${Qt6_VERSION}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ 标准: ${CMAKE_CXX_STANDARD}")