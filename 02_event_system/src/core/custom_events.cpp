#include "custom_events.h"
#include <QDateTime>
#include <QDebug>
#include <QIODevice>

// BaseCustomEvent 实现
BaseCustomEvent::BaseCustomEvent(Type type)
    : QEvent(type)
    , m_timestamp(QDateTime::currentMSecsSinceEpoch())
{
}

// DataEvent 实现
DataEvent::DataEvent(const QVariant& data)
    : BaseCustomEvent(static_cast<Type>(DataEventType))
    , m_data(data)
{
}

QVariant DataEvent::data() const
{
    return m_data;
}

void DataEvent::setData(const QVariant& data)
{
    m_data = data;
}

QByteArray DataEvent::serialize() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    
    // 写入时间戳
    stream << m_timestamp;
    
    // 写入数据
    stream << m_data;
    
    return result;
}

bool DataEvent::deserialize(const QByteArray& data)
{
    QDataStream stream(data);
    
    try {
        // 读取时间戳
        stream >> m_timestamp;
        
        // 读取数据
        stream >> m_data;
        
        return stream.status() == QDataStream::Ok;
    } catch (...) {
        return false;
    }
}

QString DataEvent::description() const
{
    return QString("DataEvent: type=%1, size=%2 bytes")
           .arg(dataTypeName())
           .arg(serialize().size());
}

bool DataEvent::isValid() const
{
    return m_data.isValid();
}

QString DataEvent::dataTypeName() const
{
    return QString(m_data.typeName());
}

// CommandEvent 实现
CommandEvent::CommandEvent(const QString& command, const QVariantMap& params)
    : BaseCustomEvent(static_cast<Type>(CommandEventType))
    , m_command(command)
    , m_parameters(params)
{
}

QVariant CommandEvent::data() const
{
    QVariantMap result;
    result["command"] = m_command;
    result["parameters"] = m_parameters;
    return result;
}

void CommandEvent::setData(const QVariant& data)
{
    if (data.canConvert<QVariantMap>()) {
        QVariantMap map = data.toMap();
        m_command = map.value("command").toString();
        m_parameters = map.value("parameters").toMap();
    }
}

QByteArray CommandEvent::serialize() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    
    // 写入时间戳
    stream << m_timestamp;
    
    // 写入命令
    stream << m_command;
    
    // 写入参数
    stream << m_parameters;
    
    return result;
}

bool CommandEvent::deserialize(const QByteArray& data)
{
    QDataStream stream(data);
    
    try {
        // 读取时间戳
        stream >> m_timestamp;
        
        // 读取命令
        stream >> m_command;
        
        // 读取参数
        stream >> m_parameters;
        
        return stream.status() == QDataStream::Ok;
    } catch (...) {
        return false;
    }
}

QString CommandEvent::description() const
{
    return QString("CommandEvent: command='%1', params=%2")
           .arg(m_command)
           .arg(m_parameters.size());
}

QString CommandEvent::command() const
{
    return m_command;
}

void CommandEvent::setCommand(const QString& command)
{
    m_command = command;
}

QVariantMap CommandEvent::parameters() const
{
    return m_parameters;
}

void CommandEvent::setParameters(const QVariantMap& params)
{
    m_parameters = params;
}

void CommandEvent::setParameter(const QString& key, const QVariant& value)
{
    m_parameters[key] = value;
}

QVariant CommandEvent::parameter(const QString& key, const QVariant& defaultValue) const
{
    return m_parameters.value(key, defaultValue);
}

bool CommandEvent::hasParameter(const QString& key) const
{
    return m_parameters.contains(key);
}

void CommandEvent::removeParameter(const QString& key)
{
    m_parameters.remove(key);
}

bool CommandEvent::isValid() const
{
    return !m_command.isEmpty();
}