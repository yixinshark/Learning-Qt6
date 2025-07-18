#include "object_hierarchy_model.h"
#include "event_logger.h"
#include <QDebug>
#include <QMutexLocker>
#include <QApplication>

ObjectHierarchyModel::ObjectHierarchyModel(QObject* parent)
    : QAbstractItemModel(parent)
    , m_rootObject(nullptr)
    , m_rootNode(nullptr)
    , m_refreshTimer(nullptr)
    , m_autoRefreshEnabled(true)
{
    // 设置自动刷新定时器
    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, &QTimer::timeout, this, &ObjectHierarchyModel::autoRefresh);
    
    // 延迟设置根对象和启动自动刷新，等待应用程序完全初始化
    QTimer::singleShot(1000, this, [this]() {
        try {
            if (QApplication::instance()) {
                qDebug() << "Setting root object to QApplication instance";
                setRootObject(QApplication::instance());
                
                // 启用自动刷新，但使用更长的间隔减少性能影响
                m_refreshTimer->setInterval(5000); // 5秒刷新一次
                m_refreshTimer->start();
                qDebug() << "Object hierarchy auto-refresh enabled with 5s interval";
            }
        } catch (...) {
            qDebug() << "Failed to set root object or start auto-refresh";
        }
    });
}

void ObjectHierarchyModel::setRootObject(QObject* root)
{
    QMutexLocker locker(&m_dataMutex);
    
    beginResetModel();
    
    m_rootObject = root;
    clearObjectTree();
    
    if (m_rootObject) {
        m_rootNode = buildObjectTree(m_rootObject);
    }
    
    endResetModel();
    
    qDebug() << "ObjectHierarchyModel root object set to:" << (root ? root->objectName() : "nullptr");
}

QObject* ObjectHierarchyModel::getRootObject() const
{
    QMutexLocker locker(&m_dataMutex);
    return m_rootObject;
}

void ObjectHierarchyModel::refresh()
{
    QMutexLocker locker(&m_dataMutex);
    
    if (!m_rootObject) {
        return;
    }
    
    beginResetModel();
    clearObjectTree();
    m_rootNode = buildObjectTree(m_rootObject);
    endResetModel();
}

QObject* ObjectHierarchyModel::getObject(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return nullptr;
    }
    
    ObjectNode* node = static_cast<ObjectNode*>(index.internalPointer());
    return node ? node->object : nullptr;
}

QModelIndex ObjectHierarchyModel::findObject(QObject* object) const
{
    QMutexLocker locker(&m_dataMutex);
    
    ObjectNode* node = findNode(object);
    if (!node || !node->parent) {
        return QModelIndex();
    }
    
    int row = getNodeRow(node);
    return createIndex(row, 0, node);
}

QModelIndex ObjectHierarchyModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    
    QMutexLocker locker(&m_dataMutex);
    
    ObjectNode* parentNode;
    if (!parent.isValid()) {
        parentNode = m_rootNode;
    } else {
        parentNode = static_cast<ObjectNode*>(parent.internalPointer());
    }
    
    if (!parentNode || row >= parentNode->children.size()) {
        return QModelIndex();
    }
    
    ObjectNode* childNode = parentNode->children.at(row);
    return createIndex(row, column, childNode);
}

QModelIndex ObjectHierarchyModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }
    
    QMutexLocker locker(&m_dataMutex);
    
    ObjectNode* childNode = static_cast<ObjectNode*>(child.internalPointer());
    if (!childNode || !childNode->parent || childNode->parent == m_rootNode) {
        return QModelIndex();
    }
    
    ObjectNode* parentNode = childNode->parent;
    int row = getNodeRow(parentNode);
    return createIndex(row, 0, parentNode);
}

int ObjectHierarchyModel::rowCount(const QModelIndex& parent) const
{
    QMutexLocker locker(&m_dataMutex);
    
    ObjectNode* parentNode;
    if (!parent.isValid()) {
        parentNode = m_rootNode;
    } else {
        parentNode = static_cast<ObjectNode*>(parent.internalPointer());
    }
    
    return parentNode ? parentNode->children.size() : 0;
}

int ObjectHierarchyModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant ObjectHierarchyModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    
    QMutexLocker locker(&m_dataMutex);
    
    ObjectNode* node = static_cast<ObjectNode*>(index.internalPointer());
    if (!node) {
        return QVariant();
    }
    
    // 额外的安全检查：确保对象仍然有效
    if (!node || !node->object) {
        return QVariant();
    }
    
    // 检查对象是否已被销毁
    if (!m_objectToNode.contains(node->object)) {
        return QVariant();
    }
    
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case NameColumn:
            return getObjectDisplayName(node->object);
        case ClassColumn:
            return getObjectClassName(node->object);
        case AddressColumn:
            return getObjectAddress(node->object);
        case ChildCountColumn:
            return node->children.size();
        case EventCountColumn:
            return node->eventCount;
        case AvgTimeColumn:
            return QString::number(node->avgProcessingTime, 'f', 2);
        default:
            return QVariant();
        }
    } else if (role == Qt::ToolTipRole) {
        return QString("对象: %1\n类型: %2\n地址: %3\n子对象: %4\n事件数: %5\n平均时间: %6ms")
               .arg(getObjectDisplayName(node->object))
               .arg(getObjectClassName(node->object))
               .arg(getObjectAddress(node->object))
               .arg(node->children.size())
               .arg(node->eventCount)
               .arg(node->avgProcessingTime, 0, 'f', 2);
    }
    
    return QVariant();
}

QVariant ObjectHierarchyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QVariant();
    }
    
    switch (section) {
    case NameColumn:
        return "对象名称";
    case ClassColumn:
        return "类型";
    case AddressColumn:
        return "地址";
    case ChildCountColumn:
        return "子对象数";
    case EventCountColumn:
        return "事件数";
    case AvgTimeColumn:
        return "平均时间(ms)";
    default:
        return QVariant();
    }
}

Qt::ItemFlags ObjectHierarchyModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void ObjectHierarchyModel::onObjectDestroyed(QObject* object)
{
    QMutexLocker locker(&m_dataMutex);
    
    if (m_objectToNode.contains(object)) {
        m_objectToNode.remove(object);
        // 在实际应用中，这里应该更新模型结构
        // 为了简化，我们在下次刷新时会自动处理
    }
}

void ObjectHierarchyModel::setAutoRefresh(bool enabled)
{
    m_autoRefreshEnabled = enabled;
    
    if (enabled) {
        m_refreshTimer->start(2000);
    } else {
        m_refreshTimer->stop();
    }
    
    qDebug() << "ObjectHierarchyModel auto refresh" << (enabled ? "enabled" : "disabled");
}

void ObjectHierarchyModel::autoRefresh()
{
    if (m_autoRefreshEnabled) {
        refresh();
    }
}

ObjectHierarchyModel::ObjectNode* ObjectHierarchyModel::buildObjectTree(QObject* object, ObjectNode* parentNode)
{
    if (!object) {
        return nullptr;
    }
    
    ObjectNode* node = new ObjectNode(object, parentNode);
    m_objectToNode[object] = node;
    
    // 更新性能数据
    updatePerformanceData(node);
    
    // 连接对象销毁信号
    connect(object, &QObject::destroyed, this, &ObjectHierarchyModel::onObjectDestroyed, Qt::UniqueConnection);
    
    // 递归构建子对象
    const QObjectList& children = object->children();
    for (QObject* child : children) {
        if (child) {
            ObjectNode* childNode = buildObjectTree(child, node);
            if (childNode) {
                node->children.append(childNode);
            }
        }
    }
    
    return node;
}

void ObjectHierarchyModel::clearObjectTree()
{
    if (m_rootNode) {
        delete m_rootNode;
        m_rootNode = nullptr;
    }
    m_objectToNode.clear();
}

void ObjectHierarchyModel::updatePerformanceData(ObjectNode* node)
{
    if (!node || !node->object) {
        return;
    }
    
    // 从EventLogger获取性能数据
    EventLogger* logger = EventLogger::instance();
    double avgTime = logger->getAverageProcessingTime(node->object);
    
    if (avgTime >= 0) {
        node->avgProcessingTime = avgTime;
        // 这里可以添加更多的性能指标计算
        node->eventCount = 10; // 简化的事件计数，实际应该从logger获取
    }
}

QString ObjectHierarchyModel::getObjectDisplayName(QObject* object) const
{
    if (!object) {
        return "nullptr";
    }
    
    // 检查对象是否仍然有效
    try {
        QString name = object->objectName();
        if (name.isEmpty()) {
            // 安全地获取类名
            const QMetaObject* metaObj = object->metaObject();
            if (metaObj) {
                name = QString("<%1>").arg(metaObj->className());
            } else {
                name = "<Unknown>";
            }
        }
        return name;
    } catch (...) {
        return "<Invalid Object>";
    }
}

QString ObjectHierarchyModel::getObjectClassName(QObject* object) const
{
    if (!object) {
        return QString();
    }
    
    try {
        const QMetaObject* metaObj = object->metaObject();
        if (metaObj) {
            return metaObj->className();
        } else {
            return "<Unknown>";
        }
    } catch (...) {
        return "<Invalid>";
    }
}

QString ObjectHierarchyModel::getObjectAddress(QObject* object) const
{
    if (!object) {
        return QString();
    }
    
    return QString("0x%1").arg(reinterpret_cast<quintptr>(object), 0, 16);
}

ObjectHierarchyModel::ObjectNode* ObjectHierarchyModel::findNode(QObject* object, ObjectNode* node) const
{
    Q_UNUSED(node)  // 参数暂未使用，但保留以备将来扩展
    
    if (!object) {
        return nullptr;
    }
    
    if (m_objectToNode.contains(object)) {
        return m_objectToNode[object];
    }
    
    return nullptr;
}

int ObjectHierarchyModel::getNodeRow(ObjectNode* node) const
{
    if (!node || !node->parent) {
        return 0;
    }
    
    return node->parent->children.indexOf(node);
}