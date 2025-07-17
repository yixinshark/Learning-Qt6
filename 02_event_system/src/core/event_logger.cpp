#include "event_logger.h"
#include "event_manager.h"
#include <QDebug>
#include <QMetaObject>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <QTimer>

// 静态成员初始化
EventLogger *EventLogger::s_instance = nullptr;
QMutex EventLogger::s_mutex;

EventLogger::EventLogger(QObject *parent)
    : QObject(parent), m_objectFilter(nullptr),
      m_maxRecords(10000), // 默认最大记录数
      m_enabled(true),
      m_eventsInLastSecond(0),
      m_performanceMonitoringEnabled(true) {
  // 连接到EventManager的信号
  EventManager *eventManager = EventManager::instance();
  connect(eventManager, &EventManager::eventPosted, this,
          &EventLogger::onEventPosted);
  connect(eventManager, &EventManager::eventProcessed, this,
          &EventLogger::onEventProcessed);

  qDebug() << "EventLogger initialized";
}

EventLogger *EventLogger::instance() {
  // 双重检查锁定模式确保线程安全的单例
  if (s_instance == nullptr) {
    QMutexLocker locker(&s_mutex);
    if (s_instance == nullptr) {
      s_instance = new EventLogger();
    }
  }
  return s_instance;
}

void EventLogger::logEvent(const EventRecord &record) {
  QMutexLocker configLocker(&m_configMutex);
  if (!m_enabled) {
    return;
  }
  configLocker.unlock();

  if (!shouldLogEvent(record.eventType, record.receiver)) {
    return;
  }

  EventRecord logRecord = record;

  // 设置时间戳（如果未设置）
  if (!logRecord.timestamp.isValid()) {
    logRecord.timestamp = QDateTime::currentDateTime();
  }

  // 缓存对象名称
  logRecord.senderName = getObjectDisplayName(logRecord.sender);
  logRecord.receiverName = getObjectDisplayName(logRecord.receiver);

  // 设置事件名称（如果未设置）
  if (logRecord.eventName.isEmpty()) {
    logRecord.eventName =
        EventManager::instance()->getEventTypeName(logRecord.eventType);
  }

  {
    QMutexLocker historyLocker(&m_historyMutex);
    m_eventHistory.append(logRecord);

    // 限制记录数量
    limitRecords();
  }

  // 收集性能数据
  collectPerformanceData(logRecord);

  // 发出信号
  emit eventLogged(logRecord);
  emit eventCountChanged(m_eventHistory.size());

  qDebug() << "Logged event:" << logRecord.eventName << "from"
           << logRecord.senderName << "to" << logRecord.receiverName;
}

QList<EventLogger::EventRecord> EventLogger::getEventHistory() const {
  QMutexLocker locker(&m_historyMutex);
  return m_eventHistory;
}

void EventLogger::clearHistory() {
  {
    QMutexLocker locker(&m_historyMutex);
    m_eventHistory.clear();
  }

  emit historyCleared();
  qDebug() << "Event history cleared";
}

void EventLogger::setEventTypeFilter(const QSet<QEvent::Type> &types) {
  QMutexLocker locker(&m_filterMutex);
  m_eventTypeFilter = types;
  qDebug() << "Event type filter updated, types count:" << types.size();
}

void EventLogger::setObjectFilter(QObject *object) {
  QMutexLocker locker(&m_filterMutex);
  m_objectFilter = object;
  qDebug() << "Object filter set to:" << getObjectDisplayName(object);
}

QSet<QEvent::Type> EventLogger::getEventTypeFilter() const {
  QMutexLocker locker(&m_filterMutex);
  return m_eventTypeFilter;
}

QObject *EventLogger::getObjectFilter() const {
  QMutexLocker locker(&m_filterMutex);
  return m_objectFilter;
}

void EventLogger::setMaxRecords(int maxRecords) {
  QMutexLocker locker(&m_configMutex);
  m_maxRecords = maxRecords;
  qDebug() << "Max records set to:" << maxRecords;
}

int EventLogger::getMaxRecords() const {
  QMutexLocker locker(&m_configMutex);
  return m_maxRecords;
}

void EventLogger::setEnabled(bool enabled) {
  QMutexLocker locker(&m_configMutex);
  m_enabled = enabled;
  qDebug() << "Event logging" << (enabled ? "enabled" : "disabled");
}

bool EventLogger::isEnabled() const {
  QMutexLocker locker(&m_configMutex);
  return m_enabled;
}

QList<EventLogger::EventRecord>
EventLogger::searchEvents(QEvent::Type eventType, const QString &objectName,
                          const QDateTime &startTime,
                          const QDateTime &endTime) const {
  QMutexLocker locker(&m_historyMutex);
  QList<EventRecord> results;

  for (const EventRecord &record : m_eventHistory) {
    // 事件类型过滤
    if (eventType != QEvent::None && record.eventType != eventType) {
      continue;
    }

    // 对象名称过滤
    if (!objectName.isEmpty()) {
      if (!record.senderName.contains(objectName, Qt::CaseInsensitive) &&
          !record.receiverName.contains(objectName, Qt::CaseInsensitive)) {
        continue;
      }
    }

    // 时间范围过滤
    if (startTime.isValid() && record.timestamp < startTime) {
      continue;
    }
    if (endTime.isValid() && record.timestamp > endTime) {
      continue;
    }

    results.append(record);
  }

  return results;
}

void EventLogger::onEventPosted(QObject *receiver, QEvent::Type type) {
  EventRecord record;
  record.timestamp = QDateTime::currentDateTime();
  record.sender = nullptr; // 投递事件时发送者未知
  record.receiver = receiver;
  record.eventType = type;
  record.eventName = EventManager::instance()->getEventTypeName(type);
  record.details = "Event posted";
  record.accepted = false; // 投递时尚未处理

  logEvent(record);
}

void EventLogger::onEventProcessed(QObject *receiver, QEvent::Type type,
                                   bool accepted) {
  EventRecord record;
  record.timestamp = QDateTime::currentDateTime();
  record.sender = nullptr; // 处理事件时发送者未知
  record.receiver = receiver;
  record.eventType = type;
  record.eventName = EventManager::instance()->getEventTypeName(type);
  record.details =
      QString("Event processed - %1").arg(accepted ? "accepted" : "ignored");
  record.accepted = accepted;

  logEvent(record);
}

bool EventLogger::shouldLogEvent(QEvent::Type eventType,
                                 QObject *object) const {
  QMutexLocker locker(&m_filterMutex);

  // 检查事件类型过滤器
  if (!m_eventTypeFilter.isEmpty() && !m_eventTypeFilter.contains(eventType)) {
    return false;
  }

  // 检查对象过滤器
  if (m_objectFilter != nullptr && object != m_objectFilter) {
    return false;
  }

  return true;
}

QString EventLogger::getObjectDisplayName(QObject *object) const {
  if (!object) {
    return "nullptr";
  }

  QString name = object->objectName();
  if (name.isEmpty()) {
    name = object->metaObject()->className();
    name += QString("@%1").arg(reinterpret_cast<quintptr>(object), 0, 16);
  }

  return name;
}

void EventLogger::limitRecords() {
  // 注意：此函数应在已获取m_historyMutex锁的情况下调用
  QMutexLocker configLocker(&m_configMutex);
  int maxRecords = m_maxRecords;
  configLocker.unlock();

  if (maxRecords > 0 && m_eventHistory.size() > maxRecords) {
    int removeCount = m_eventHistory.size() - maxRecords;
    m_eventHistory.erase(m_eventHistory.begin(),
                         m_eventHistory.begin() + removeCount);
    qDebug() << "Removed" << removeCount << "old event records";
  }
}

// EventRecordModel 实现

EventRecordModel::EventRecordModel(QObject *parent)
    : QAbstractTableModel(parent), m_filterEventType(QEvent::None) {
  // 连接到EventLogger的信号
  EventLogger *logger = EventLogger::instance();
  connect(logger, &EventLogger::eventLogged, this,
          &EventRecordModel::onEventLogged);
  connect(logger, &EventLogger::historyCleared, this,
          &EventRecordModel::onHistoryCleared);
}

void EventRecordModel::addEventRecord(const EventLogger::EventRecord &record) {
  QMutexLocker locker(&m_dataMutex);

  m_allRecords.append(record);

  if (passesFilter(record)) {
    beginInsertRows(QModelIndex(), m_filteredRecords.size(),
                    m_filteredRecords.size());
    m_filteredRecords.append(record);
    endInsertRows();
  }
}

void EventRecordModel::clearRecords() {
  QMutexLocker locker(&m_dataMutex);

  beginResetModel();
  m_allRecords.clear();
  m_filteredRecords.clear();
  endResetModel();
}

void EventRecordModel::setFilter(QEvent::Type eventType,
                                 const QString &objectName) {
  QMutexLocker locker(&m_dataMutex);

  m_filterEventType = eventType;
  m_filterObjectName = objectName;

  applyFilter();
}

EventLogger::EventRecord
EventRecordModel::getEventRecord(const QModelIndex &index) const {
  // Don't lock here as it can cause deadlock during model operations
  if (!index.isValid() || index.row() >= m_filteredRecords.size()) {
    return EventLogger::EventRecord();
  }

  return m_filteredRecords.at(index.row());
}

int EventRecordModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  // Don't lock here as it can cause deadlock during model operations
  return m_filteredRecords.size();
}

int EventRecordModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return ColumnCount;
}

QVariant EventRecordModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }

  // Don't lock here as it can cause deadlock during model operations
  if (index.row() >= m_filteredRecords.size()) {
    return QVariant();
  }

  const EventLogger::EventRecord &record = m_filteredRecords.at(index.row());

  if (role == Qt::DisplayRole) {
    switch (index.column()) {
    case TimestampColumn:
      return record.timestamp.toString("hh:mm:ss.zzz");
    case EventTypeColumn:
      return record.eventName;
    case SenderColumn:
      return record.senderName;
    case ReceiverColumn:
      return record.receiverName;
    case AcceptedColumn:
      return record.accepted ? "Yes" : "No";
    case DetailsColumn:
      return record.details;
    default:
      return QVariant();
    }
  } else if (role == Qt::ToolTipRole) {
    return QString("Event: %1\nTime: %2\nSender: %3\nReceiver: %4\nAccepted: "
                   "%5\nDetails: %6")
        .arg(record.eventName)
        .arg(record.timestamp.toString())
        .arg(record.senderName)
        .arg(record.receiverName)
        .arg(record.accepted ? "Yes" : "No")
        .arg(record.details);
  }

  return QVariant();
}

QVariant EventRecordModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
    return QVariant();
  }

  switch (section) {
  case TimestampColumn:
    return "Time";
  case EventTypeColumn:
    return "Event Type";
  case SenderColumn:
    return "Sender";
  case ReceiverColumn:
    return "Receiver";
  case AcceptedColumn:
    return "Accepted";
  case DetailsColumn:
    return "Details";
  default:
    return QVariant();
  }
}

void EventRecordModel::onEventLogged(const EventLogger::EventRecord &record) {
  addEventRecord(record);
}

void EventRecordModel::onHistoryCleared() { clearRecords(); }

bool EventRecordModel::passesFilter(
    const EventLogger::EventRecord &record) const {
  // 事件类型过滤
  if (m_filterEventType != QEvent::None &&
      record.eventType != m_filterEventType) {
    return false;
  }

  // 对象名称过滤
  if (!m_filterObjectName.isEmpty()) {
    if (!record.senderName.contains(m_filterObjectName, Qt::CaseInsensitive) &&
        !record.receiverName.contains(m_filterObjectName,
                                      Qt::CaseInsensitive)) {
      return false;
    }
  }

  return true;
}

void EventRecordModel::applyFilter() {
  // 注意：此函数应在已获取m_dataMutex锁的情况下调用
  beginResetModel();

  m_filteredRecords.clear();
  for (const EventLogger::EventRecord &record : m_allRecords) {
    if (passesFilter(record)) {
      m_filteredRecords.append(record);
    }
  }

  endResetModel();
}

// 性能监控相关方法实现

double EventLogger::getAverageProcessingTime(QEvent::Type eventType) const {
    QMutexLocker locker(&m_performanceMutex);
    
    if (!m_eventProcessingTimes.contains(eventType)) {
        return -1.0; // 无数据
    }
    
    const QList<qint64>& times = m_eventProcessingTimes[eventType];
    if (times.isEmpty()) {
        return -1.0;
    }
    
    qint64 totalTime = 0;
    for (qint64 time : times) {
        totalTime += time;
    }
    
    // 转换为毫秒
    return static_cast<double>(totalTime) / times.size() / 1000000.0;
}

double EventLogger::getAverageProcessingTime(QObject* object) const {
    QMutexLocker locker(&m_performanceMutex);
    
    if (!m_objectProcessingTimes.contains(object)) {
        return -1.0; // 无数据
    }
    
    const QList<qint64>& times = m_objectProcessingTimes[object];
    if (times.isEmpty()) {
        return -1.0;
    }
    
    qint64 totalTime = 0;
    for (qint64 time : times) {
        totalTime += time;
    }
    
    // 转换为毫秒
    return static_cast<double>(totalTime) / times.size() / 1000000.0;
}

int EventLogger::getEventsPerSecond() const {
    QMutexLocker locker(&m_performanceMutex);
    
    QDateTime now = QDateTime::currentDateTime();
    QDateTime oneSecondAgo = now.addSecs(-1);
    
    int count = 0;
    for (const auto& entry : m_eventCountHistory) {
        if (entry.first >= oneSecondAgo) {
            count += entry.second;
        }
    }
    
    return count;
}

QHash<QString, QVariant> EventLogger::getPerformanceStats() const {
    QMutexLocker locker(&m_performanceMutex);
    QHash<QString, QVariant> stats;
    
    // 总体统计
    stats["totalEvents"] = m_eventHistory.size();
    
    // 直接计算每秒事件数，避免死锁
    QDateTime now = QDateTime::currentDateTime();
    QDateTime oneSecondAgo = now.addSecs(-1);
    int eventsPerSecond = 0;
    for (const auto& entry : m_eventCountHistory) {
        if (entry.first >= oneSecondAgo) {
            eventsPerSecond += entry.second;
        }
    }
    stats["eventsPerSecond"] = eventsPerSecond;
    
    // 事件类型统计
    QHash<QEvent::Type, int> eventTypeCounts;
    QHash<QEvent::Type, double> eventTypeAvgTimes;
    
    for (auto it = m_eventProcessingTimes.begin(); it != m_eventProcessingTimes.end(); ++it) {
        QEvent::Type type = it.key();
        const QList<qint64>& times = it.value();
        
        eventTypeCounts[type] = times.size();
        if (!times.isEmpty()) {
            qint64 totalTime = 0;
            for (qint64 time : times) {
                totalTime += time;
            }
            eventTypeAvgTimes[type] = static_cast<double>(totalTime) / times.size() / 1000000.0;
        }
    }
    
    // 将统计数据转换为可序列化的格式
    QVariantMap eventTypeStats;
    for (auto it = eventTypeCounts.begin(); it != eventTypeCounts.end(); ++it) {
        QString typeName = EventManager::instance()->getEventTypeName(it.key());
        QVariantMap typeStats;
        typeStats["count"] = it.value();
        typeStats["avgTime"] = eventTypeAvgTimes.value(it.key(), 0.0);
        eventTypeStats[typeName] = typeStats;
    }
    stats["eventTypes"] = eventTypeStats;
    
    // 对象统计
    QVariantMap objectStats;
    for (auto it = m_objectProcessingTimes.begin(); it != m_objectProcessingTimes.end(); ++it) {
        QObject* obj = it.key();
        const QList<qint64>& times = it.value();
        
        if (obj && !times.isEmpty()) {
            QString objName = getObjectDisplayName(obj);
            QVariantMap objStat;
            objStat["count"] = times.size();
            
            qint64 totalTime = 0;
            for (qint64 time : times) {
                totalTime += time;
            }
            objStat["avgTime"] = static_cast<double>(totalTime) / times.size() / 1000000.0;
            objectStats[objName] = objStat;
        }
    }
    stats["objects"] = objectStats;
    
    return stats;
}

void EventLogger::resetPerformanceStats() {
    QMutexLocker locker(&m_performanceMutex);
    
    m_eventProcessingTimes.clear();
    m_objectProcessingTimes.clear();
    m_eventCountHistory.clear();
    m_eventsInLastSecond = 0;
    m_lastPerformanceUpdate = QDateTime::currentDateTime();
    
    qDebug() << "Performance statistics reset";
}

void EventLogger::setPerformanceMonitoringEnabled(bool enabled) {
    QMutexLocker locker(&m_configMutex);
    m_performanceMonitoringEnabled = enabled;
    qDebug() << "Performance monitoring" << (enabled ? "enabled" : "disabled");
}

bool EventLogger::isPerformanceMonitoringEnabled() const {
    QMutexLocker locker(&m_configMutex);
    return m_performanceMonitoringEnabled;
}

void EventLogger::collectPerformanceData(const EventRecord& record) {
    QMutexLocker configLocker(&m_configMutex);
    if (!m_performanceMonitoringEnabled) {
        return;
    }
    configLocker.unlock();

    QMutexLocker locker(&m_performanceMutex);
    
    // 更新事件计数历史
    QDateTime now = QDateTime::currentDateTime();
    m_eventCountHistory.append(qMakePair(now, 1));
    
    // 清理超过1分钟的历史记录
    QDateTime oneMinuteAgo = now.addSecs(-60);
    auto it = m_eventCountHistory.begin();
    while (it != m_eventCountHistory.end()) {
        if (it->first < oneMinuteAgo) {
            it = m_eventCountHistory.erase(it);
        } else {
            ++it;
        }
    }
    
    // 模拟处理时间（在实际应用中，这应该是实际测量的处理时间）
    // 这里我们使用一个简单的启发式方法来估算处理时间
    qint64 estimatedProcessingTime = 1000000; // 1ms in nanoseconds as base
    
    // 根据事件类型调整处理时间
    switch (record.eventType) {
        case QEvent::Paint:
            estimatedProcessingTime *= 5; // 绘制事件通常较慢
            break;
        case QEvent::Resize:
            estimatedProcessingTime *= 3; // 调整大小事件中等
            break;
        case QEvent::MouseMove:
            estimatedProcessingTime *= 1; // 鼠标移动事件很快
            break;
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
            estimatedProcessingTime *= 2; // 键盘事件中等
            break;
        default:
            estimatedProcessingTime *= 2; // 默认中等处理时间
            break;
    }
    
    // 添加一些随机性来模拟真实的处理时间变化
    estimatedProcessingTime += (rand() % 500000); // 添加0-0.5ms的随机变化
    
    // 记录事件类型的处理时间
    if (!m_eventProcessingTimes.contains(record.eventType)) {
        m_eventProcessingTimes[record.eventType] = QList<qint64>();
    }
    m_eventProcessingTimes[record.eventType].append(estimatedProcessingTime);
    
    // 限制每种事件类型的记录数量（保留最近的100条记录）
    if (m_eventProcessingTimes[record.eventType].size() > 100) {
        m_eventProcessingTimes[record.eventType].removeFirst();
    }
    
    // 记录对象的处理时间
    if (record.receiver) {
        if (!m_objectProcessingTimes.contains(record.receiver)) {
            m_objectProcessingTimes[record.receiver] = QList<qint64>();
        }
        m_objectProcessingTimes[record.receiver].append(estimatedProcessingTime);
        
        // 限制每个对象的记录数量（保留最近的100条记录）
        if (m_objectProcessingTimes[record.receiver].size() > 100) {
            m_objectProcessingTimes[record.receiver].removeFirst();
        }
    }
    
    // 定期发出性能更新信号（每秒一次）
    if (m_lastPerformanceUpdate.isNull() || 
        m_lastPerformanceUpdate.secsTo(now) >= 1) {
        
        m_lastPerformanceUpdate = now;
        
        // 计算总体平均处理时间
        qint64 totalTime = 0;
        int totalCount = 0;
        
        for (auto it = m_eventProcessingTimes.begin(); it != m_eventProcessingTimes.end(); ++it) {
            const QList<qint64>& times = it.value();
            for (qint64 time : times) {
                totalTime += time;
                totalCount++;
            }
        }
        
        double avgTime = totalCount > 0 ? static_cast<double>(totalTime) / totalCount / 1000000.0 : 0.0;
        int eventsPerSec = getEventsPerSecond();
        
        // 发出性能更新信号（需要在解锁后发出以避免死锁）
        locker.unlock();
        emit performanceUpdate(avgTime, eventsPerSec);
    }
}