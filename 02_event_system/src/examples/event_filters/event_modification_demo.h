#ifndef EVENT_MODIFICATION_DEMO_H
#define EVENT_MODIFICATION_DEMO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QTimer>
#include "global_event_filter.h"
#include "selective_event_filter.h"

/**
 * @brief 事件修改演示组件
 * 
 * 这个组件演示了事件过滤器的实际应用，包括：
 * - 全局事件过滤和拦截
 * - 选择性事件过滤
 * - 事件修改和转换
 * - 实时事件监控和统计
 */
class EventModificationDemo : public QWidget
{
    Q_OBJECT

public:
    explicit EventModificationDemo(QWidget *parent = nullptr);
    virtual ~EventModificationDemo();

protected:
    // 重写事件处理函数以演示事件处理
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    // 全局过滤器控制
    void onGlobalFilterToggled(bool enabled);
    void onInterceptModeToggled(bool enabled);
    void onEventModificationToggled(bool enabled);
    void onGlobalFilterReset();

    // 选择性过滤器控制
    void onSelectiveFilterToggled(bool enabled);
    void onAllowedEventTypeChanged();
    void onMouseButtonFilterChanged();
    void onKeyFilterChanged();
    void onSelectiveFilterReset();

    // 事件处理
    void onGlobalEventFiltered(QObject *watched, QEvent *event, bool intercepted);
    void onGlobalEventModified(QObject *watched, QEvent::Type originalType, QEvent::Type modifiedType);
    void onSelectiveEventPassed(QObject *watched, QEvent *event);
    void onSelectiveEventBlocked(QObject *watched, QEvent *event, const QString &reason);
    void onSelectiveEventTransformed(QObject *watched, QEvent *originalEvent, QEvent *transformedEvent);

    // 统计更新
    void updateStatistics();
    void clearEventLog();

    // 测试按钮
    void onTestButtonClicked();
    void generateTestEvents();

private:
    void setupUI();
    void setupGlobalFilterControls();
    void setupSelectiveFilterControls();
    void setupEventDisplay();
    void setupTestArea();
    void connectSignals();
    void setupCustomFilterRules();
    void logEvent(const QString &message);
    QString formatEventInfo(QObject *watched, QEvent *event) const;

    // UI组件
    QVBoxLayout *m_mainLayout;
    
    // 全局过滤器控制区
    QGroupBox *m_globalFilterGroup;
    QCheckBox *m_globalFilterEnabled;
    QCheckBox *m_interceptMode;
    QCheckBox *m_eventModification;
    QPushButton *m_globalFilterReset;
    QLabel *m_globalStatsLabel;

    // 选择性过滤器控制区
    QGroupBox *m_selectiveFilterGroup;
    QCheckBox *m_selectiveFilterEnabled;
    QComboBox *m_eventTypeCombo;
    QComboBox *m_mouseButtonCombo;
    QSpinBox *m_keyCodeSpin;
    QPushButton *m_selectiveFilterReset;
    QLabel *m_selectiveStatsLabel;

    // 事件显示区
    QGroupBox *m_eventDisplayGroup;
    QTextEdit *m_eventLog;
    QPushButton *m_clearLogButton;

    // 测试区域
    QGroupBox *m_testAreaGroup;
    QPushButton *m_testButton;
    QLabel *m_testLabel;
    QPushButton *m_generateEventsButton;

    // 过滤器实例
    GlobalEventFilter *m_globalFilter;
    SelectiveEventFilter *m_selectiveFilter;

    // 统计定时器
    QTimer *m_statsTimer;

    // 测试控件列表（用于安装事件过滤器）
    QList<QWidget*> m_testWidgets;
};

#endif // EVENT_MODIFICATION_DEMO_H