#include "main_window.h"
#include "event_display_widget.h"
#include "interactive_area_widget.h"
#include "debug_panel_widget.h"
#include "performance_monitor_widget.h"

#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>

// 包含示例模块
#include "../examples/basic_events/basic_event_widget.h"
#include "../examples/mouse_keyboard/interaction_demo.h"
#include "../examples/custom_events/custom_event_demo.h"
#include "../examples/event_filters/global_event_filter.h"
#include "../examples/event_propagation/parent_child_event_demo.h"
#include "../examples/advanced_patterns/advanced_patterns_demo.h"

#include "../core/event_logger.h"
#include "../core/event_manager.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_tabWidget(nullptr)
    , m_mainSplitter(nullptr)
    , m_rightSplitter(nullptr)
    , m_eventDisplay(nullptr)
    , m_interactiveArea(nullptr)
    , m_debugPanel(nullptr)
    , m_currentExampleLabel(nullptr)
    , m_eventCountLabel(nullptr)
    , m_performanceLabel(nullptr)
    , m_statusUpdateTimer(nullptr)
{
    setWindowTitle("Qt6 事件系统学习演示");
    setMinimumSize(1400, 900);
    resize(1600, 1000);
    
    setupMenuBar();
    setupStatusBar();
    setupUI();
    
    // 设置状态更新定时器
    m_statusUpdateTimer = new QTimer(this);
    connect(m_statusUpdateTimer, &QTimer::timeout, this, &MainWindow::updateStatusBar);
    m_statusUpdateTimer->start(1000); // 每秒更新一次
    
    // 连接事件日志信号
    EventLogger* logger = EventLogger::instance();
    connect(logger, &EventLogger::eventCountChanged, this, &MainWindow::onEventCountChanged);
    connect(logger, &EventLogger::performanceUpdate, this, &MainWindow::onPerformanceUpdate);
    
    qDebug() << "MainWindow initialized successfully";
}

void MainWindow::setupUI()
{
    setupCentralWidget();
    loadExampleModules();
}

void MainWindow::setupMenuBar()
{
    // 文件菜单
    QMenu* fileMenu = menuBar()->addMenu("文件(&F)");
    
    QAction* exitAction = fileMenu->addAction("退出(&X)");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // 视图菜单
    QMenu* viewMenu = menuBar()->addMenu("视图(&V)");
    
    QAction* resetLayoutAction = viewMenu->addAction("重置布局(&R)");
    connect(resetLayoutAction, &QAction::triggered, [this]() {
        // 重置分割器比例
        m_mainSplitter->setSizes({800, 600});
        m_rightSplitter->setSizes({400, 200});
    });
    
    viewMenu->addSeparator();
    
    QAction* clearLogAction = viewMenu->addAction("清除日志(&C)");
    connect(clearLogAction, &QAction::triggered, [this]() {
        EventLogger::instance()->clearHistory();
    });
    
    // 帮助菜单
    QMenu* helpMenu = menuBar()->addMenu("帮助(&H)");
    
    QAction* aboutAction = helpMenu->addAction("关于(&A)");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "关于", 
            "Qt6 事件系统学习演示\n\n"
            "这是一个用于学习Qt事件系统的综合演示程序。\n"
            "包含了从基础事件处理到高级事件模式的完整示例。");
    });
}

void MainWindow::setupStatusBar()
{
    // 当前示例标签
    m_currentExampleLabel = new QLabel("当前示例: 基础事件处理");
    m_currentExampleLabel->setMinimumWidth(200);
    statusBar()->addWidget(m_currentExampleLabel);
    
    statusBar()->addPermanentWidget(new QLabel("|"));
    
    // 事件计数标签
    m_eventCountLabel = new QLabel("事件计数: 0");
    m_eventCountLabel->setMinimumWidth(100);
    statusBar()->addPermanentWidget(m_eventCountLabel);
    
    statusBar()->addPermanentWidget(new QLabel("|"));
    
    // 性能信息标签
    m_performanceLabel = new QLabel("性能: 0 事件/秒");
    m_performanceLabel->setMinimumWidth(120);
    statusBar()->addPermanentWidget(m_performanceLabel);
}

void MainWindow::setupCentralWidget()
{
    qDebug() << "Setting up central widget...";
    
    // 尝试使用分割器布局，但使用安全的初始化方式
    try {
        // 创建主分割器（水平分割）
        m_mainSplitter = new QSplitter(Qt::Horizontal, this);
        setCentralWidget(m_mainSplitter);
        
        // 创建标签页控件
        m_tabWidget = new QTabWidget();
        connect(m_tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
        
        // 创建右侧分割器（垂直分割）
        m_rightSplitter = new QSplitter(Qt::Vertical);
        
        // 尝试创建自定义控件
        qDebug() << "Creating EventDisplayWidget...";
        m_eventDisplay = new EventDisplayWidget();
        qDebug() << "EventDisplayWidget created successfully";
        
        qDebug() << "Creating InteractiveAreaWidget...";
        m_interactiveArea = new InteractiveAreaWidget();
        qDebug() << "InteractiveAreaWidget created successfully";
        
        // 尝试创建 DebugPanelWidget
        qDebug() << "Creating DebugPanelWidget...";
        try {
            m_debugPanel = new DebugPanelWidget();
            qDebug() << "DebugPanelWidget created successfully";
            
            // 添加到右侧分割器
            m_rightSplitter->addWidget(m_eventDisplay);
            m_rightSplitter->addWidget(m_debugPanel);
        } catch (...) {
            qDebug() << "Failed to create DebugPanelWidget, using placeholder";
            
            // 添加到右侧分割器
            m_rightSplitter->addWidget(m_eventDisplay);
            
            // 添加一个简单的占位符替代调试面板
            QWidget* debugPlaceholder = new QWidget();
            QVBoxLayout* debugLayout = new QVBoxLayout(debugPlaceholder);
            debugLayout->addWidget(new QLabel("调试面板"));
            debugLayout->addWidget(new QLabel("(组件初始化失败，使用占位符)"));
            debugLayout->addStretch();
            m_rightSplitter->addWidget(debugPlaceholder);
        }
        
        // 添加到主分割器
        m_mainSplitter->addWidget(m_tabWidget);
        m_mainSplitter->addWidget(m_rightSplitter);
        
        // 设置分割器比例
        m_mainSplitter->setSizes({800, 600});
        m_rightSplitter->setSizes({400, 200});
        
        // 设置分割器属性
        m_mainSplitter->setChildrenCollapsible(false);
        m_rightSplitter->setChildrenCollapsible(false);
        
        qDebug() << "Splitter layout created successfully";
        
    } catch (...) {
        qDebug() << "Failed to create splitter layout, falling back to simple layout";
        
        // 如果分割器创建失败，回退到简单布局
        m_tabWidget = new QTabWidget();
        connect(m_tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
        setCentralWidget(m_tabWidget);
    }
    
    qDebug() << "Central widget setup completed";
    
    /*
    // 暂时注释掉复杂的分割器布局
    // 创建主分割器（水平分割）
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(m_mainSplitter);
    
    // 创建标签页控件
    m_tabWidget = new QTabWidget();
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    
    // 创建右侧分割器（垂直分割）
    m_rightSplitter = new QSplitter(Qt::Vertical);
    
    // 创建自定义控件
    m_eventDisplay = new EventDisplayWidget();
    m_interactiveArea = new InteractiveAreaWidget();
    m_debugPanel = new DebugPanelWidget();
    
    // 添加到右侧分割器
    m_rightSplitter->addWidget(m_eventDisplay);
    m_rightSplitter->addWidget(m_debugPanel);
    
    // 添加到主分割器
    m_mainSplitter->addWidget(m_tabWidget);
    m_mainSplitter->addWidget(m_rightSplitter);
    
    // 设置分割器比例
    m_mainSplitter->setSizes({800, 600});
    m_rightSplitter->setSizes({400, 200});
    
    // 设置分割器属性
    m_mainSplitter->setChildrenCollapsible(false);
    m_rightSplitter->setChildrenCollapsible(false);
    */
}

void MainWindow::loadExampleModules()
{
    qDebug() << "Loading example modules...";
    
    try {
        // 只添加最简单的占位符标签页，避免复杂组件初始化问题
        qDebug() << "Creating simple placeholder tabs...";
        
        // 基础事件处理示例 - 尝试使用真实的组件
        qDebug() << "Creating BasicEventWidget...";
        try {
            m_basicEventsExample = new BasicEventWidget();
            m_tabWidget->addTab(m_basicEventsExample, "基础事件");
            qDebug() << "BasicEventWidget created successfully";
        } catch (...) {
            qDebug() << "Failed to create BasicEventWidget, using placeholder";
            QWidget* basicPlaceholder = new QWidget();
            QVBoxLayout* basicLayout = new QVBoxLayout(basicPlaceholder);
            basicLayout->addWidget(new QLabel("基础事件处理演示"));
            basicLayout->addWidget(new QLabel("(组件初始化失败，使用占位符)"));
            basicLayout->addStretch();
            m_tabWidget->addTab(basicPlaceholder, "基础事件");
        }
        
        // 鼠标键盘事件示例
        qDebug() << "Creating InteractionDemo...";
        try {
            m_mouseKeyboardExample = new InteractionDemo();
            m_tabWidget->addTab(m_mouseKeyboardExample, "鼠标键盘");
            qDebug() << "InteractionDemo created successfully";
        } catch (...) {
            qDebug() << "Failed to create InteractionDemo, using placeholder";
            QWidget* mousePlaceholder = new QWidget();
            QVBoxLayout* mouseLayout = new QVBoxLayout(mousePlaceholder);
            mouseLayout->addWidget(new QLabel("鼠标键盘事件演示"));
            mouseLayout->addWidget(new QLabel("(组件初始化失败，使用占位符)"));
            mouseLayout->addStretch();
            m_tabWidget->addTab(mousePlaceholder, "鼠标键盘");
        }
        
        // 自定义事件示例
        qDebug() << "Creating CustomEventDemo...";
        try {
            m_customEventsExample = new CustomEventDemo();
            m_tabWidget->addTab(m_customEventsExample, "自定义事件");
            qDebug() << "CustomEventDemo created successfully";
        } catch (...) {
            qDebug() << "Failed to create CustomEventDemo, using placeholder";
            QWidget* customPlaceholder = new QWidget();
            QVBoxLayout* customLayout = new QVBoxLayout(customPlaceholder);
            customLayout->addWidget(new QLabel("自定义事件演示"));
            customLayout->addWidget(new QLabel("(组件初始化失败，使用占位符)"));
            customLayout->addStretch();
            m_tabWidget->addTab(customPlaceholder, "自定义事件");
        }
        
        // 事件过滤器示例 - 使用更完整的实现
        qDebug() << "Creating event filter example...";
        m_eventFiltersExample = new QWidget();
        QVBoxLayout* filterLayout = new QVBoxLayout(m_eventFiltersExample);
        filterLayout->addWidget(new QLabel("事件过滤器演示"));
        
        // 创建一个简单的事件过滤器演示控件
        QWidget* filterDemoWidget = new QWidget();
        filterDemoWidget->setMinimumHeight(200);
        filterDemoWidget->setStyleSheet("background-color: #f0f0f0; border: 1px solid #ccc;");
        QVBoxLayout* demoLayout = new QVBoxLayout(filterDemoWidget);
        demoLayout->addWidget(new QLabel("事件过滤器将在后台运行"));
        demoLayout->addWidget(new QLabel("查看右侧面板以观察过滤的事件"));
        
        filterLayout->addWidget(filterDemoWidget);
        m_tabWidget->addTab(m_eventFiltersExample, "事件过滤");
        qDebug() << "Event filter example created successfully";
        
        // 创建全局事件过滤器
        qDebug() << "Creating GlobalEventFilter...";
        try {
            GlobalEventFilter* globalFilter = new GlobalEventFilter(this);
            globalFilter->setEnabled(true);
            QApplication::instance()->installEventFilter(globalFilter);
            qDebug() << "GlobalEventFilter created and installed successfully";
        } catch (...) {
            qDebug() << "Failed to create GlobalEventFilter";
        }
        
        // 事件传播示例
        qDebug() << "Creating ParentChildEventDemo...";
        try {
            m_eventPropagationExample = new ParentChildEventDemo();
            m_tabWidget->addTab(m_eventPropagationExample, "事件传播");
            qDebug() << "ParentChildEventDemo created successfully";
        } catch (...) {
            qDebug() << "Failed to create ParentChildEventDemo, using placeholder";
            QWidget* propagationPlaceholder = new QWidget();
            QVBoxLayout* propagationLayout = new QVBoxLayout(propagationPlaceholder);
            propagationLayout->addWidget(new QLabel("事件传播演示"));
            propagationLayout->addWidget(new QLabel("(组件初始化失败，使用占位符)"));
            propagationLayout->addStretch();
            m_tabWidget->addTab(propagationPlaceholder, "事件传播");
        }
        
        // 高级模式示例
        qDebug() << "Creating AdvancedPatternsDemo...";
        try {
            m_advancedPatternsExample = new AdvancedPatternsDemo();
            m_tabWidget->addTab(m_advancedPatternsExample, "高级模式");
            qDebug() << "AdvancedPatternsDemo created successfully";
        } catch (...) {
            qDebug() << "Failed to create AdvancedPatternsDemo, using placeholder";
            QWidget* advancedPlaceholder = new QWidget();
            QVBoxLayout* advancedLayout = new QVBoxLayout(advancedPlaceholder);
            advancedLayout->addWidget(new QLabel("高级模式演示"));
            advancedLayout->addWidget(new QLabel("(组件初始化失败，使用占位符)"));
            advancedLayout->addStretch();
            m_tabWidget->addTab(advancedPlaceholder, "高级模式");
        }
        
        // 性能监控面板 - 尝试使用真实的组件
        qDebug() << "Creating PerformanceMonitorWidget...";
        try {
            PerformanceMonitorWidget* performanceMonitor = new PerformanceMonitorWidget();
            m_tabWidget->addTab(performanceMonitor, "性能监控");
            qDebug() << "PerformanceMonitorWidget created successfully";
        } catch (...) {
            qDebug() << "Failed to create PerformanceMonitorWidget, using placeholder";
            QWidget* performancePlaceholder = new QWidget();
            QVBoxLayout* placeholderLayout = new QVBoxLayout(performancePlaceholder);
            placeholderLayout->addWidget(new QLabel("性能监控功能"));
            placeholderLayout->addWidget(new QLabel("(组件初始化失败，使用占位符)"));
            placeholderLayout->addStretch();
            m_tabWidget->addTab(performancePlaceholder, "性能监控");
        }
        
    } catch (const std::exception& e) {
        qDebug() << "Exception in loadExampleModules:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception in loadExampleModules";
    }
    
    qDebug() << "Example modules loading completed";
}

void MainWindow::onTabChanged(int index)
{
    QString tabName = m_tabWidget->tabText(index);
    m_currentExampleLabel->setText(QString("当前示例: %1").arg(tabName));
    
    // 通知交互区域当前示例变化
    if (m_interactiveArea) {
        m_interactiveArea->setCurrentExample(tabName);
    }
    
    qDebug() << "Switched to example:" << tabName;
}

void MainWindow::updateStatusBar()
{
    // 这里可以添加定期更新的状态信息
    // 例如内存使用情况、CPU使用率等
}

void MainWindow::onEventCountChanged(int count)
{
    m_eventCountLabel->setText(QString("事件计数: %1").arg(count));
}

void MainWindow::onPerformanceUpdate(double avgTime, int eventsPerSecond)
{
    m_performanceLabel->setText(QString("性能: %1 事件/秒 (平均 %2ms)")
                               .arg(eventsPerSecond)
                               .arg(avgTime, 0, 'f', 2));
}