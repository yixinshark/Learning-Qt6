#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <QObject>
#include <QEvent>
#include <QDateTime>
#include <QList>
#include <QSet>
#include <QMutex>
#include <QAbstractTableModel>
#include <QVariant>

/**
 * @brief EventLogger 事件日志记录器
 * 
 * 负责记录和跟踪事件的产生、传递和处理过程
 * 提供事件流的可视化和分析工具
 * 支持事件过滤和搜索功能
 */
class EventLogger : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 事件记录结构体
     */
    struct EventRecord {
        QDateTime timestamp;        // 事件时间戳
        QObject* sender;           // 事件发送者
        QObject* receiver;         // 事件接收者
        QEvent::Type eventType;    // 事件类型
        QString eventName;         // 事件名称
        QString details;           // 事件详细信息
        bool accepted;             // 事件是否被接受
        QString senderName;        // 发送者名称（缓存）
        QString receiverName;      // 接收者名称（缓存）
        
        EventRecord() : sender(nullptr), receiver(nullptr), 
                       eventType(QEvent::None), accepted(false) {}
    };

    /**
     * @brief 获取EventLogger的单例实例
     * @return EventLogger的单例指针
     */
    static EventLogger* instance();

    /**
     * @brief 记录事件
     * @param record 事件记录
     */
    void logEvent(const EventRecord& record);

    /**
     * @brief 获取事件历史记录
     * @return 事件记录列表
     */
    QList<EventRecord> getEventHistory() const;

    /**
     * @brief 清除历史记录
     */
    void clearHistory();

    /**
     * @brief 设置事件类型过滤器
     * @param types 要记录的事件类型集合，空集合表示记录所有类型
     */
    void setEventTypeFilter(const QSet<QEvent::Type>& types);

    /**
     * @brief 设置对象过滤器
     * @param object 要记录的对象，nullptr表示记录所有对象
     */
    void setObjectFilter(QObject* object);

    /**
     * @brief 获取当前事件类型过滤器
     * @return 事件类型集合
     */
    QSet<QEvent::Type> getEventTypeFilter() const;

    /**
     * @brief 获取当前对象过滤器
     * @return 过滤对象指针
     */
    QObject* getObjectFilter() const;

    /**
     * @brief 设置最大记录数量
     * @param maxRecords 最大记录数，0表示无限制
     */
    void setMaxRecords(int maxRecords);

    /**
     * @brief 获取最大记录数量
     * @return 最大记录数
     */
    int getMaxRecords() const;

    /**
     * @brief 启用或禁用日志记录
     * @param enabled 是否启用
     */
    void setEnabled(bool enabled);

    /**
     * @brief 检查日志记录是否启用
     * @return 是否启用
     */
    bool isEnabled() const;

    /**
     * @brief 根据条件搜索事件记录
     * @param eventType 事件类型过滤，QEvent::None表示不过滤
     * @param objectName 对象名称过滤，空字符串表示不过滤
     * @param startTime 开始时间过滤
     * @param endTime 结束时间过滤
     * @return 符合条件的事件记录列表
     */
    QList<EventRecord> searchEvents(QEvent::Type eventType = QEvent::None,
                                   const QString& objectName = QString(),
                                   const QDateTime& startTime = QDateTime(),
                                   const QDateTime& endTime = QDateTime()) const;

    // 性能监控相关方法
    /**
     * @brief 获取事件类型的平均处理时间
     * @param eventType 事件类型
     * @return 平均处理时间（毫秒），-1表示无数据
     */
    double getAverageProcessingTime(QEvent::Type eventType) const;

    /**
     * @brief 获取对象的平均处理时间
     * @param object 对象指针
     * @return 平均处理时间（毫秒），-1表示无数据
     */
    double getAverageProcessingTime(QObject* object) const;

    /**
     * @brief 获取当前每秒事件数
     * @return 每秒事件数
     */
    int getEventsPerSecond() const;

    /**
     * @brief 获取性能统计信息
     * @return 性能统计的映射表
     */
    QHash<QString, QVariant> getPerformanceStats() const;

    /**
     * @brief 重置性能统计数据
     */
    void resetPerformanceStats();

    /**
     * @brief 启用或禁用性能监控
     * @param enabled 是否启用
     */
    void setPerformanceMonitoringEnabled(bool enabled);

    /**
     * @brief 检查性能监控是否启用
     * @return 是否启用
     */
    bool isPerformanceMonitoringEnabled() const;

public slots:
    /**
     * @brief 处理事件投递信号的槽函数
     * @param receiver 接收者
     * @param type 事件类型
     */
    void onEventPosted(QObject* receiver, QEvent::Type type);

    /**
     * @brief 处理事件处理信号的槽函数
     * @param receiver 接收者
     * @param type 事件类型
     * @param accepted 是否被接受
     */
    void onEventProcessed(QObject* receiver, QEvent::Type type, bool accepted);

signals:
    /**
     * @brief 当新事件被记录时发出的信号
     * @param record 事件记录
     */
    void eventLogged(const EventRecord& record);

    /**
     * @brief 当历史记录被清除时发出的信号
     */
    void historyCleared();

    /**
     * @brief 当事件计数变化时发出的信号
     * @param count 当前事件总数
     */
    void eventCountChanged(int count);

    /**
     * @brief 当性能数据更新时发出的信号
     * @param avgTime 平均处理时间(ms)
     * @param eventsPerSecond 每秒事件数
     */
    void performanceUpdate(double avgTime, int eventsPerSecond);

private:
    /**
     * @brief 私有构造函数，确保单例模式
     */
    explicit EventLogger(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~EventLogger() override = default;

    // 禁用拷贝构造和赋值操作
    EventLogger(const EventLogger&) = delete;
    EventLogger& operator=(const EventLogger&) = delete;

    /**
     * @brief 检查事件是否应该被记录
     * @param eventType 事件类型
     * @param object 相关对象
     * @return 是否应该记录
     */
    bool shouldLogEvent(QEvent::Type eventType, QObject* object) const;

    /**
     * @brief 获取对象的显示名称
     * @param object 对象指针
     * @return 显示名称
     */
    QString getObjectDisplayName(QObject* object) const;

    /**
     * @brief 限制记录数量
     */
    void limitRecords();

    /**
     * @brief 收集性能数据
     * @param record 事件记录
     */
    void collectPerformanceData(const EventRecord& record);

    // 静态实例
    static EventLogger* s_instance;
    static QMutex s_mutex;

    // 事件记录列表
    QList<EventRecord> m_eventHistory;
    mutable QMutex m_historyMutex;

    // 过滤器
    QSet<QEvent::Type> m_eventTypeFilter;
    QObject* m_objectFilter;
    mutable QMutex m_filterMutex;

    // 配置
    int m_maxRecords;
    bool m_enabled;
    bool m_performanceMonitoringEnabled;
    mutable QMutex m_configMutex;

    // 性能监控相关
    struct PerformanceData {
        QDateTime startTime;
        QDateTime endTime;
        qint64 processingTimeNs;  // 处理时间（纳秒）
        
        PerformanceData() : processingTimeNs(0) {}
    };
    
    QHash<QEvent::Type, QList<qint64>> m_eventProcessingTimes;  // 每种事件类型的处理时间列表
    QHash<QObject*, QList<qint64>> m_objectProcessingTimes;     // 每个对象的处理时间列表
    QDateTime m_lastPerformanceUpdate;                          // 上次性能更新时间
    int m_eventsInLastSecond;                                   // 上一秒的事件数量
    QList<QPair<QDateTime, int>> m_eventCountHistory;           // 事件数量历史记录
    mutable QMutex m_performanceMutex;
};

/**
 * @brief EventRecordModel 事件记录的表格数据模型
 * 
 * 为事件日志提供表格数据模型，支持Qt的Model/View架构
 */
class EventRecordModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /**
     * @brief 表格列枚举
     */
    enum Column {
        TimestampColumn = 0,
        EventTypeColumn,
        SenderColumn,
        ReceiverColumn,
        AcceptedColumn,
        DetailsColumn,
        ColumnCount
    };

    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit EventRecordModel(QObject* parent = nullptr);

    /**
     * @brief 添加事件记录
     * @param record 事件记录
     */
    void addEventRecord(const EventLogger::EventRecord& record);

    /**
     * @brief 清除所有记录
     */
    void clearRecords();

    /**
     * @brief 设置过滤器
     * @param eventType 事件类型过滤
     * @param objectName 对象名称过滤
     */
    void setFilter(QEvent::Type eventType, const QString& objectName = QString());

    /**
     * @brief 获取指定索引的事件记录
     * @param index 模型索引
     * @return 事件记录，如果索引无效则返回空记录
     */
    EventLogger::EventRecord getEventRecord(const QModelIndex& index) const;

    // QAbstractTableModel接口实现
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
    /**
     * @brief 处理新事件记录的槽函数
     * @param record 事件记录
     */
    void onEventLogged(const EventLogger::EventRecord& record);

    /**
     * @brief 处理历史清除的槽函数
     */
    void onHistoryCleared();

private:
    /**
     * @brief 检查记录是否通过过滤器
     * @param record 事件记录
     * @return 是否通过过滤器
     */
    bool passesFilter(const EventLogger::EventRecord& record) const;

    /**
     * @brief 应用过滤器
     */
    void applyFilter();

    // 所有事件记录
    QList<EventLogger::EventRecord> m_allRecords;
    
    // 过滤后的事件记录
    QList<EventLogger::EventRecord> m_filteredRecords;
    
    // 过滤器设置
    QEvent::Type m_filterEventType;
    QString m_filterObjectName;
    
    mutable QMutex m_dataMutex;
};

#endif // EVENT_LOGGER_H