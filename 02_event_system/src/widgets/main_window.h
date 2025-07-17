#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QStatusBar>
#include <QLabel>
#include <QTimer>
#include <QSplitter>

class EventDisplayWidget;
class InteractiveAreaWidget;
class DebugPanelWidget;

/**
 * @brief 主窗口类 - 提供标签页式的示例切换界面
 * 
 * 这个类实现了Qt事件系统学习项目的主界面，包括：
 * - 标签页式的示例切换
 * - 事件显示区域
 * - 交互测试区域
 * - 调试面板
 * - 状态栏信息显示
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onTabChanged(int index);
    void updateStatusBar();
    void onEventCountChanged(int count);
    void onPerformanceUpdate(double avgTime, int eventsPerSecond);

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupCentralWidget();
    void loadExampleModules();
    
    // UI组件
    QTabWidget* m_tabWidget;
    QSplitter* m_mainSplitter;
    QSplitter* m_rightSplitter;
    
    // 自定义控件
    EventDisplayWidget* m_eventDisplay;
    InteractiveAreaWidget* m_interactiveArea;
    DebugPanelWidget* m_debugPanel;
    
    // 状态栏组件
    QLabel* m_currentExampleLabel;
    QLabel* m_eventCountLabel;
    QLabel* m_performanceLabel;
    QTimer* m_statusUpdateTimer;
    
    // 示例模块
    QWidget* m_basicEventsExample;
    QWidget* m_mouseKeyboardExample;
    QWidget* m_customEventsExample;
    QWidget* m_eventFiltersExample;
    QWidget* m_eventPropagationExample;
    QWidget* m_advancedPatternsExample;
};

#endif // MAIN_WINDOW_H