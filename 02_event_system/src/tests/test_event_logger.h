#ifndef TEST_EVENT_LOGGER_H
#define TEST_EVENT_LOGGER_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QTimer>
#include <QApplication>
#include "../core/event_logger.h"
#include "../core/event_manager.h"

/**
 * @brief TestEventLogger 事件日志系统的单元测试类
 */
class TestEventLogger : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief 测试初始化和清理
     */
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    /**
     * @brief 测试EventLogger基本功能
     */
    void testSingletonInstance();
    void testEventLogging();
    void testEventHistory();
    void testClearHistory();

    /**
     * @brief 测试过滤功能
     */
    void testEventTypeFilter();
    void testObjectFilter();
    void testSearchEvents();

    /**
     * @brief 测试配置功能
     */
    void testMaxRecords();
    void testEnableDisable();

    /**
     * @brief 测试EventRecordModel
     */
    void testEventRecordModel();
    void testModelFiltering();
    void testModelSignals();

    /**
     * @brief 测试与EventManager的集成
     */
    void testEventManagerIntegration();

    /**
     * @brief 测试线程安全性
     */
    void testThreadSafety();

private:
    /**
     * @brief 创建测试事件记录
     */
    EventLogger::EventRecord createTestRecord(QEvent::Type type = QEvent::User,
                                             const QString& eventName = "TestEvent",
                                             QObject* sender = nullptr,
                                             QObject* receiver = nullptr);

    /**
     * @brief 等待信号发出
     */
    bool waitForSignal(QSignalSpy& spy, int timeout = 1000);

    EventLogger* m_logger;
    QObject* m_testSender;
    QObject* m_testReceiver;
};

#endif // TEST_EVENT_LOGGER_H