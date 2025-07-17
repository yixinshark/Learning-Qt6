#ifndef EVENT_POOLING_DEMO_H
#define EVENT_POOLING_DEMO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QSpinBox>
#include <QProgressBar>
#include <QCheckBox>
#include <QTimer>
#include <QEvent>
#include <QQueue>
#include <QStack>
#include <QMutex>
#include <QElapsedTimer>
#include <QDateTime>
#include <QThread>
#include <memory>

// 自定义事件类型
class PooledEvent : public QEvent
{
public:
    static const QEvent::Type PooledEventType = static_cast<QEvent::Type>(QEvent::User + 1000);
    
    PooledEvent() : QEvent(PooledEventType), m_inUse(false) {}
    
    // 重置事件状态以便重用
    void reset() {
        m_data.clear();
        m_timestamp = QDateTime();
        m_priority = 0;
        m_inUse = false;
    }
    
    // 设置事件数据
    void setData(const QString &key, const QVariant &value) {
        m_data[key] = value;
    }
    
    QVariant getData(const QString &key) const {
        return m_data.value(key);
    }
    
    void setTimestamp(const QDateTime &timestamp) { m_timestamp = timestamp; }
    QDateTime timestamp() const { return m_timestamp; }
    
    void setPriority(int priority) { m_priority = priority; }
    int priority() const { return m_priority; }
    
    void setInUse(bool inUse) { m_inUse = inUse; }
    bool isInUse() const { return m_inUse; }

private:
    QMap<QString, QVariant> m_data;
    QDateTime m_timestamp;
    int m_priority;
    bool m_inUse;
};

// 事件池管理器
class EventPool
{
public:
    EventPool(int initialSize = 100);
    ~EventPool();
    
    // 获取事件对象
    PooledEvent* acquireEvent();
    
    // 释放事件对象回池中
    void releaseEvent(PooledEvent* event);
    
    // 统计信息
    int totalEvents() const { return m_totalEvents; }
    int availableEvents() const { return m_availableEvents.size(); }
    int usedEvents() const { return m_totalEvents - m_availableEvents.size(); }
    
    // 池管理
    void expandPool(int additionalSize);
    void shrinkPool();
    void clearPool();

private:
    QStack<PooledEvent*> m_availableEvents;
    QMutex m_mutex;
    int m_totalEvents;
    int m_initialSize;
    
    void createEvents(int count);
};

/**
 * EventPoolingDemo - 演示事件对象池化技术
 * 
 * 这个组件展示了如何通过对象池来优化事件处理性能：
 * 1. 事件对象重用 - 避免频繁的内存分配和释放
 * 2. 内存池管理 - 预分配事件对象池
 * 3. 性能监控 - 监控池的使用情况和性能指标
 * 4. 自动扩展 - 根据需要动态调整池大小
 */
class EventPoolingDemo : public QWidget
{
    Q_OBJECT

public:
    explicit EventPoolingDemo(QWidget *parent = nullptr);
    ~EventPoolingDemo();

protected:
    bool event(QEvent *event) override;

private slots:
    void onGenerateEventsClicked();
    void onClearPoolClicked();
    void onPoolSizeChanged(int size);
    void onAutoExpandToggled(bool enabled);
    void updateStatistics();
    void processPooledEvents();

private:
    // UI组件
    void setupUI();
    QVBoxLayout *m_mainLayout;
    QGroupBox *m_controlGroup;
    QGroupBox *m_statisticsGroup;
    QGroupBox *m_logGroup;
    
    // 控制面板
    QPushButton *m_generateEventsBtn;
    QPushButton *m_clearPoolBtn;
    QSpinBox *m_poolSizeSpinBox;
    QCheckBox *m_autoExpandCheckBox;
    QLabel *m_statusLabel;
    
    // 统计显示
    QLabel *m_totalEventsLabel;
    QLabel *m_availableEventsLabel;
    QLabel *m_usedEventsLabel;
    QLabel *m_poolEfficiencyLabel;
    QProgressBar *m_poolUsageBar;
    QLabel *m_memoryUsageLabel;
    QLabel *m_performanceLabel;
    
    // 日志区域
    QTextEdit *m_logTextEdit;
    
    // 事件池和处理
    std::unique_ptr<EventPool> m_eventPool;
    QTimer *m_statisticsTimer;
    QTimer *m_eventProcessingTimer;
    QQueue<PooledEvent*> m_pendingEvents;
    
    // 性能监控
    QElapsedTimer m_performanceTimer;
    int m_eventsProcessed;
    int m_totalEventsGenerated;
    qint64 m_totalProcessingTime;
    
    // 配置
    bool m_autoExpandEnabled;
    int m_maxPoolSize;
    
    // 辅助方法
    void logMessage(const QString &message);
    void generateTestEvents(int count);
    void updatePoolStatistics();
    void measurePerformance();
    QString formatMemorySize(qint64 bytes);
    
    // 事件处理
    void handlePooledEvent(PooledEvent *event);
    void simulateEventProcessing(PooledEvent *event);
};

#endif // EVENT_POOLING_DEMO_H