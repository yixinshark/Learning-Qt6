#ifndef EVENT_INTERCEPTION_DEMO_H
#define EVENT_INTERCEPTION_DEMO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QProgressBar>
#include <QTabWidget>
#include <QSplitter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QTimer>
#include <QDateTime>
#include <QEventLoop>
#include <QApplication>

/**
 * @brief 事件拦截演示组件
 * 
 * 这个组件演示如何拦截、修改和控制事件的传播过程。
 * 包括事件过滤、事件阻止、事件修改和条件性传播等高级技术。
 */
class EventInterceptionDemo : public QWidget
{
    Q_OBJECT

public:
    explicit EventInterceptionDemo(QWidget *parent = nullptr);
    ~EventInterceptionDemo() = default;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onInterceptionModeChanged();
    void onEventTypeFilterChanged();
    void onClearLog();
    void onResetDemo();
    void onToggleGlobalFilter();

private:
    void setupUI();
    void setupInterceptionControls();
    void setupTestArea();
    void setupLogArea();
    void installEventFilters();
    void removeEventFilters();
    void logInterceptionEvent(const QString &source, const QString &action, 
                             const QString &eventType, const QString &details);
    bool shouldInterceptEvent(QEvent *event, QObject *source);
    QEvent* modifyEvent(QEvent *event);
    
    // UI组件
    QHBoxLayout *m_mainLayout;
    QSplitter *m_splitter;
    
    // 左侧控制面板
    QGroupBox *m_controlGroup;
    QVBoxLayout *m_controlLayout;
    
    // 拦截模式控制
    QGroupBox *m_modeGroup;
    QVBoxLayout *m_modeLayout;
    QComboBox *m_interceptionModeCombo;
    QCheckBox *m_globalFilterCheckBox;
    QCheckBox *m_modifyEventsCheckBox;
    QCheckBox *m_logAllEventsCheckBox;
    
    // 事件类型过滤
    QGroupBox *m_filterGroup;
    QVBoxLayout *m_filterLayout;
    QCheckBox *m_mouseEventsCheckBox;
    QCheckBox *m_keyboardEventsCheckBox;
    QCheckBox *m_focusEventsCheckBox;
    QCheckBox *m_paintEventsCheckBox;
    
    // 拦截参数
    QGroupBox *m_paramGroup;
    QVBoxLayout *m_paramLayout;
    QLabel *m_delayLabel;
    QSpinBox *m_delaySpinBox;
    QLabel *m_probabilityLabel;
    QSlider *m_probabilitySlider;
    QProgressBar *m_probabilityBar;
    
    // 控制按钮
    QPushButton *m_resetButton;
    QPushButton *m_clearLogButton;
    
    // 中间测试区域
    QGroupBox *m_testGroup;
    QVBoxLayout *m_testLayout;
    QTabWidget *m_testTabs;
    
    // 测试控件
    QWidget *m_mouseTestWidget;
    QWidget *m_keyboardTestWidget;
    QWidget *m_complexTestWidget;
    
    // 右侧日志区域
    QGroupBox *m_logGroup;
    QVBoxLayout *m_logLayout;
    QTextEdit *m_logDisplay;
    
    // 拦截控制状态
    enum InterceptionMode {
        NoInterception,
        BlockAll,
        BlockSelected,
        ModifyEvents,
        ConditionalBlock
    };
    
    InterceptionMode m_currentMode;
    bool m_globalFilterEnabled;
    bool m_modifyEvents;
    bool m_logAllEvents;
    int m_interceptionDelay;
    int m_interceptionProbability;
    
    // 事件统计
    int m_totalEvents;
    int m_interceptedEvents;
    int m_modifiedEvents;
    int m_blockedEvents;
    
    QTimer *m_statsUpdateTimer;
};

/**
 * @brief 可拦截的测试控件
 * 
 * 专门用于测试事件拦截的自定义控件。
 */
class InterceptableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InterceptableWidget(const QString &name, QWidget *parent = nullptr);
    
    void setInterceptionEnabled(bool enabled) { m_interceptionEnabled = enabled; }
    bool isInterceptionEnabled() const { return m_interceptionEnabled; }
    
    const QString& widgetName() const { return m_name; }

signals:
    void eventProcessed(const QString &widgetName, const QString &eventType, 
                       const QString &details, bool intercepted);

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString getEventDetails(QEvent *event);
    
    QString m_name;
    bool m_interceptionEnabled;
    QColor m_backgroundColor;
    int m_eventCount;
};



#endif // EVENT_INTERCEPTION_DEMO_H