# Qt6 元对象系统深度学习

> 📖 **详细技术文档**: 请查看 [`Qt6_MetaObject_System_Guide.md`](Qt6_MetaObject_System_Guide.md) 获取完整的理论分析和技术深度解析

这是一个完整的Qt6元对象系统学习和演示项目，通过实际代码展示Qt元对象系统的核心特性和工作原理。

## 🎯 学习目标

通过这个项目，你将深入理解：
- Qt元对象系统的设计原理和实现机制
- Q_OBJECT、Q_PROPERTY、Q_INVOKABLE等关键宏的工作原理
- 信号槽机制的内部实现和Connect函数原理
- MOC编译器生成代码的详细分析
- 属性系统、反射和内省的实际应用
- 性能基准测试和优化建议

## 📁 项目结构

```
01_metaobject_system/
├── CMakeLists.txt                    # CMake构建配置
├── README.md                         # 项目说明文档
├── Qt6_MetaObject_System_Guide.md    # 📖 详细技术文档 (重点阅读)
├── src/                              # 源代码目录
│   ├── main.cpp                      # 程序入口
│   ├── person.h/.cpp                 # Person类 - 基础元对象演示
│   ├── company.h/.cpp                # Company类 - 复杂元对象演示
│   ├── demo_runner.h/.cpp            # 演示控制器
│   └── generated/                    # MOC生成的文件
│       ├── moc_person.cpp            # Person类的MOC生成代码
│       ├── moc_company.cpp           # Company类的MOC生成代码
│       ├── moc_demo_runner.cpp       # DemoRunner类的MOC生成代码
│       └── moc_person_analyzed.cpp   # 🔍 带详细中文注释的MOC代码分析
└── build/                            # 构建目录
```

## 🚀 快速开始

### 环境要求
- Qt6.0或更高版本
- CMake 3.16或更高版本  
- C++17支持的编译器

### 编译步骤
```bash
# 1. 创建构建目录
mkdir build && cd build

# 2. 配置CMake
cmake ..

# 3. 编译项目
cmake --build .

# 4. 运行演示程序
./bin/Qt6MetaObjectDemo
```

## 📊 演示内容

程序运行时会依次演示7个场景：

1. **基本信号槽演示** - 属性变化信号、方法调用响应
2. **属性系统演示** - 动态属性访问、计算属性使用
3. **反射和内省演示** - 元对象信息分析、类继承关系
4. **动态方法调用演示** - 字符串调用、参数传递、返回值处理
5. **复杂对象交互演示** - 对象组合、信号槽网络、生命周期管理
6. **元对象深度分析** - 继承链分析、枚举信息、方法签名解析
7. **性能测试** - 直接调用 vs 属性系统 vs 反射调用的性能对比

## 🔍 学习资源

### 核心文档
- 📖 **[Qt6_MetaObject_System_Guide.md](Qt6_MetaObject_System_Guide.md)** - 完整的理论分析和技术深度解析
- 🔍 **[src/generated/moc_person_analyzed.cpp](src/generated/moc_person_analyzed.cpp)** - 带详细中文注释的MOC代码分析

### 代码示例
- **Person类** - 基础元对象特性 (信号槽、属性、Q_INVOKABLE)
- **Company类** - 复杂元对象应用 (枚举、对象组合、动态属性)
- **DemoRunner类** - 演示控制和元对象分析

### 性能基准
- 直接调用：最快 (基准)
- 属性系统调用：约慢4-5倍
- 反射调用：约慢28倍以上

## 🎓 学习建议

1. **先运行程序** - 观察实际效果，建立感性认识
2. **阅读技术文档** - 深入理解理论基础和实现原理
3. **分析源代码** - 理解各种元对象特性的具体实现
4. **研究MOC代码** - 了解Qt元对象系统的内部机制
5. **实践扩展** - 尝试添加新功能，加深理解

---

> 💡 **重要提示**: 详细的技术解析、宏定义原理、Connect函数实现等深度内容请查看 [`Qt6_MetaObject_System_Guide.md`](Qt6_MetaObject_System_Guide.md) 文档。