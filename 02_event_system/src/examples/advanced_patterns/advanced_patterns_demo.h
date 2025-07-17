#ifndef ADVANCED_PATTERNS_DEMO_H
#define ADVANCED_PATTERNS_DEMO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QTextEdit>
#include <QSplitter>

#include "event_compression_demo.h"
#include "event_pooling_demo.h"

/**
 * AdvancedPatternsDemo - 高级事件处理模式演示主窗口
 * 
 * 这个组件整合了多种高级事件处理模式的演示：
 * 1. 事件压缩演示 - 展示如何压缩和合并事件
 * 2. 事件池化演示 - 展示如何使用对象池优化性能
 * 3. 综合性能对比 - 对比不同模式的性能表现
 * 4. 最佳实践指南 - 提供使用建议和注意事项
 */
class AdvancedPatternsDemo : public QWidget
{
    Q_OBJECT

public:
    explicit AdvancedPatternsDemo(QWidget *parent = nullptr);
    ~AdvancedPatternsDemo();

private slots:
    void onRunBenchmarkClicked();
    void onClearAllLogsClicked();
    void onExportResultsClicked();
    void onTabChanged(int index);

private:
    // UI组件
    void setupUI();
    void setupBenchmarkPanel();
    void setupGuidePanel();
    
    QVBoxLayout *m_mainLayout;
    QTabWidget *m_tabWidget;
    QSplitter *m_splitter;
    
    // 演示组件
    EventCompressionDemo *m_compressionDemo;
    EventPoolingDemo *m_poolingDemo;
    
    // 基准测试面板
    QWidget *m_benchmarkWidget;
    QGroupBox *m_benchmarkGroup;
    QPushButton *m_runBenchmarkBtn;
    QPushButton *m_clearLogsBtn;
    QPushButton *m_exportResultsBtn;
    QTextEdit *m_benchmarkResults;
    QLabel *m_benchmarkStatus;
    
    // 指南面板
    QWidget *m_guideWidget;
    QTextEdit *m_guideText;
    
    // 辅助方法
    void runPerformanceBenchmark();
    void generateBenchmarkReport();
    void loadBestPracticesGuide();
    void logBenchmarkResult(const QString &message);
    
    // 基准测试相关
    struct BenchmarkResult {
        QString testName;
        qint64 executionTime;
        int eventsProcessed;
        double eventsPerSecond;
        qint64 memoryUsed;
    };
    
    QList<BenchmarkResult> m_benchmarkResultsList;
};

#endif // ADVANCED_PATTERNS_DEMO_H