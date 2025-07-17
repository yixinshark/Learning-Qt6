#include "advanced_patterns_demo.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QThread>
#include <QDebug>

AdvancedPatternsDemo::AdvancedPatternsDemo(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_tabWidget(nullptr)
    , m_splitter(nullptr)
    , m_compressionDemo(nullptr)
    , m_poolingDemo(nullptr)
    , m_benchmarkWidget(nullptr)
    , m_benchmarkGroup(nullptr)
    , m_runBenchmarkBtn(nullptr)
    , m_clearLogsBtn(nullptr)
    , m_exportResultsBtn(nullptr)
    , m_benchmarkResults(nullptr)
    , m_benchmarkStatus(nullptr)
    , m_guideWidget(nullptr)
    , m_guideText(nullptr)
{
    setupUI();
    loadBestPracticesGuide();
}

AdvancedPatternsDemo::~AdvancedPatternsDemo()
{
    // Qt会自动清理子对象
}

void AdvancedPatternsDemo::setupUI()
{
    setWindowTitle("Qt6 高级事件处理模式演示");
    setMinimumSize(1200, 800);
    
    m_mainLayout = new QVBoxLayout(this);
    
    // 创建标签页组件
    m_tabWidget = new QTabWidget(this);
    
    // 事件压缩演示标签页
    m_compressionDemo = new EventCompressionDemo(this);
    m_tabWidget->addTab(m_compressionDemo, "事件压缩演示");
    
    // 事件池化演示标签页
    m_poolingDemo = new EventPoolingDemo(this);
    m_tabWidget->addTab(m_poolingDemo, "事件池化演示");
    
    // 基准测试标签页
    setupBenchmarkPanel();
    m_tabWidget->addTab(m_benchmarkWidget, "性能基准测试");
    
    // 最佳实践指南标签页
    setupGuidePanel();
    m_tabWidget->addTab(m_guideWidget, "最佳实践指南");
    
    m_mainLayout->addWidget(m_tabWidget);
    
    // 连接信号
    connect(m_tabWidget, &QTabWidget::currentChanged,
            this, &AdvancedPatternsDemo::onTabChanged);
}

void AdvancedPatternsDemo::setupBenchmarkPanel()
{
    m_benchmarkWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_benchmarkWidget);
    
    // 控制面板
    m_benchmarkGroup = new QGroupBox("性能基准测试", this);
    QHBoxLayout *controlLayout = new QHBoxLayout(m_benchmarkGroup);
    
    m_runBenchmarkBtn = new QPushButton("运行基准测试", this);
    m_clearLogsBtn = new QPushButton("清空结果", this);
    m_exportResultsBtn = new QPushButton("导出结果", this);
    m_benchmarkStatus = new QLabel("状态: 就绪", this);
    
    controlLayout->addWidget(m_runBenchmarkBtn);
    controlLayout->addWidget(m_clearLogsBtn);
    controlLayout->addWidget(m_exportResultsBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(m_benchmarkStatus);
    
    // 结果显示区域
    m_benchmarkResults = new QTextEdit(this);
    m_benchmarkResults->setReadOnly(true);
    m_benchmarkResults->setFont(QFont("Consolas", 10));
    
    layout->addWidget(m_benchmarkGroup);
    layout->addWidget(m_benchmarkResults);
    
    // 连接信号
    connect(m_runBenchmarkBtn, &QPushButton::clicked,
            this, &AdvancedPatternsDemo::onRunBenchmarkClicked);
    connect(m_clearLogsBtn, &QPushButton::clicked,
            this, &AdvancedPatternsDemo::onClearAllLogsClicked);
    connect(m_exportResultsBtn, &QPushButton::clicked,
            this, &AdvancedPatternsDemo::onExportResultsClicked);
}

void AdvancedPatternsDemo::setupGuidePanel()
{
    m_guideWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_guideWidget);
    
    QLabel *titleLabel = new QLabel("Qt6 高级事件处理模式 - 最佳实践指南", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    
    m_guideText = new QTextEdit(this);
    m_guideText->setReadOnly(true);
    
    layout->addWidget(titleLabel);
    layout->addWidget(m_guideText);
}

void AdvancedPatternsDemo::onRunBenchmarkClicked()
{
    m_benchmarkStatus->setText("状态: 运行基准测试中...");
    m_runBenchmarkBtn->setEnabled(false);
    
    // 使用QTimer延迟执行，避免阻塞UI
    QTimer::singleShot(100, this, &AdvancedPatternsDemo::runPerformanceBenchmark);
}

void AdvancedPatternsDemo::onClearAllLogsClicked()
{
    m_benchmarkResults->clear();
    m_benchmarkResultsList.clear();
    logBenchmarkResult("基准测试结果已清空");
}

void AdvancedPatternsDemo::onExportResultsClicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "导出基准测试结果",
        "benchmark_results.txt",
        "文本文件 (*.txt);;所有文件 (*)"
    );
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << m_benchmarkResults->toPlainText();
            QMessageBox::information(this, "导出成功", "基准测试结果已导出到: " + fileName);
        } else {
            QMessageBox::warning(this, "导出失败", "无法写入文件: " + fileName);
        }
    }
}

void AdvancedPatternsDemo::onTabChanged(int index)
{
    // 可以在这里添加标签页切换时的逻辑
    Q_UNUSED(index)
}

void AdvancedPatternsDemo::runPerformanceBenchmark()
{
    logBenchmarkResult("=== Qt6 高级事件处理模式性能基准测试 ===");
    logBenchmarkResult(QString("测试时间: %1").arg(QDateTime::currentDateTime().toString()));
    logBenchmarkResult("");
    
    // 测试1: 事件压缩性能
    logBenchmarkResult("测试1: 事件压缩性能测试");
    logBenchmarkResult("----------------------------------------");
    
    QElapsedTimer timer;
    timer.start();
    
    // 模拟大量鼠标移动事件
    const int eventCount = 10000;
    int compressedEvents = 0;
    
    // 模拟事件压缩算法
    QQueue<QPoint> eventQueue;
    for (int i = 0; i < eventCount; ++i) {
        QPoint pos(QRandomGenerator::global()->bounded(1920),
                   QRandomGenerator::global()->bounded(1080));
        eventQueue.enqueue(pos);
        
        // 每50个事件压缩一次
        if (eventQueue.size() >= 50) {
            // 压缩：只保留最后一个位置
            eventQueue.clear();
            eventQueue.enqueue(pos);
            compressedEvents++;
        }
    }
    
    qint64 compressionTime = timer.elapsed();
    double compressionRatio = (1.0 - (double)compressedEvents / eventCount) * 100.0;
    
    BenchmarkResult compressionResult;
    compressionResult.testName = "事件压缩";
    compressionResult.executionTime = compressionTime;
    compressionResult.eventsProcessed = eventCount;
    compressionResult.eventsPerSecond = (double)eventCount * 1000.0 / compressionTime;
    compressionResult.memoryUsed = eventQueue.size() * sizeof(QPoint);
    
    m_benchmarkResultsList.append(compressionResult);
    
    logBenchmarkResult(QString("原始事件数: %1").arg(eventCount));
    logBenchmarkResult(QString("压缩后事件数: %1").arg(compressedEvents));
    logBenchmarkResult(QString("压缩比率: %1%").arg(QString::number(compressionRatio, 'f', 1)));
    logBenchmarkResult(QString("执行时间: %1 ms").arg(compressionTime));
    logBenchmarkResult(QString("处理速度: %1 事件/秒").arg(QString::number(compressionResult.eventsPerSecond, 'f', 0)));
    logBenchmarkResult("");
    
    // 测试2: 事件池化性能
    logBenchmarkResult("测试2: 事件池化性能测试");
    logBenchmarkResult("----------------------------------------");
    
    timer.restart();
    
    // 创建事件池
    EventPool eventPool(1000);
    QList<PooledEvent*> acquiredEvents;
    
    // 获取事件对象
    for (int i = 0; i < eventCount; ++i) {
        PooledEvent* event = eventPool.acquireEvent();
        event->setData("id", i);
        event->setData("value", QRandomGenerator::global()->bounded(1000));
        acquiredEvents.append(event);
    }
    
    qint64 acquisitionTime = timer.elapsed();
    timer.restart();
    
    // 释放事件对象
    for (PooledEvent* event : acquiredEvents) {
        eventPool.releaseEvent(event);
    }
    
    qint64 releaseTime = timer.elapsed();
    qint64 totalPoolingTime = acquisitionTime + releaseTime;
    
    BenchmarkResult poolingResult;
    poolingResult.testName = "事件池化";
    poolingResult.executionTime = totalPoolingTime;
    poolingResult.eventsProcessed = eventCount;
    poolingResult.eventsPerSecond = (double)eventCount * 1000.0 / totalPoolingTime;
    poolingResult.memoryUsed = eventPool.totalEvents() * sizeof(PooledEvent);
    
    m_benchmarkResultsList.append(poolingResult);
    
    logBenchmarkResult(QString("池大小: %1").arg(eventPool.totalEvents()));
    logBenchmarkResult(QString("获取时间: %1 ms").arg(acquisitionTime));
    logBenchmarkResult(QString("释放时间: %1 ms").arg(releaseTime));
    logBenchmarkResult(QString("总时间: %1 ms").arg(totalPoolingTime));
    logBenchmarkResult(QString("处理速度: %1 事件/秒").arg(QString::number(poolingResult.eventsPerSecond, 'f', 0)));
    logBenchmarkResult("");
    
    // 测试3: 传统方式对比
    logBenchmarkResult("测试3: 传统事件处理对比");
    logBenchmarkResult("----------------------------------------");
    
    timer.restart();
    
    // 模拟传统的事件处理方式
    QList<QEvent*> traditionalEvents;
    for (int i = 0; i < eventCount; ++i) {
        QEvent* event = new QEvent(QEvent::User);
        traditionalEvents.append(event);
    }
    
    qint64 traditionalCreationTime = timer.elapsed();
    timer.restart();
    
    // 清理传统事件
    qDeleteAll(traditionalEvents);
    traditionalEvents.clear();
    
    qint64 traditionalCleanupTime = timer.elapsed();
    qint64 totalTraditionalTime = traditionalCreationTime + traditionalCleanupTime;
    
    BenchmarkResult traditionalResult;
    traditionalResult.testName = "传统方式";
    traditionalResult.executionTime = totalTraditionalTime;
    traditionalResult.eventsProcessed = eventCount;
    traditionalResult.eventsPerSecond = (double)eventCount * 1000.0 / totalTraditionalTime;
    traditionalResult.memoryUsed = eventCount * sizeof(QEvent);
    
    m_benchmarkResultsList.append(traditionalResult);
    
    logBenchmarkResult(QString("创建时间: %1 ms").arg(traditionalCreationTime));
    logBenchmarkResult(QString("清理时间: %1 ms").arg(traditionalCleanupTime));
    logBenchmarkResult(QString("总时间: %1 ms").arg(totalTraditionalTime));
    logBenchmarkResult(QString("处理速度: %1 事件/秒").arg(QString::number(traditionalResult.eventsPerSecond, 'f', 0)));
    logBenchmarkResult("");
    
    // 生成综合报告
    generateBenchmarkReport();
    
    m_benchmarkStatus->setText("状态: 基准测试完成");
    m_runBenchmarkBtn->setEnabled(true);
}

void AdvancedPatternsDemo::generateBenchmarkReport()
{
    logBenchmarkResult("=== 性能对比总结 ===");
    logBenchmarkResult("");
    
    if (m_benchmarkResultsList.size() >= 3) {
        const auto& compression = m_benchmarkResultsList[0];
        const auto& pooling = m_benchmarkResultsList[1];
        const auto& traditional = m_benchmarkResultsList[2];
        
        logBenchmarkResult("处理速度对比 (事件/秒):");
        logBenchmarkResult(QString("  事件压缩: %1").arg(QString::number(compression.eventsPerSecond, 'f', 0)));
        logBenchmarkResult(QString("  事件池化: %1").arg(QString::number(pooling.eventsPerSecond, 'f', 0)));
        logBenchmarkResult(QString("  传统方式: %1").arg(QString::number(traditional.eventsPerSecond, 'f', 0)));
        logBenchmarkResult("");
        
        logBenchmarkResult("内存使用对比 (字节):");
        logBenchmarkResult(QString("  事件压缩: %1").arg(compression.memoryUsed));
        logBenchmarkResult(QString("  事件池化: %1").arg(pooling.memoryUsed));
        logBenchmarkResult(QString("  传统方式: %1").arg(traditional.memoryUsed));
        logBenchmarkResult("");
        
        // 计算性能提升
        double poolingImprovement = (pooling.eventsPerSecond / traditional.eventsPerSecond - 1.0) * 100.0;
        double compressionImprovement = (compression.eventsPerSecond / traditional.eventsPerSecond - 1.0) * 100.0;
        
        logBenchmarkResult("性能提升:");
        logBenchmarkResult(QString("  事件池化相比传统方式: %1%").arg(QString::number(poolingImprovement, 'f', 1)));
        logBenchmarkResult(QString("  事件压缩相比传统方式: %1%").arg(QString::number(compressionImprovement, 'f', 1)));
    }
    
    logBenchmarkResult("");
    logBenchmarkResult("=== 基准测试完成 ===");
}

void AdvancedPatternsDemo::loadBestPracticesGuide()
{
    QString guideContent = R"(
# Qt6 高级事件处理模式最佳实践指南

## 1. 事件压缩 (Event Compression)

### 适用场景
- 高频率的鼠标移动事件
- 连续的重绘请求
- 频繁的数据更新通知
- 实时数据流处理

### 实现要点
- 使用定时器批处理事件
- 保留最新状态，丢弃中间状态
- 合并相似的事件类型
- 设置合适的压缩间隔

### 性能优势
- 减少事件处理次数
- 降低CPU使用率
- 提高UI响应性
- 减少不必要的重绘

### 注意事项
- 可能丢失中间状态信息
- 需要权衡延迟和性能
- 不适用于需要精确序列的场景

## 2. 事件池化 (Event Pooling)

### 适用场景
- 大量临时事件对象创建
- 高频率的事件处理
- 内存敏感的应用
- 需要减少GC压力的场景

### 实现要点
- 预分配事件对象池
- 实现获取和释放机制
- 支持池的动态扩展
- 线程安全的池管理

### 性能优势
- 减少内存分配开销
- 降低垃圾回收压力
- 提高事件处理速度
- 更好的内存局部性

### 注意事项
- 需要正确管理对象生命周期
- 避免对象泄漏
- 合理设置池大小
- 注意线程安全问题

## 3. 事件优先级处理

### 实现策略
- 使用优先级队列
- 分类处理不同类型事件
- 实现事件调度算法
- 支持事件抢占机制

### 应用场景
- 实时系统
- 游戏引擎
- 多媒体应用
- 交互式应用

## 4. 异步事件处理

### 实现方式
- 使用工作线程处理事件
- 实现事件队列机制
- 支持事件回调
- 处理线程间通信

### 优势
- 避免阻塞主线程
- 提高系统响应性
- 支持并发处理
- 更好的用户体验

## 5. 性能监控和调优

### 监控指标
- 事件处理延迟
- 事件队列长度
- 内存使用情况
- CPU使用率

### 调优建议
- 根据实际需求选择合适的模式
- 定期进行性能测试
- 监控系统资源使用
- 优化热点代码路径

## 6. 最佳实践总结

1. **选择合适的模式**: 根据应用场景选择最适合的事件处理模式
2. **性能测试**: 定期进行基准测试，验证优化效果
3. **资源管理**: 正确管理事件对象的生命周期
4. **错误处理**: 实现健壮的错误处理机制
5. **文档记录**: 详细记录设计决策和实现细节
6. **代码审查**: 定期审查事件处理相关代码
7. **持续优化**: 根据实际使用情况持续优化性能

## 7. 常见问题和解决方案

### Q: 事件压缩导致状态丢失怎么办？
A: 可以保存关键状态信息，或者使用选择性压缩策略。

### Q: 事件池内存泄漏如何避免？
A: 实现严格的获取/释放配对，使用RAII模式管理对象生命周期。

### Q: 如何选择合适的压缩间隔？
A: 根据应用的响应性要求和事件频率进行调整，通常在10-100ms之间。

### Q: 多线程环境下如何保证事件处理的线程安全？
A: 使用互斥锁保护共享资源，或者使用无锁数据结构。
)";

    m_guideText->setPlainText(guideContent);
}

void AdvancedPatternsDemo::logBenchmarkResult(const QString &message)
{
    m_benchmarkResults->append(message);
    
    // 自动滚动到底部
    QTextCursor cursor = m_benchmarkResults->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_benchmarkResults->setTextCursor(cursor);
    
    // 强制更新UI
    QApplication::processEvents();
}