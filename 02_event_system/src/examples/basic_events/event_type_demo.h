#ifndef EVENT_TYPE_DEMO_H
#define EVENT_TYPE_DEMO_H

#include <QWidget>
#include <QEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QMap>

/**
 * EventTypeDemo - 展示不同事件类型的特点和属性
 * 
 * 这个类展示了：
 * 1. Qt中各种事件类型的分类和特点
 * 2. 事件类型的枚举值和名称对应关系
 * 3. 不同事件类型的属性和数据结构
 * 4. 事件类型的继承关系和分类
 */
class EventTypeDemo : public QWidget
{
    Q_OBJECT

public:
    explicit EventTypeDemo(QWidget *parent = nullptr);
    ~EventTypeDemo() override = default;

protected:
    bool event(QEvent *event) override;

private slots:
    void onEventTypeSelected();
    void onSimulateEvent();
    void onClearDetails();

private:
    void setupUI();
    void initializeEventTypes();
    void updateEventDetails(QEvent::Type eventType);
    void simulateSelectedEvent();
    QString getEventDescription(QEvent::Type eventType) const;
    QString getEventCategory(QEvent::Type eventType) const;
    void logEventOccurrence(QEvent *event, const QString &customMessage = QString());
    
    // UI组件
    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QComboBox *m_eventTypeCombo;
    QTableWidget *m_eventPropertiesTable;
    QTextEdit *m_eventDescription;
    QPushButton *m_simulateButton;
    QPushButton *m_clearButton;
    QTextEdit *m_eventLog;
    QLabel *m_statusLabel;
    
    // 数据结构
    struct EventTypeInfo {
        QEvent::Type type;
        QString name;
        QString category;
        QString description;
        QStringList properties;
        bool canSimulate;
    };
    
    QMap<QEvent::Type, EventTypeInfo> m_eventTypes;
    QTimer *m_statusTimer;
    int m_eventCounter;
};

#endif // EVENT_TYPE_DEMO_H