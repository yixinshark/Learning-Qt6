#ifndef EVENT_COMPRESSION_DEMO_H
#define EVENT_COMPRESSION_DEMO_H

#include <QCheckBox>
#include <QDateTime>
#include <QElapsedTimer>
#include <QEvent>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QProgressBar>
#include <QPushButton>
#include <QQueue>
#include <QSpinBox>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

/**
 * EventCompressionDemo - 演示事件压缩和合并技术
 *
 * 这个组件展示了如何通过事件压缩来优化性能：
 * 1. 鼠标移动事件压缩 - 只保留最新的位置信息
 * 2. 重绘事件合并 - 将多个重绘请求合并为一次
 * 3. 数据更新事件批处理 - 将频繁的数据更新合并处理
 * 4. 定时器事件优化 - 避免过于频繁的定时器触发
 */
class EventCompressionDemo : public QWidget {
  Q_OBJECT

public:
  explicit EventCompressionDemo(QWidget *parent = nullptr);
  ~EventCompressionDemo();

protected:
  // 重写事件处理函数来演示事件压缩
  bool event(QEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private slots:
  // 控制面板槽函数
  void onCompressionEnabledChanged(bool enabled);
  void onCompressionIntervalChanged(int interval);
  void onGenerateEventsClicked();
  void onClearLogClicked();

  // 事件处理槽函数
  void processCompressedMouseEvents();
  void processCompressedPaintEvents();
  void processBatchedDataUpdates();

private:
  // UI组件
  void setupUI();
  QVBoxLayout *m_mainLayout;
  QHBoxLayout *m_controlLayout;
  QGroupBox *m_controlGroup;
  QGroupBox *m_displayGroup;
  QGroupBox *m_logGroup;

  // 控制面板
  QCheckBox *m_compressionEnabled;
  QSpinBox *m_compressionInterval;
  QPushButton *m_generateEventsBtn;
  QPushButton *m_clearLogBtn;
  QLabel *m_statusLabel;
  QProgressBar *m_eventLoadBar;

  // 显示区域
  QLabel *m_mousePositionLabel;
  QLabel *m_paintCountLabel;
  QLabel *m_dataUpdateLabel;
  QLabel *m_compressionRatioLabel;

  // 日志区域
  QTextEdit *m_logTextEdit;

  // 事件压缩相关
  struct MouseEventData {
    QPoint position;
    Qt::MouseButtons buttons;
    QDateTime timestamp;
  };

  struct PaintEventData {
    QRect region;
    QDateTime timestamp;
  };

  struct DataUpdateEvent {
    QString key;
    QVariant value;
    QDateTime timestamp;
  };

  // 事件队列和压缩状态
  QQueue<MouseEventData> m_mouseEventQueue;
  QQueue<PaintEventData> m_paintEventQueue;
  QQueue<DataUpdateEvent> m_dataUpdateQueue;

  // 定时器用于批处理
  QTimer *m_mouseCompressionTimer;
  QTimer *m_paintCompressionTimer;
  QTimer *m_dataCompressionTimer;

  // 统计信息
  int m_totalMouseEvents;
  int m_compressedMouseEvents;
  int m_totalPaintEvents;
  int m_compressedPaintEvents;
  int m_totalDataUpdates;
  int m_compressedDataUpdates;

  QElapsedTimer m_performanceTimer;

  // 配置参数
  bool m_compressionEnabledFlag;
  int m_compressionIntervalMs;

  // 辅助方法
  void logEvent(const QString &message);
  void updateStatistics();
  void updateCompressionRatio();
  QString formatTimestamp(const QDateTime &timestamp);

  // 事件压缩算法
  void compressMouseEvents();
  void compressPaintEvents();
  void compressDataUpdates();

  // 性能测试
  void generateTestEvents();
  void measureCompressionPerformance();
};

#endif // EVENT_COMPRESSION_DEMO_H