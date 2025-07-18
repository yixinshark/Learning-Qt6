#ifndef CUSTOM_EVENT_SENDER_H
#define CUSTOM_EVENT_SENDER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QTimer>
#include "../../core/custom_events.h"

/**
 * @brief 自定义事件发送器组件
 * 
 * 演示自定义事件的创建和发送，支持DataEvent和CommandEvent的发送
 */
class CustomEventSender : public QWidget
{
    Q_OBJECT

public:
    explicit CustomEventSender(QWidget *parent = nullptr);
    ~CustomEventSender() = default;

    // 设置事件接收目标
    void setEventTarget(QObject* target);
    QObject* eventTarget() const;

public slots:
    // 发送数据事件
    void sendDataEvent();
    void sendStringData();
    void sendNumberData();
    void sendMapData();
    void sendListData();
    
    // 发送命令事件
    void sendCommandEvent();
    void sendSimpleCommand();
    void sendParameterizedCommand();
    
    // 批量发送事件
    void sendBatchEvents();
    
    // 定时发送事件
    void startPeriodicSending();
    void stopPeriodicSending();

signals:
    // 事件发送通知
    void eventSent(const QString& eventType, const QString& description);
    void batchEventsSent(int count);

private slots:
    void onPeriodicTimer();

private:
    void setupUI();
    void setupDataEventControls();
    void setupCommandEventControls();
    void setupBatchControls();
    
    // 发送事件的便利方法
    void postEvent(BaseCustomEvent* event);
    void sendEvent(BaseCustomEvent* event);
    
    // UI组件
    QVBoxLayout* m_mainLayout;
    
    // 数据事件控件
    QGroupBox* m_dataEventGroup;
    QLineEdit* m_stringDataEdit;
    QSpinBox* m_numberDataSpin;
    QTextEdit* m_mapDataEdit;
    QTextEdit* m_listDataEdit;
    QPushButton* m_sendStringBtn;
    QPushButton* m_sendNumberBtn;
    QPushButton* m_sendMapBtn;
    QPushButton* m_sendListBtn;
    
    // 命令事件控件
    QGroupBox* m_commandEventGroup;
    QLineEdit* m_commandEdit;
    QTextEdit* m_parametersEdit;
    QPushButton* m_sendCommandBtn;
    QPushButton* m_sendSimpleCommandBtn;
    
    // 批量发送控件
    QGroupBox* m_batchGroup;
    QSpinBox* m_batchCountSpin;
    QComboBox* m_batchTypeCombo;
    QPushButton* m_sendBatchBtn;
    
    // 定时发送控件
    QGroupBox* m_periodicGroup;
    QSpinBox* m_intervalSpin;
    QPushButton* m_startPeriodicBtn;
    QPushButton* m_stopPeriodicBtn;
    QTimer* m_periodicTimer;
    
    // 状态显示
    QLabel* m_statusLabel;
    QLabel* m_targetLabel;
    
    // 统计信息
    int m_eventsSent;
    
    // 事件目标
    QObject* m_eventTarget;
};

#endif // CUSTOM_EVENT_SENDER_H