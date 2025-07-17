# Qt6 高级事件处理模式演示

本目录包含了Qt6事件系统的高级处理模式演示，展示了如何通过优化技术提升事件处理性能。

## 组件说明

### 1. EventCompressionDemo (事件压缩演示)
- **文件**: `event_compression_demo.h/cpp`
- **功能**: 演示如何压缩和合并高频事件
- **特性**:
  - 鼠标移动事件压缩
  - 重绘事件合并
  - 数据更新批处理
  - 可配置的压缩间隔
  - 实时性能统计

### 2. EventPoolingDemo (事件池化演示)
- **文件**: `event_pooling_demo.h/cpp`
- **功能**: 演示对象池技术优化事件处理
- **特性**:
  - 预分配事件对象池
  - 自动池扩展机制
  - 内存使用监控
  - 性能基准测试
  - 线程安全的池管理

### 3. AdvancedPatternsDemo (综合演示)
- **文件**: `advanced_patterns_demo.h/cpp`
- **功能**: 整合所有高级模式的主演示窗口
- **特性**:
  - 标签页式界面
  - 性能基准测试
  - 最佳实践指南
  - 结果导出功能

## 核心技术

### 事件压缩 (Event Compression)
```cpp
// 示例：鼠标事件压缩
void EventCompressionDemo::mouseMoveEvent(QMouseEvent *event)
{
    if (m_compressionEnabled) {
        // 将事件添加到队列
        m_mouseEventQueue.enqueue(mouseData);
        
        // 启动压缩定时器
        if (!m_mouseCompressionTimer->isActive()) {
            m_mouseCompressionTimer->start(m_compressionIntervalMs);
        }
    }
}
```

### 事件池化 (Event Pooling)
```cpp
// 示例：从池中获取事件对象
PooledEvent* event = m_eventPool->acquireEvent();
event->setData("key", value);

// 使用完毕后释放回池中
m_eventPool->releaseEvent(event);
```

## 性能优势

### 事件压缩
- 减少事件处理次数：高达90%的压缩率
- 降低CPU使用率：减少不必要的计算
- 提高UI响应性：避免事件积压

### 事件池化
- 减少内存分配：避免频繁的new/delete
- 提高处理速度：预分配对象重用
- 降低GC压力：减少垃圾回收开销

## 使用场景

### 适合事件压缩的场景
- 高频鼠标移动事件
- 连续的重绘请求
- 实时数据更新
- 滚动事件处理

### 适合事件池化的场景
- 大量临时事件对象
- 高频事件处理
- 内存敏感应用
- 游戏或实时应用

## 编译和运行

这些组件已集成到主项目中，通过以下方式运行：

```bash
cd 02_event_system
mkdir build && cd build
cmake ..
make
./Qt6EventSystemDemo
```

然后选择"高级事件处理模式"标签页即可体验各种演示。

## 注意事项

1. **事件压缩**：可能丢失中间状态，需要权衡延迟和性能
2. **事件池化**：需要正确管理对象生命周期，避免内存泄漏
3. **线程安全**：多线程环境下需要适当的同步机制
4. **性能测试**：建议在实际使用场景中进行基准测试

## 扩展建议

- 实现更多事件类型的压缩算法
- 添加自适应池大小调整
- 支持事件优先级处理
- 集成异步事件处理机制