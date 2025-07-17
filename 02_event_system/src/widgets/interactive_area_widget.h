#ifndef INTERACTIVE_AREA_WIDGET_H
#define INTERACTIVE_AREA_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QProgressBar>
#include <QGroupBox>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QTimer>

/**
 * @brief 交互区域控件 - 提供用户交互的测试区域
 * 
 * 这个控件提供各种交互元素，用于：
 * - 测试不同类型的事件
 * - 演示事件的产生和处理
 * - 提供可视化的交互反馈
 * - 支持自定义事件的触发
 */
class InteractiveAreaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InteractiveAreaWidget(QWidget* parent = nullptr);
    ~InteractiveAreaWidget() override = default;

    void setCurrentExample(const QString& exampleName);

public slots:
    void triggerCustomEvent();
    void simulateMouseEvent();
    void simulateKeyEvent();
    void startEventStorm();
    void stopEventStorm();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private slots:
    void onButtonClicked();
    void onSliderValueChanged(int value);
    void onTextChanged();
    void onComboBoxChanged();
    void onCheckBoxToggled(bool checked);
    void generateEventStorm();

private:
    void setupUI();
    void setupBasicControls();
    void setupEventTestArea();
    void setupEventGenerators();
    void updateInteractionInfo(const QString& info);
    void logInteractionEvent(const QString& eventType, const QString& details);
    
    // UI组件
    QVBoxLayout* m_mainLayout;
    QGroupBox* m_basicControlsGroup;
    QGroupBox* m_eventTestGroup;
    QGroupBox* m_eventGeneratorGroup;
    
    // 基础控件
    QPushButton* m_testButton;
    QLineEdit* m_testLineEdit;
    QTextEdit* m_testTextEdit;
    QSlider* m_testSlider;
    QSpinBox* m_testSpinBox;
    QCheckBox* m_testCheckBox;
    QComboBox* m_testComboBox;
    QProgressBar* m_testProgressBar;
    
    // 事件测试区域
    QLabel* m_interactionLabel;
    QLabel* m_mousePositionLabel;
    QLabel* m_keyInfoLabel;
    QTextEdit* m_eventLogText;
    
    // 事件生成器
    QPushButton* m_customEventButton;
    QPushButton* m_mouseEventButton;
    QPushButton* m_keyEventButton;
    QPushButton* m_eventStormButton;
    QPushButton* m_stopStormButton;
    QSpinBox* m_stormIntervalSpinBox;
    
    // 状态信息
    QString m_currentExample;
    QPoint m_lastMousePos;
    bool m_mousePressed;
    QTimer* m_eventStormTimer;
    int m_eventStormCount;
    
    // 绘制相关
    QList<QPoint> m_mouseTrail;
    QColor m_backgroundColor;
};

#endif // INTERACTIVE_AREA_WIDGET_H