#ifndef CUSTOM_EVENT_DEMO_H
#define CUSTOM_EVENT_DEMO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include "custom_event_sender.h"
#include "custom_event_receiver.h"

/**
 * @brief 自定义事件系统演示主界面
 * 
 * 集成CustomEventSender和CustomEventReceiver，演示完整的自定义事件工作流程
 */
class CustomEventDemo : public QWidget
{
    Q_OBJECT

public:
    explicit CustomEventDemo(QWidget *parent = nullptr);
    ~CustomEventDemo() = default;

public slots:
    // 演示控制
    void startDemo();
    void stopDemo();
    void resetDemo();
    
    // 预设演示场景
    void runBasicDataDemo();
    void runCommandDemo();
    void runSerializationDemo();
    void runPerformanceDemo();

private slots:
    // 事件处理响应
    void onEventSent(const QString& eventType, const QString& description);
    void onEventReceived(const QString& eventType, const QString& description);
    void onDataEventReceived(const QVariant& data);
    void onCommandEventReceived(const QString& command, const QVariantMap& parameters);
    void onStatisticsUpdated(const CustomEventReceiver::EventStatistics& stats);
    
    // 自动演示
    void onDemoTimer();

private:
    void setupUI();
    void setupDemoControls();
    void setupEventFlow();
    void updateEventFlowDisplay();
    
    // 演示场景实现
    void demonstrateDataSerialization();
    void demonstrateEventBatching();
    void demonstrateEventFiltering();
    
    // UI组件
    QVBoxLayout* m_mainLayout;
    QSplitter* m_mainSplitter;
    
    // 演示控制面板
    QGroupBox* m_demoControlGroup;
    QPushButton* m_startDemoBtn;
    QPushButton* m_stopDemoBtn;
    QPushButton* m_resetDemoBtn;
    QPushButton* m_basicDataDemoBtn;
    QPushButton* m_commandDemoBtn;
    QPushButton* m_serializationDemoBtn;
    QPushButton* m_performanceDemoBtn;
    
    // 事件流显示
    QGroupBox* m_eventFlowGroup;
    QTextEdit* m_eventFlowDisplay;
    QLabel* m_flowStatsLabel;
    
    // 核心组件
    CustomEventSender* m_sender;
    CustomEventReceiver* m_receiver;
    
    // 演示状态
    QTimer* m_demoTimer;
    int m_demoStep;
    bool m_demoRunning;
    
    // 统计信息
    int m_totalEventsSent;
    int m_totalEventsReceived;
    QDateTime m_demoStartTime;
};

#endif // CUSTOM_EVENT_DEMO_H