#ifndef CUSTOM_EVENTS_H
#define CUSTOM_EVENTS_H

#include <QEvent>
#include <QVariant>
#include <QVariantMap>
#include <QString>
#include <QDataStream>
#include <QByteArray>

// 自定义事件类型枚举
enum CustomEventType {
    DataEventType = QEvent::User + 1,
    CommandEventType = QEvent::User + 2
};

/**
 * @brief 自定义事件基类
 * 
 * 定义自定义事件的通用接口，提供数据传递和序列化的标准模式
 */
class BaseCustomEvent : public QEvent
{
public:
    explicit BaseCustomEvent(Type type);
    virtual ~BaseCustomEvent() = default;
    
    // 事件数据接口
    virtual QVariant data() const = 0;
    virtual void setData(const QVariant& data) = 0;
    
    // 序列化接口
    virtual QByteArray serialize() const = 0;
    virtual bool deserialize(const QByteArray& data) = 0;
    
    // 事件描述
    virtual QString description() const = 0;
    
    // 时间戳
    qint64 timestamp() const { return m_timestamp; }
    
protected:
    qint64 m_timestamp;
};

/**
 * @brief 数据传递事件
 * 
 * 支持QVariant数据的传递和获取，适用于各种数据类型的传输
 */
class DataEvent : public BaseCustomEvent
{
public:
    explicit DataEvent(const QVariant& data = QVariant());
    
    // 实现基类接口
    QVariant data() const override;
    void setData(const QVariant& data) override;
    
    QByteArray serialize() const override;
    bool deserialize(const QByteArray& data) override;
    
    QString description() const override;
    
    // 数据类型检查
    bool isValid() const;
    QString dataTypeName() const;
    
private:
    QVariant m_data;
};

/**
 * @brief 命令事件
 * 
 * 支持命令和参数的封装传递，适用于命令模式的事件通信
 */
class CommandEvent : public BaseCustomEvent
{
public:
    explicit CommandEvent(const QString& command = QString(), 
                         const QVariantMap& params = QVariantMap());
    
    // 实现基类接口
    QVariant data() const override;
    void setData(const QVariant& data) override;
    
    QByteArray serialize() const override;
    bool deserialize(const QByteArray& data) override;
    
    QString description() const override;
    
    // 命令相关接口
    QString command() const;
    void setCommand(const QString& command);
    
    QVariantMap parameters() const;
    void setParameters(const QVariantMap& params);
    
    // 参数操作便利方法
    void setParameter(const QString& key, const QVariant& value);
    QVariant parameter(const QString& key, const QVariant& defaultValue = QVariant()) const;
    bool hasParameter(const QString& key) const;
    void removeParameter(const QString& key);
    
    // 验证
    bool isValid() const;
    
private:
    QString m_command;
    QVariantMap m_parameters;
};

#endif // CUSTOM_EVENTS_H