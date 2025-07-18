#include "test_event_logger.h"
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QRandomGenerator>

void TestEventLogger::initTestCase()
{
    // 初始化测试环境
    m_logger = EventLogger::instance();
    QVERIFY(m_logger != nullptr);
    
    // 创建测试对象
    m_testSender = new QObject(this);
    m_testSender->setObjectName("TestSender");
    
    m_testReceiver = new QObject(this);
    m_testReceiver->setObjectName("TestReceiver");
}

void TestEventLogger::cleanupTestCase()
{
    // 清理测试环境
    if (m_testSender) {
        delete m_testSender;
        m_testSender = nullptr;
    }
    
    if (m_testReceiver) {
        delete m_testReceiver;
        m_testReceiver = nullptr;
    }
}

void TestEventLogger::init()
{
    // 每个测试前的初始化
    m_logger->clearHistory();
    m_logger->setEnabled(true);
    m_logger->setEventTypeFilter(QSet<QEvent::Type>());
    m_logger->setObjectFilter(nullptr);
    m_logger->setMaxRecords(10000);
}

void TestEventLogger::cleanup()
{
    // 每个测试后的清理
    m_logger->clearHistory();
}

void TestEventLogger::testSingletonInstance()
{
    // 测试单例模式
    EventLogger* logger1 = EventLogger::instance();
    EventLogger* logger2 = EventLogger::instance();
    
    QVERIFY(logger1 != nullptr);
    QVERIFY(logger2 != nullptr);
    QCOMPARE(logger1, logger2);
}

void TestEventLogger::testEventLogging()
{
    // 测试事件记录功能
    QSignalSpy loggedSpy(m_logger, &EventLogger::eventLogged);
    
    EventLogger::EventRecord record = createTestRecord();
    m_logger->logEvent(record);
    
    QVERIFY(waitForSignal(loggedSpy));
    QCOMPARE(loggedSpy.count(), 1);
    
    QList<EventLogger::EventRecord> history = m_logger->getEventHistory();
    QCOMPARE(history.size(), 1);
    
    const EventLogger::EventRecord& loggedRecord = history.first();
    QCOMPARE(loggedRecord.eventType, record.eventType);
    QCOMPARE(loggedRecord.eventName, record.eventName);
    QCOMPARE(loggedRecord.sender, record.sender);
    QCOMPARE(loggedRecord.receiver, record.receiver);
}

void TestEventLogger::testEventHistory()
{
    // 测试事件历史记录
    const int recordCount = 5;
    
    for (int i = 0; i < recordCount; ++i) {
        EventLogger::EventRecord record = createTestRecord(
            static_cast<QEvent::Type>(QEvent::User + i),
            QString("TestEvent%1").arg(i)
        );
        m_logger->logEvent(record);
    }
    
    QList<EventLogger::EventRecord> history = m_logger->getEventHistory();
    QCOMPARE(history.size(), recordCount);
    
    // 验证记录顺序
    for (int i = 0; i < recordCount; ++i) {
        QCOMPARE(history[i].eventName, QString("TestEvent%1").arg(i));
    }
}

void TestEventLogger::testClearHistory()
{
    // 测试清除历史记录
    QSignalSpy clearedSpy(m_logger, &EventLogger::historyCleared);
    
    // 添加一些记录
    for (int i = 0; i < 3; ++i) {
        m_logger->logEvent(createTestRecord());
    }
    
    QCOMPARE(m_logger->getEventHistory().size(), 3);
    
    // 清除历史记录
    m_logger->clearHistory();
    
    QVERIFY(waitForSignal(clearedSpy));
    QCOMPARE(clearedSpy.count(), 1);
    QCOMPARE(m_logger->getEventHistory().size(), 0);
}

void TestEventLogger::testEventTypeFilter()
{
    // 测试事件类型过滤器
    QSet<QEvent::Type> filter;
    filter.insert(QEvent::MouseButtonPress);
    filter.insert(QEvent::KeyPress);
    
    m_logger->setEventTypeFilter(filter);
    QCOMPARE(m_logger->getEventTypeFilter(), filter);
    
    // 记录应该被过滤的事件
    m_logger->logEvent(createTestRecord(QEvent::MouseMove, "MouseMove"));
    QCOMPARE(m_logger->getEventHistory().size(), 0);
    
    // 记录应该通过过滤器的事件
    m_logger->logEvent(createTestRecord(QEvent::MouseButtonPress, "MousePress"));
    QCOMPARE(m_logger->getEventHistory().size(), 1);
    
    m_logger->logEvent(createTestRecord(QEvent::KeyPress, "KeyPress"));
    QCOMPARE(m_logger->getEventHistory().size(), 2);
}

void TestEventLogger::testObjectFilter()
{
    // 测试对象过滤器
    m_logger->setObjectFilter(m_testReceiver);
    QCOMPARE(m_logger->getObjectFilter(), m_testReceiver);
    
    // 记录应该被过滤的事件
    m_logger->logEvent(createTestRecord(QEvent::User, "TestEvent", nullptr, m_testSender));
    QCOMPARE(m_logger->getEventHistory().size(), 0);
    
    // 记录应该通过过滤器的事件
    m_logger->logEvent(createTestRecord(QEvent::User, "TestEvent", nullptr, m_testReceiver));
    QCOMPARE(m_logger->getEventHistory().size(), 1);
}

void TestEventLogger::testSearchEvents()
{
    // 测试事件搜索功能
    QDateTime startTime = QDateTime::currentDateTime();
    
    // 添加不同类型的事件
    m_logger->logEvent(createTestRecord(QEvent::MouseButtonPress, "MousePress", m_testSender, m_testReceiver));
    QThread::msleep(10);  // 确保时间戳不同
    
    m_logger->logEvent(createTestRecord(QEvent::KeyPress, "KeyPress", m_testSender, m_testReceiver));
    QThread::msleep(10);
    
    m_logger->logEvent(createTestRecord(QEvent::MouseMove, "MouseMove", m_testSender, m_testReceiver));
    
    QDateTime endTime = QDateTime::currentDateTime();
    
    // 按事件类型搜索
    QList<EventLogger::EventRecord> mouseEvents = m_logger->searchEvents(QEvent::MouseButtonPress);
    QCOMPARE(mouseEvents.size(), 1);
    QCOMPARE(mouseEvents.first().eventName, QString("MousePress"));
    
    // 按对象名称搜索
    QList<EventLogger::EventRecord> senderEvents = m_logger->searchEvents(QEvent::None, "TestSender");
    QCOMPARE(senderEvents.size(), 3);
    
    // 按时间范围搜索
    QList<EventLogger::EventRecord> timeRangeEvents = m_logger->searchEvents(QEvent::None, QString(), startTime, endTime);
    QCOMPARE(timeRangeEvents.size(), 3);
}

void TestEventLogger::testMaxRecords()
{
    // 测试最大记录数限制
    const int maxRecords = 3;
    m_logger->setMaxRecords(maxRecords);
    QCOMPARE(m_logger->getMaxRecords(), maxRecords);
    
    // 添加超过最大数量的记录
    for (int i = 0; i < maxRecords + 2; ++i) {
        m_logger->logEvent(createTestRecord(static_cast<QEvent::Type>(QEvent::User + i), QString("Event%1").arg(i)));
    }
    
    QList<EventLogger::EventRecord> history = m_logger->getEventHistory();
    QCOMPARE(history.size(), maxRecords);
    
    // 验证保留的是最新的记录
    QCOMPARE(history.last().eventName, QString("Event%1").arg(maxRecords + 1));
}

void TestEventLogger::testEnableDisable()
{
    // 测试启用/禁用功能
    m_logger->setEnabled(false);
    QVERIFY(!m_logger->isEnabled());
    
    // 禁用时不应记录事件
    m_logger->logEvent(createTestRecord());
    QCOMPARE(m_logger->getEventHistory().size(), 0);
    
    // 启用后应该记录事件
    m_logger->setEnabled(true);
    QVERIFY(m_logger->isEnabled());
    
    m_logger->logEvent(createTestRecord());
    QCOMPARE(m_logger->getEventHistory().size(), 1);
}

void TestEventLogger::testEventRecordModel()
{
    // 测试EventRecordModel
    EventRecordModel model;
    
    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    
    // 添加事件记录
    EventLogger::EventRecord record = createTestRecord();
    model.addEventRecord(record);
    
    // 直接检查结果，不等待信号（因为addEventRecord是同步的）
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.columnCount(), EventRecordModel::ColumnCount);
    
    // 验证数据
    QModelIndex index = model.index(0, EventRecordModel::EventTypeColumn);
    QCOMPARE(model.data(index, Qt::DisplayRole).toString(), record.eventName);
    
    // 测试获取事件记录
    EventLogger::EventRecord retrievedRecord = model.getEventRecord(index);
    QCOMPARE(retrievedRecord.eventName, record.eventName);
    
    // 验证信号确实被发出了
    QVERIFY(rowsInsertedSpy.count() > 0);
}

void TestEventLogger::testModelFiltering()
{
    // 测试模型过滤功能
    EventRecordModel model;
    
    // 添加不同类型的事件
    model.addEventRecord(createTestRecord(QEvent::MouseButtonPress, "MousePress"));
    model.addEventRecord(createTestRecord(QEvent::KeyPress, "KeyPress"));
    model.addEventRecord(createTestRecord(QEvent::MouseMove, "MouseMove"));
    
    QCOMPARE(model.rowCount(), 3);
    
    // 设置事件类型过滤器
    model.setFilter(QEvent::MouseButtonPress);
    QCOMPARE(model.rowCount(), 1);
    
    QModelIndex index = model.index(0, EventRecordModel::EventTypeColumn);
    QCOMPARE(model.data(index, Qt::DisplayRole).toString(), QString("MousePress"));
    
    // 清除过滤器
    model.setFilter(QEvent::None);
    QCOMPARE(model.rowCount(), 3);
}

void TestEventLogger::testModelSignals()
{
    // 测试模型信号连接
    EventRecordModel model;
    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy modelResetSpy(&model, &QAbstractItemModel::modelReset);
    
    // 通过EventLogger添加记录应该触发模型更新
    m_logger->logEvent(createTestRecord());
    
    QVERIFY(waitForSignal(rowsInsertedSpy));
    QCOMPARE(model.rowCount(), 1);
    
    // 清除历史应该触发模型重置
    m_logger->clearHistory();
    
    QVERIFY(waitForSignal(modelResetSpy));
    QCOMPARE(model.rowCount(), 0);
}

void TestEventLogger::testEventManagerIntegration()
{
    // 测试与EventManager的集成
    QSignalSpy loggedSpy(m_logger, &EventLogger::eventLogged);
    
    EventManager* eventManager = EventManager::instance();
    
    // 创建一个简单的事件
    QEvent* event = new QEvent(QEvent::User);
    
    // 发送事件应该触发日志记录
    eventManager->postCustomEvent(m_testReceiver, event);
    
    // 等待事件处理
    QCoreApplication::processEvents();
    
    QVERIFY(waitForSignal(loggedSpy));
    QVERIFY(loggedSpy.count() > 0);
}

void TestEventLogger::testThreadSafety()
{
    // 测试线程安全性
    const int threadCount = 3;
    const int eventsPerThread = 10;
    
    QList<QThread*> threads;
    QMutex finishedMutex;
    QWaitCondition finishedCondition;
    int finishedThreads = 0;
    
    // 创建多个线程同时记录事件
    for (int i = 0; i < threadCount; ++i) {
        QThread* thread = QThread::create([this, i, eventsPerThread, &finishedMutex, &finishedCondition, &finishedThreads]() {
            for (int j = 0; j < eventsPerThread; ++j) {
                EventLogger::EventRecord record = createTestRecord(
                    static_cast<QEvent::Type>(QEvent::User + j),
                    QString("Thread%1_Event%2").arg(i).arg(j)
                );
                m_logger->logEvent(record);
                
                // 随机延迟
                QThread::msleep(QRandomGenerator::global()->bounded(5));
            }
            
            QMutexLocker locker(&finishedMutex);
            finishedThreads++;
            finishedCondition.wakeAll();
        });
        
        threads.append(thread);
        thread->start();
    }
    
    // 等待所有线程完成
    QMutexLocker locker(&finishedMutex);
    while (finishedThreads < threadCount) {
        finishedCondition.wait(&finishedMutex, 5000);
    }
    
    // 等待线程结束
    for (QThread* thread : threads) {
        thread->wait();
        delete thread;
    }
    
    // 验证所有事件都被记录
    QList<EventLogger::EventRecord> history = m_logger->getEventHistory();
    QCOMPARE(history.size(), threadCount * eventsPerThread);
}

EventLogger::EventRecord TestEventLogger::createTestRecord(QEvent::Type type,
                                                          const QString& eventName,
                                                          QObject* sender,
                                                          QObject* receiver)
{
    EventLogger::EventRecord record;
    record.timestamp = QDateTime::currentDateTime();
    record.eventType = type;
    record.eventName = eventName;
    record.sender = sender ? sender : m_testSender;
    record.receiver = receiver ? receiver : m_testReceiver;
    record.details = QString("Test event: %1").arg(eventName);
    record.accepted = true;
    
    return record;
}

bool TestEventLogger::waitForSignal(QSignalSpy& spy, int timeout)
{
    if (spy.count() > 0) {
        return true;
    }
    
    return spy.wait(timeout);
}

// 注册测试类
QTEST_MAIN(TestEventLogger)
// #include "test_event_logger.moc" // 移除不需要的moc包含