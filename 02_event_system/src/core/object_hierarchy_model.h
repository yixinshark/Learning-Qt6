#ifndef OBJECT_HIERARCHY_MODEL_H
#define OBJECT_HIERARCHY_MODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QHash>
#include <QTimer>
#include <QMutex>

/**
 * @brief ObjectHierarchyModel 对象层次结构模型
 * 
 * 用于显示Qt对象的层次结构，便于性能监控和调试
 * 支持实时更新对象树结构
 */
class ObjectHierarchyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    /**
     * @brief 列枚举
     */
    enum Column {
        NameColumn = 0,         // 对象名称
        ClassColumn,            // 类名
        AddressColumn,          // 内存地址
        ChildCountColumn,       // 子对象数量
        EventCountColumn,       // 事件数量
        AvgTimeColumn,          // 平均处理时间
        ColumnCount
    };

    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit ObjectHierarchyModel(QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ObjectHierarchyModel() override = default;

    /**
     * @brief 设置根对象
     * @param root 根对象指针
     */
    void setRootObject(QObject* root);

    /**
     * @brief 获取根对象
     * @return 根对象指针
     */
    QObject* getRootObject() const;

    /**
     * @brief 刷新模型数据
     */
    void refresh();

    /**
     * @brief 获取指定索引的对象
     * @param index 模型索引
     * @return 对象指针
     */
    QObject* getObject(const QModelIndex& index) const;

    /**
     * @brief 查找对象的索引
     * @param object 对象指针
     * @return 模型索引
     */
    QModelIndex findObject(QObject* object) const;

    // QAbstractItemModel接口实现
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

public slots:
    /**
     * @brief 对象被销毁时的处理
     * @param object 被销毁的对象
     */
    void onObjectDestroyed(QObject* object);

    /**
     * @brief 启用或禁用自动刷新
     * @param enabled 是否启用
     */
    void setAutoRefresh(bool enabled);

private slots:
    /**
     * @brief 定时刷新
     */
    void autoRefresh();

private:
    /**
     * @brief 对象节点结构
     */
    struct ObjectNode {
        QObject* object;
        ObjectNode* parent;
        QList<ObjectNode*> children;
        int eventCount;
        double avgProcessingTime;
        
        ObjectNode(QObject* obj = nullptr, ObjectNode* par = nullptr)
            : object(obj), parent(par), eventCount(0), avgProcessingTime(0.0) {}
        
        ~ObjectNode() {
            qDeleteAll(children);
        }
    };

    /**
     * @brief 构建对象树
     * @param object 当前对象
     * @param parentNode 父节点
     * @return 创建的节点
     */
    ObjectNode* buildObjectTree(QObject* object, ObjectNode* parentNode = nullptr);

    /**
     * @brief 清理对象树
     */
    void clearObjectTree();

    /**
     * @brief 更新性能数据
     * @param node 对象节点
     */
    void updatePerformanceData(ObjectNode* node);

    /**
     * @brief 获取对象的显示名称
     * @param object 对象指针
     * @return 显示名称
     */
    QString getObjectDisplayName(QObject* object) const;

    /**
     * @brief 获取对象的类名
     * @param object 对象指针
     * @return 类名
     */
    QString getObjectClassName(QObject* object) const;

    /**
     * @brief 获取对象的内存地址字符串
     * @param object 对象指针
     * @return 地址字符串
     */
    QString getObjectAddress(QObject* object) const;

    /**
     * @brief 查找节点
     * @param object 对象指针
     * @param node 搜索起始节点
     * @return 找到的节点
     */
    ObjectNode* findNode(QObject* object, ObjectNode* node = nullptr) const;

    /**
     * @brief 获取节点的行号
     * @param node 节点指针
     * @return 行号
     */
    int getNodeRow(ObjectNode* node) const;

    // 数据成员
    QObject* m_rootObject;
    ObjectNode* m_rootNode;
    QHash<QObject*, ObjectNode*> m_objectToNode;
    
    // 自动刷新
    QTimer* m_refreshTimer;
    bool m_autoRefreshEnabled;
    
    // 线程安全
    mutable QMutex m_dataMutex;
};

#endif // OBJECT_HIERARCHY_MODEL_H