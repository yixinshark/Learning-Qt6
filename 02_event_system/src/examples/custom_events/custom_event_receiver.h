#ifndef CUSTOM_EVENT_RECEIVER_H
#define CUSTOM_EVENT_RECEIVER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QTimer>
#include <QDateTime>
#include <QCheckBox>
#include <QSpinBox>
#include "../../core/custom_events.h"

/**
 * @brief 自定义事件接收器组件
 * 
 * 演示自定义事件的接收和处理，提供事件日志显示和统计功能
 */
class CustomEventReceiver : public QWidget
{
    Q_OBJECT

public:
    explicit CustomEventReceiver(QWidget *parent = nullptr);
    ~CustomEventReceiver() = default;

    // 事件统计信息
    struct EventStatistics {
        int totalEvents = 0;
        int dataEvents = 0;
        int commandEvents = 0;
        int processedEvents = 0;
        int ignoredEvents = 0;
        QDateTime firstEventTime;
        QDateTime lastEventTime;
    };

    // 获取统计信息
    const EventStatistics& statistics() const;
    void resetStatistics();

public slots:
    // 控制事件处理
    void setEventProcessingEnabled(bool enabled);
    void clearEventLog();
    void saveEventLog();
    
    // 事件过滤控制
    void setDataEventFilter(bool enabled);
    void setCommandEventFilter(bool enabled);
    void setMaxLogEntries(int maxEntries);

signals:
    // 事件接收通知
    void eventReceived(const QString& eventType, const QString& description);
    void dataEventReceived(const QVariant& data);
    void commandEventReceived(const QString& command, const QVariantMap& parameters);
    
    // 统计信息更新
    void statisticsUpdated(const EventStatistics& stats);

protected:
    // 重写事件处理函数
    bool event(QEvent* event) override;
    
    // 具体事件处理方法
    bool handleDataEvent(DataEvent* event);
    bool handleCommandEvent(CommandEvent* event);

private slots:
    void onUpdateStatistics();

private:
    void setupUI();
    void setupControlPanel();
    void setupEventLog();
    void setupStatisticsPanel();
    
    // 日志管理
    void addLogEntry(const QString& eventType, const QString& timestamp, 
                    const QString& description, bool processed);
    void updateStatisticsDisplay();
    
    // 事件处理辅助方法
    QString formatDataEventInfo(DataEvent* event);
    QString formatCommandEventInfo(CommandEvent* event);
    QString formatEventData(const QVariant& data);
    
    // UI组件
    QVBoxLayout* m_mainLayout;
    
    // 控制面板
    QGroupBox* m_controlGroup;
    QCheckBox* m_enableProcessingCheck;
    QCheckBox* m_dataEventFilterCheck;
    QCheckBox* m_commandEventFilterCheck;
    QSpinBox* m_maxLogEntriesSpin;
    QPushButton* m_clearLogBtn;
    QPushButton* m_saveLogBtn;
    QPushButton* m_resetStatsBtn;
    
    // 事件日志
    QGroupBox* m_logGroup;
    QTableWidget* m_eventLogTable;
    
    // 统计面板
    QGroupBox* m_statsGroup;
    QLabel* m_totalEventsLabel;
    QLabel* m_dataEventsLabel;
    QLabel* m_commandEventsLabel;
    QLabel* m_processedEventsLabel;
    QLabel* m_ignoredEventsLabel;
    QLabel* m_firstEventLabel;
    QLabel* m_lastEventLabel;
    QLabel* m_eventRateLabel;
    
    // 详细信息显示
    QGroupBox* m_detailGroup;
    QTextEdit* m_lastEventDetail;
    
    // 状态和配置
    bool m_processingEnabled;
    bool m_dataEventFilterEnabled;
    bool m_commandEventFilterEnabled;
    int m_maxLogEntries;
    
    // 统计数据
    EventStatistics m_statistics;
    QTimer* m_statisticsTimer;
    
    // 事件处理历史
    QList<QPair<QDateTime, QString>> m_eventHistory;
};

#endif // CUSTOM_EVENT_RECEIVER_H