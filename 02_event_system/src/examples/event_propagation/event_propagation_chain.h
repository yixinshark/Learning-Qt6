#ifndef EVENT_PROPAGATION_CHAIN_H
#define EVENT_PROPAGATION_CHAIN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QGroupBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSplitter>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QDateTime>
#include <QColor>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QGraphicsDropShadowEffect>

// Forward declarations
class ChainWidget;

/**
 * @brief 事件传播链演示类
 * 
 * 这个类创建一个多层级的控件层次结构，用于演示事件在复杂对象树中的传播过程。
 * 包括可视化的传播路径显示和详细的传播分析。
 */
class EventPropagationChain : public QWidget
{
    Q_OBJECT

public:
    explicit EventPropagationChain(QWidget *parent = nullptr);
    ~EventPropagationChain() = default;

private slots:
    void onResetDemo();
    void onToggleVisualization();
    void onClearLog();

private:
    void setupUI();
    void createHierarchy();
    void updateHierarchyTree();
    void highlightPropagationPath(const QStringList &path);
    void logPropagationStep(const QString &step, const QString &details);
    
    // UI组件
    QHBoxLayout *m_mainLayout;
    QSplitter *m_splitter;
    
    // 左侧层次结构显示
    QGroupBox *m_hierarchyGroup;
    QVBoxLayout *m_hierarchyLayout;
    QTreeWidget *m_hierarchyTree;
    QPushButton *m_resetButton;
    QPushButton *m_visualizeButton;
    
    // 中间交互区域
    QGroupBox *m_interactionGroup;
    QVBoxLayout *m_interactionLayout;
    
    // 右侧日志区域
    QGroupBox *m_logGroup;
    QVBoxLayout *m_logLayout;
    QTextEdit *m_logDisplay;
    QPushButton *m_clearLogButton;
    
    // 控件层次结构
    QWidget *m_rootWidget;
    QList<ChainWidget*> m_chainWidgets;
    
    // 可视化控制
    bool m_visualizationEnabled;
    QTimer *m_highlightTimer;
    QPropertyAnimation *m_highlightAnimation;
};

/**
 * @brief 链式传播控件类
 * 
 * 用于构建事件传播链的自定义控件，每个控件都能记录和报告事件传播信息。
 */
class ChainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChainWidget(const QString &name, int level, QWidget *parent = nullptr);
    
    void setAcceptEvents(bool accept) { m_acceptEvents = accept; }
    bool acceptsEvents() const { return m_acceptEvents; }
    
    const QString& widgetName() const { return m_name; }
    int hierarchyLevel() const { return m_level; }
    
    void highlightWidget(bool highlight);
    void setHighlightColor(const QColor &color);

signals:
    void eventPropagated(const QString &widgetName, int level, 
                        const QString &eventType, const QString &details, 
                        bool accepted, bool willPropagate);

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QString getEventDescription(QEvent *event);
    void animateHighlight();
    
    QString m_name;
    int m_level;
    bool m_acceptEvents;
    bool m_isHighlighted;
    QColor m_baseColor;
    QColor m_highlightColor;
    QColor m_currentColor;
    
    QGraphicsDropShadowEffect *m_shadowEffect;
    QPropertyAnimation *m_colorAnimation;
};



#endif // EVENT_PROPAGATION_CHAIN_H