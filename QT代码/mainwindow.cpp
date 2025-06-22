#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QRandomGenerator>
#include <cmath>
#include <QListWidget>
#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFileDialog>
#include <QComboBox>
#include <QPainterPath>
#include <QMessageBox>
#include <QTextBlock>
#include <QTimer>
#include <QScrollBar>
#include <QDateTime>
#define MAX_NODES 3




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,currentNodeIndex(-1)
    , lastNodeIndex(-1)
{
    ui->setupUi(this);
    QAction *searchAction = new QAction(this);
    searchAction->setIcon(QIcon(":/icon/搜索_search.png"));  // 图标路径
    ui->lineEditSearch->addAction(searchAction, QLineEdit::LeadingPosition);  // Leading表示左侧
    ui->lbHigh->setMouseTracking(true);


    pointTimes.resize(10);
    initialTime = QTime::currentTime();

    // 初始化每个点的时间
    for (int i = 0; i < 10; ++i) {
        pointTimes[i] = initialTime.addSecs(i * timeInterval);
    }

    // 创建并启动时间更新定时器（每秒更新一次）
    timeUpdateTimer = new QTimer(this);
    connect(timeUpdateTimer, &QTimer::timeout, this, &MainWindow::updateTimeDisplay);
    timeUpdateTimer->start(1000); // 每秒触发一次



    memset(mLight, 0, sizeof(mLight));
    memset(mLightBlue, 0, sizeof(mLightBlue));
    memset(mLightRed, 0, sizeof(mLightRed));
    memset(mTemperature, 0, sizeof(mTemperature));
    memset(mTemperatureBlue, 0, sizeof(mTemperatureBlue));
    memset(mTemperatureRed, 0, sizeof(mTemperatureRed));
    memset(mGasConcentration, 0, sizeof(mGasConcentration));
    memset(mGasConcentrationBlue, 0, sizeof(mGasConcentrationBlue));
    memset(mGasConcentrationRed, 0, sizeof(mGasConcentrationRed));
    memset(mHumidity, 0, sizeof(mHumidity));
    memset(mHumidityBlue, 0, sizeof(mHumidityBlue));
    memset(mHumidityRed, 0, sizeof(mHumidityRed));

    updateLight();
    ui->lbHigh->installEventFilter(this);

    allLogs.clear();
    for (int i = 0; i < MAX_NODES; ++i) {
        allLogs[i] = QStringList(); // 为每个节点创建空日志列表
    }


    ui->spinBox_light->setRange(0,10000);
    ui->spinBox_temperature->setRange(0,10000);
    ui->spinBox_gas->setRange(0,10000);
    ui->spinBox_humidity->setRange(0, 10000);

    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::saveTemperatureThreshold);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::saveLightThreshold);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::saveGasThreshold);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::saveHumidityThreshold);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onListWidgetItemClicked);

    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::Send_Temp_Value);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::Send_Light_Value);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::Send_Gas_Value);


    connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::Smoke_Warning);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::Temp_Warning);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &MainWindow::OneTap_Warning);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &MainWindow::Stop_Warning);


    for (int i = 0; i < ui->listWidget->count(); ++i) {
        nodeThresholds[i] = NodeThresholds();
    }

    connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::exportLog);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::exportImage);

    QString configPath = QDir::homePath() + "/.config/env_monitor.ini";
    settings = new QSettings(configPath, QSettings::IniFormat);
    loadThresholds();

    // 连接按钮点击信号到槽函数
    connect(ui->button1, &QPushButton::clicked, this, &MainWindow::showLightCurves);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::showTemperatureCurves);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::showGasConcentrationCurves);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onListWidgetItemClicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::showHumidityCurves);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &MainWindow::filterLogByKeyword);

    ui->comboBox->addItem("全部");
    ui->comboBox->addItems({"节点1", "节点2", "节点3"});
    ui->comboBox->setCurrentIndex(0);

    // 连接ComboBox选项变化信号到槽函数
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_currentIndexChanged);
    // 默认显示所有节点日志（初始索引-1表示全部）
    filterLogByNode(0);

    ui->comboBoxParameter->addItem("全部"); // 假设第二个ComboBox对象名为comboBoxParameter
    ui->comboBoxParameter->addItem("温度");
    ui->comboBoxParameter->addItem("湿度");
    ui->comboBoxParameter->addItem("光照");
    ui->comboBoxParameter->addItem("气体浓度");
    ui->comboBoxParameter->setCurrentIndex(0);

    connect(ui->comboBoxParameter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBoxParameter_currentIndexChanged);

    for (int nodeIndex = 0; nodeIndex < 3; ++nodeIndex) {
        parameterLogs[nodeIndex] = {
            {ParameterType::Temperature, QStringList()},
            {ParameterType::Humidity, QStringList()},
            {ParameterType::Light, QStringList()},
            {ParameterType::Gas, QStringList()}
        };
    }

    // 初始显示光照曲线
    currentCurveType = LightCurves;


    //Back接受
    workerThread = new WorkerThread(this);
    workerThread->setPortNames("/dev/pts/2", "/dev/ttyUSB1"); // 设置串口名称/dev/pts/1/dev/ttyUSB0

    // 连接 WorkerThread 的信号到 MainWindow 的槽
    connect(workerThread, &WorkerThread::uploadStatus, this, [this](const QString& status){
        addNodeLog(-1, status); // 添加上传状态到日志
    });

    connect(workerThread, &WorkerThread::errorOccurred, this, [this](const QString& error){
        addNodeLog(-1, error); // 添加错误信息到日志
        QMessageBox::critical(this, "错误", error);
    });

    // 启动工作线程
    workerThread->start();

    bool connected = connect(workerThread, &WorkerThread::dataReceived,
                             this, &MainWindow::showLoraData);
    qDebug() << "Signal connection status:" << connected;
    // 新增：默认选择节点一
    if (ui->listWidget->count() > 0) {
        ui->listWidget->setCurrentRow(0); // 选择第一个节点
        onListWidgetItemClicked(ui->listWidget->item(0)); // 触发节点选择事件
    }
}

MainWindow::~MainWindow()
{

    saveThresholds();
    delete settings;
    delete workerThread;    // 直接删除（确保无遗留）
    delete ui;
}



bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->lbHigh) {
        if (event->type() == QEvent::Paint) {
            paintLight();
        } else if (event->type() == QEvent::MouseButtonDblClick) {
            updateLight();
        } else if (event->type() == QEvent::MouseMove) {
            // 处理鼠标移动事件
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            mouseMoveEvent(mouseEvent);
            return false; // 事件已处理
        }
    }
    return QMainWindow::eventFilter(watched, event);
}
void MainWindow::updateTimeDisplay()
{
    // 重新计算每个点的当前时间（相对于初始时间的偏移）
    QTime currentTime = QTime::currentTime();
    int secondsSinceInitial = initialTime.secsTo(currentTime);

    for (int i = 0; i < 10; ++i) {
        pointTimes[i] = initialTime.addSecs(secondsSinceInitial + i * timeInterval);
    }

    // 刷新图表显示
    ui->lbHigh->update();
}

void MainWindow::paintLight()     //绘制曲线
{
    int savedHoveredIndex = hoveredPointIndex;
    hoveredPointIndex = -1;

    QPainter painter(ui->lbHigh);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int margin = 40;
    int xMin = margin;
    int xMax = ui->lbHigh->width() - margin;
    int yMin = margin;
    int yMax = ui->lbHigh->height() - margin;

    // 使用缓存的范围值
    double minY = 0;
    double maxY = currentMaxY;

    // 绘制坐标轴
    QPen axisPen(Qt::black);
    axisPen.setWidth(1);
    painter.setPen(axisPen);
    painter.drawLine(xMin, yMax, xMax, yMax); // X轴
    painter.drawLine(xMin, yMin, xMin, yMax); // Y轴

    // 绘制X轴刻度和标签（显示当前时间）
    for (int i = 0; i < 10; ++i) {
        double ratio = static_cast<double>(i) / 9.0;
        int x = xMin + ratio * (xMax - xMin);
        painter.drawLine(x, yMax, x, yMax + 5);

        // 绘制Y轴刻度和标签
        for (int i = 0; i <= 10; ++i) {
            double yValue = currentMinY + (currentMaxY - currentMinY) * i / 10.0;
            double ratio = (yValue - currentMinY) / (currentMaxY - currentMinY);
            int y = yMax - ratio * (yMax - yMin);
            painter.drawLine(xMin - 5, y, xMin, y);
            painter.drawText(xMin - 30, y + 5, QString::number(yValue, 'f', 0));
        }


        // 显示当前时间（会随定时器更新而变化）
        QString timeText = pointTimes[i].toString("hh:mm:ss");
        painter.drawText(x - 20, yMax + 20, timeText);
    }

    // 计算数据点位置（固定10个点位置，下标0对应最左侧）
    double pointx[10] = {0.0};
    for (int i = 0; i < 10; ++i) {
        double ratio = static_cast<double>(i) / 9.0;
        pointx[i] = xMin + ratio * (xMax - xMin);
    }

    // 确定每个节点的有效数据点数量（从下标0开始连续非零值的数量）
    int validPoints[3] = {0};
    for (int node = 0; node < 3; ++node) {
        const double *data = nullptr;
        switch(node) {
        case 0: data = mTemperatureRed; break;
        case 1: data = mTemperatureBlue; break;
        case 2: data = mTemperature; break;
        }

        validPoints[node] = 0;
        for (int i = 0; i < 10; ++i) {
            if (data[i] != 0)
                validPoints[node]++;
            else
                break; // 遇到0则停止计数
        }
    }

    // 绘制平滑曲线（只绘制有效数据点）
    auto drawSmoothCurve = [&](const double values[], const QColor &color, int validCount) {
        if (validCount < 2) return;

        QPen pen;
        pen.setWidth(2);
        pen.setColor(color);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        QPainterPath path;
        path.moveTo(pointx[0], yMax - ((values[0] - minY) / (maxY - minY)) * (yMax - yMin));

        for (int i = 1; i < validCount; ++i) {
            double x1 = pointx[i-1];
            double y1 = yMax - ((values[i-1] - minY) / (maxY - minY)) * (yMax - yMin);
            double x2 = pointx[i];
            double y2 = yMax - ((values[i] - minY) / (maxY - minY)) * (yMax - yMin);

            double controlX1 = x1 + (x2 - x1) / 3.0;
            double controlY1 = y1;
            double controlX2 = x2 - (x2 - x1) / 3.0;
            double controlY2 = y2;

            path.cubicTo(controlX1, controlY1, controlX2, controlY2, x2, y2);
        }

        painter.drawPath(path);
    };

    // 存储所有数据点的位置和值，用于鼠标悬停检测
    pointData.clear();

    // 绘制三条曲线和数据点
    auto drawCurveAndPoints = [&](const double values[], const QColor &color, int validCount, int nodeIndex) {
        // 绘制曲线
        drawSmoothCurve(values, color, validCount);

        // 绘制数据点
        QPen pointPen(color);
        pointPen.setWidth(2);
        painter.setPen(pointPen);
        painter.setBrush(color);

        for (int i = 0; i < validCount; ++i) {
            double y = yMax - ((values[i] - minY) / (maxY - minY)) * (yMax - yMin);
            painter.drawEllipse(QPointF(pointx[i], y), 4, 4);

            // 存储数据点信息，包括时间
            PointInfo info;
            info.x = pointx[i];
            info.y = y;
            info.value = values[i];
            info.nodeIndex = nodeIndex;
            info.pointIndex = i;
            info.time = pointTimes[i]; // 添加时间信息
            pointData.append(info);
        }
    };

    // 获取当前曲线类型的数据
    const double *data1, *data2, *data3;
    if (currentCurveType == LightCurves) {
        data1 = mLight;
        data2 = mLightBlue;
        data3 = mLightRed;
    } else if (currentCurveType == TemperatureCurves) {
        data1 = mTemperature;
        data2 = mTemperatureBlue;
        data3 = mTemperatureRed;
    } else if (currentCurveType == GasConcentrationCurves) {
        data1 = mGasConcentration;
        data2 = mGasConcentrationBlue;
        data3 = mGasConcentrationRed;
    } else if (currentCurveType == HumidityCurves) {
        data1 = mHumidity;
        data2 = mHumidityBlue;
        data3 = mHumidityRed;
    }

    // 绘制三条曲线和数据点
    drawCurveAndPoints(data1, QColor(255, 170, 0), validPoints[2], 2);
    drawCurveAndPoints(data2, Qt::blue, validPoints[1], 1);
    drawCurveAndPoints(data3, Qt::red, validPoints[0], 0);

    // 显示当前曲线类型
    QString curveTypeText;
    if (currentCurveType == LightCurves) {
        curveTypeText = "当前是光照曲线";
    } else if (currentCurveType == TemperatureCurves) {
        curveTypeText = "当前是温度曲线";
    } else if (currentCurveType == GasConcentrationCurves) {
        curveTypeText = "当前是气体浓度曲线";
    } else if (currentCurveType == HumidityCurves) {
        curveTypeText = "当前是湿度曲线";
    }
    painter.drawText(xMin + 10, yMin + 20, curveTypeText);

    // 绘制图例和悬停提示
    drawLegend(painter);

    // 绘制数据点到坐标轴的虚线
    QPen dashedPen(Qt::DashLine);
    dashedPen.setColor(QColor(128, 128, 128));
    dashedPen.setWidth(1);
    painter.setPen(dashedPen);
    foreach(const PointInfo &info, pointData) {
        painter.drawLine(info.x, info.y, info.x, yMax);
        painter.drawLine(info.x, info.y, xMin, info.y);
    }

    // 恢复悬停索引并绘制提示
    hoveredPointIndex = savedHoveredIndex;
    if (hoveredPointIndex >= 0 && !pointData.isEmpty() && hoveredPointIndex < pointData.size()) {
        const PointInfo &info = pointData[hoveredPointIndex];
        QString nodeName;
        switch(info.nodeIndex) {
        case 0: nodeName = "节点1(红)"; break;
        case 1: nodeName = "节点2(蓝)"; break;
        case 2: nodeName = "节点3(橙)"; break;
        }

        //QString tooltip = QString("%1\n值: %2\n时间: %3")
        //.arg(nodeName)
        //.arg(info.value, 0, 'f', 1)
        //.arg(info.time.toString("hh:mm:ss"));
        QString tooltip = QString("%1\n值: %2")
                              .arg(nodeName)
                              .arg(info.value, 0, 'f', 1);
        // 计算提示框大小
        QFontMetrics fm(painter.font());
        QRect textRect = fm.boundingRect(QRect(), Qt::AlignLeft, tooltip);
        QRect rect = textRect.adjusted(-10, -10, 10, 10); // 增加边距

        // 计算提示框位置
        int margin = 10;
        int rectX = info.x + 10;
        int rectY = info.y - rect.height() - 10;

        // 检查是否超出边界
        if (rectX + rect.width() > xMax) rectX = info.x - rect.width() - 10;
        if (rectX < xMin) rectX = xMin;
        if (rectY < yMin) rectY = info.y + 10;
        if (rectY + rect.height() > yMax) rectY = yMax - rect.height();

        rect.moveTo(rectX, rectY);

        painter.setBrush(QColor(128, 0, 128, 200));
        painter.setPen(QColor(255, 255, 255));
        painter.drawRect(rect);

        painter.setPen(QColor(255, 255, 255));
        painter.drawText(rect, Qt::AlignCenter, tooltip);
    }

    painter.end();
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    // 将鼠标坐标转换为lbHigh控件的局部坐标
    QPoint localPos = ui->lbHigh->mapFromParent(event->pos());

    // 确保坐标在lbHigh范围内
    if (ui->lbHigh->rect().contains(localPos)) {
        double mouseX = localPos.x();
        double mouseY = localPos.y();

        double previousHoveredIndex = hoveredPointIndex;
        hoveredPointIndex = -1;
        double minDistance = 8.0; // 调整检测范围

        for (int i = 0; i < pointData.size(); ++i) {
            const PointInfo &info = pointData[i];
            double dx = mouseX - info.x;
            double dy = mouseY - info.y;
            double distance = std::hypot(dx, dy);

            if (distance < minDistance) {
                minDistance = distance;
                hoveredPointIndex = i;
            }
        }

        // 只有当悬停状态改变时才触发重绘，优化性能
        if (hoveredPointIndex != previousHoveredIndex) {
            ui->lbHigh->update();
        }
    } else {
        // 鼠标移出控件范围时重置悬停状态
        if (hoveredPointIndex != -1) {
            hoveredPointIndex = -1;
            ui->lbHigh->update();
        }
    }
}

void MainWindow::showLoraData(const Lora::LoraData &data)
{

    int nodeIndex = data.nodeId - 1;

    // 计算当前有效数据点数量（从下标0开始连续非零值的数量）
    int validCount = 0;
    const double *currentData = getCurrentCurveData(nodeIndex);
    for (int i = 0; i < 10; ++i) {
        if (currentData[i] != 0)
            validCount++;
        else
            break; // 遇到0则停止计数
    }
    // 若未满10个点，直接在右侧添加新数据
    if (validCount < 10) {
        switch(nodeIndex) {
        case 0:
            mTemperatureRed[validCount] = data.temp;
            mHumidityRed[validCount] = data.hum;
            mLightRed[validCount] = data.light;
            mGasConcentrationRed[validCount] = data.gas;
            break;
        case 1:
            mTemperatureBlue[validCount] = data.temp;
            mHumidityBlue[validCount] = data.hum;
            mLightBlue[validCount] = data.light;
            mGasConcentrationBlue[validCount] = data.gas;
            break;
        case 2:
            mTemperature[validCount] = data.temp;
            mHumidity[validCount] = data.hum;
            mLight[validCount] = data.light;
            mGasConcentration[validCount] = data.gas;
            break;
        }
    }
    // 若已满10个点，全体左移，丢弃最早数据（下标0），新数据添加到下标9
    else {
        for (int i = 0; i < 9; ++i) {
            switch(nodeIndex) {
            case 0:
                mTemperatureRed[i] = mTemperatureRed[i+1];
                mHumidityRed[i] = mHumidityRed[i+1];
                mLightRed[i] = mLightRed[i+1];
                mGasConcentrationRed[i] = mGasConcentrationRed[i+1];
                break;
            case 1:
                mTemperatureBlue[i] = mTemperatureBlue[i+1];
                mHumidityBlue[i] = mHumidityBlue[i+1];
                mLightBlue[i] = mLightBlue[i+1];
                mGasConcentrationBlue[i] = mGasConcentrationBlue[i+1];
                break;
            case 2:
                mTemperature[i] = mTemperature[i+1];
                mHumidity[i] = mHumidity[i+1];
                mLight[i] = mLight[i+1];
                mGasConcentration[i] = mGasConcentration[i+1];
                break;
            }
        }
        // 添加新数据到下标9
        switch(nodeIndex) {
        case 0:
            mTemperatureRed[9] = data.temp;
            mHumidityRed[9] = data.hum;
            mLightRed[9] = data.light;
            mGasConcentrationRed[9] = data.gas;
            break;
        case 1:
            mTemperatureBlue[9] = data.temp;
            mHumidityBlue[9] = data.hum;
            mLightBlue[9] = data.light;
            mGasConcentrationBlue[9] = data.gas;
            break;
        case 2:
            mTemperature[9] = data.temp;
            mHumidity[9] = data.hum;
            mLight[9] = data.light;
            mGasConcentration[9] = data.gas;
            break;
        }
    }

    if (validCount < 10) {
        // 未满10个点，添加新时间
        pointTimes[validCount] = QTime::currentTime();
    } else {
        // 已满10个点，时间戳左移
        for (int i = 0; i < 9; ++i) {
            pointTimes[i] = pointTimes[i+1];
        }
        // 添加新时间
        pointTimes[9] = QTime::currentTime();
    }
    // 后续逻辑保持不变
    checkCurrentDataThresholds(nodeIndex, data);
    updateLight();



    // 构建带时间戳的日志消息

    QString logMessage = QString(" Node ID: %1, Temp: %2°C, Hum: %3%, Light: %4lux, Gas: %5ppm")
                             .arg(data.nodeId)
                             .arg(data.temp, 0, 'f', 1)
                             .arg(data.hum)
                             .arg(data.light)
                             .arg(data.gas);

    // 添加到统一的日志存储（使用修复后的函数）
    addNodeLog(nodeIndex, logMessage);

}


void MainWindow::checkCurrentDataThresholds(int nodeIndex, const Lora::LoraData &data)
{
    if (data.temp > nodeThresholds[nodeIndex].temperature)
        alertThresholdExceeded(nodeIndex, "温度");
    if (data.light > nodeThresholds[nodeIndex].light)
        alertThresholdExceeded(nodeIndex, "光照");
    if (data.gas > nodeThresholds[nodeIndex].gas)
        alertThresholdExceeded(nodeIndex, "气体浓度");
    if (data.hum > nodeThresholds[nodeIndex].humidity)
        alertThresholdExceeded(nodeIndex, "湿度");
}
void MainWindow::updateLogDisplay()
{
    // 保存当前滚动位置
    QScrollBar *scrollBar = ui->plainTextEdit->verticalScrollBar();
    int scrollPosition = scrollBar->value();
    bool atBottom = (scrollPosition == scrollBar->maximum());

    // 清空现有内容
    ui->plainTextEdit->clear();

    // 获取当前过滤条件
    int nodeIndex = ui->comboBox->currentIndex() - 1;
    ParameterType paramType = currentParameterType;
    QString keyword = currentSearchKeyword.trimmed();

    // 构建过滤标题
    QString header;
    if (nodeIndex == -1 && paramType == ParameterType::All) {
        header = "显示全部日志：";
    } else if (nodeIndex == -1 && paramType != ParameterType::All) {
        header = QString("显示所有节点%1相关日志：").arg(getParameterName(paramType));
    } else if (nodeIndex != -1 && paramType == ParameterType::All) {
        header = QString("显示节点%1所有参数日志：").arg(nodeIndex + 1);
    } else {
        header = QString("显示节点%1的%2日志：").arg(nodeIndex + 1).arg(getParameterName(paramType));
    }

    if (!keyword.isEmpty()) {
        header += QString("（包含关键字：\"%1\"）").arg(keyword);
    }

    ui->plainTextEdit->insertPlainText(header + "\n\n");

    // 显示系统日志（如果选择全部或系统日志）
    if (nodeIndex == -1) {
        ui->plainTextEdit->insertPlainText("系统日志：\n");
        foreach (const QString &log, systemLogs) {
            if (keyword.isEmpty() || log.contains(keyword, Qt::CaseInsensitive)) {
                ui->plainTextEdit->appendPlainText(log);
            }
        }
        ui->plainTextEdit->insertPlainText("\n");
    }

    // 创建一个有序的日志列表
    QList<QString> filteredLogs;

    // 遍历所有节点的日志
    for (int nodeId = 0; nodeId < MAX_NODES; ++nodeId) {
        // 过滤节点
        if (nodeIndex != -1 && nodeId != nodeIndex) continue;

        // 获取该节点的所有日志
        if (allLogs.contains(nodeId)) {
            foreach (const QString &log, allLogs[nodeId]) {
                // 过滤参数类型
                bool paramMatch = (paramType == ParameterType::All);
                if (!paramMatch) {
                    QString paramName = getParameterName(paramType);
                    if (!log.contains(paramName)) continue;
                }

                // 过滤关键字
                if (keyword.isEmpty() || log.contains(keyword, Qt::CaseInsensitive)) {
                    filteredLogs.append(log);
                }
            }
        }
    }

    // 按时间戳排序
    std::sort(filteredLogs.begin(), filteredLogs.end(), [](const QString &a, const QString &b) {
        QString timeA = a.mid(1, 19);
        QString timeB = b.mid(1, 19);
        return QDateTime::fromString(timeA, "yyyy-MM-dd hh:mm:ss") <
               QDateTime::fromString(timeB, "yyyy-MM-dd hh:mm:ss");
    });

    // 显示所有过滤后的日志
    foreach (const QString &log, filteredLogs) {
        ui->plainTextEdit->appendPlainText(log);
    }

    // 恢复滚动位置
    if (atBottom) {
        scrollBar->setValue(scrollBar->maximum());
    } else {
        scrollBar->setValue(scrollPosition);
    }
}
ParameterType MainWindow::getParameterTypeFromData(const Lora::LoraData &data) {
    // 这里可以根据数据内容决定具体的参数类型
    // 例如根据哪个参数变化最大来决定
    // 简化版本，返回所有参数类型
    return ParameterType::All;
}

void MainWindow::calculateNiceScale(double min, double max, int numTicks, double *niceMin, double *niceMax, double *tickSpacing)
{
    // 根据不同曲线类型设置固定刻度间隔
    if (currentCurveType == LightCurves) {
        *tickSpacing = 100; // 光照曲线每100一个刻度
    } else if (currentCurveType == TemperatureCurves) {
        *tickSpacing = 10; // 温度曲线每10一个刻度
    } else if (currentCurveType == HumidityCurves) {
        *tickSpacing = 20; // 湿度曲线每20一个刻度
    } else if (currentCurveType == GasConcentrationCurves) {
        *tickSpacing = 100; // 气体浓度曲线每100一个刻度
    } else {
        *tickSpacing = 20; // 默认间隔
    }

    // 固定最小值和最大值
    *niceMin = currentMinY;
    *niceMax = currentMaxY;
}



// 辅助函数：生成"美观"的数字
double MainWindow::niceNumber(double value, bool round)
{
    int exponent;      // 10的幂
    double fraction;   // 最高位数字
    double niceFraction; // 格式化后的最高位数字

    exponent = std::floor(std::log10(value));
    fraction = value / std::pow(10, exponent);

    if (round) {
        if (fraction < 1.5)
            niceFraction = 1;
        else if (fraction < 3)
            niceFraction = 2;
        else if (fraction < 7)
            niceFraction = 5;
        else
            niceFraction = 10;
    } else {
        if (fraction <= 1)
            niceFraction = 1;
        else if (fraction <= 2)
            niceFraction = 2;
        else if (fraction <= 5)
            niceFraction = 5;
        else
            niceFraction = 10;
    }

    return niceFraction * std::pow(10, exponent);
}

double generateRandomDouble(double min, double max)//生成均匀分布随机浮点数
{
    return min + QRandomGenerator::global()->generateDouble() * (max - min);
}



void MainWindow::drawLegend(QPainter &painter)//绘制图例
{
    // 设置图例位置（右上角，距离边缘10像素）
    int legendX = ui->lbHigh->width() - 200;
    int legendY = 10;
    int itemHeight = 20;
    int lineWidth = 15;
    int textOffset = lineWidth + 5;

    // 定义节点颜色（兼容Qt5和Qt6的橙色表示）
    QColor nodeColors[] = {
        Qt::red,                            // 节点1 红色
        Qt::blue,                           // 节点2 蓝色
        QColor(255, 165, 0)                 // 节点3 橙色（使用RGB值，兼容所有Qt版本）
        // 或使用Qt6的方式：QColorConstants::Svg::orange
    };

    // 绘制节点图例（保持原有代码不变）
    for (int i = 0; i < 3; ++i) {
        QPainterPath wavePath;
        wavePath.moveTo(legendX, legendY + itemHeight * i + itemHeight / 2);

        for (int j = 0; j < lineWidth; j += 2) {
            wavePath.cubicTo(
                legendX + j, legendY + itemHeight * i + itemHeight / 2 - 3,
                legendX + j + 1, legendY + itemHeight * i + itemHeight / 2,
                legendX + j + 2, legendY + itemHeight * i + itemHeight / 2 + 3
                );
        }

        QPen pen(nodeColors[i]);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawPath(wavePath);

        painter.drawText(legendX + textOffset,
                         legendY + itemHeight * i + itemHeight / 2 + 3,
                         QString("节点%1").arg(i+1));
    }
}

void MainWindow::drawArrow(QPainter &painter, int x, int y, bool isHorizontal)//绘制箭头
{
    const int arrowSize = 10; // 箭头大小

    QPainterPath arrowPath;

    if (isHorizontal) {
        // 水平箭头（X轴）
        arrowPath.moveTo(x, y);
        arrowPath.lineTo(x - arrowSize, y - arrowSize / 2);
        arrowPath.lineTo(x - arrowSize, y + arrowSize / 2);
        arrowPath.closeSubpath();
    } else {
        // 垂直箭头（Y轴）
        arrowPath.moveTo(x, y);
        arrowPath.lineTo(x - arrowSize / 2, y + arrowSize);
        arrowPath.lineTo(x + arrowSize / 2, y + arrowSize);
        arrowPath.closeSubpath();
    }

    // 填充箭头
    painter.fillPath(arrowPath, Qt::black);
}


const double* MainWindow::getCurrentCurveData(int nodeIndex) const
{
    switch (currentCurveType) {
    case LightCurves:
        return (nodeIndex == 0) ? mLightRed : (nodeIndex == 1) ? mLightBlue : mLight;
    case TemperatureCurves:
        return (nodeIndex == 0) ? mTemperatureRed : (nodeIndex == 1) ? mTemperatureBlue : mTemperature;
    case GasConcentrationCurves:
        return (nodeIndex == 0) ? mGasConcentrationRed : (nodeIndex == 1) ? mGasConcentrationBlue : mGasConcentration;
    case HumidityCurves:
        return (nodeIndex == 0) ? mHumidityRed : (nodeIndex == 1) ? mHumidityBlue : mHumidity;
    default:
        return nullptr;
    }
}


void MainWindow::updateLight()//生成随机数据
{
    hoveredPointIndex = -1; // 新增：重置悬停索引
    pointData.clear(); // 新增：清空旧数据点
    calculateYAxisRange();
    ui->lbHigh->update();
}
void MainWindow::calculateYAxisRange()//固定Y轴范围
{
    // 根据当前曲线类型设置固定Y轴范围
    switch(currentCurveType) {
    case LightCurves:
        currentMinY = 0;
        currentMaxY = 500; // 光照曲线Y轴范围固定为0~500
        break;
    case TemperatureCurves:
        currentMinY = 0;
        currentMaxY = 40; // 温度曲线Y轴范围固定为0~40
        break;
    case HumidityCurves:
        currentMinY = 0;
        currentMaxY = 100; // 湿度曲线Y轴范围固定为0~100
        break;
    case GasConcentrationCurves:
        currentMinY = 0;
        currentMaxY = 400; // 气体浓度曲线Y轴范围固定为0~400
        break;
    default:
        // 默认范围（防止异常情况）
        currentMinY = 0;
        currentMaxY = 100;
        break;
    }
}


void MainWindow::showLightCurves()
{
    currentCurveType = LightCurves;
    pointData.clear();
    calculateYAxisRange(); // 添加范围计算
    ui->lbHigh->update();
}

void MainWindow::showTemperatureCurves()
{
    currentCurveType = TemperatureCurves;
    pointData.clear();
    calculateYAxisRange(); // 添加范围计算
    ui->lbHigh->update();
}

void MainWindow::showGasConcentrationCurves()
{
    currentCurveType = GasConcentrationCurves;
    pointData.clear();
    calculateYAxisRange(); // 添加范围计算
    ui->lbHigh->update();
}

void MainWindow::showHumidityCurves()
{
    currentCurveType = HumidityCurves;
    pointData.clear();
    calculateYAxisRange(); // 添加范围计算
    ui->lbHigh->update();
}
void MainWindow::onListWidgetItemClicked(QListWidgetItem *item)
{

    int nodeIndex = ui->listWidget->row(item);

    // 更新 spinBox 显示当前节点的阈值
    ui->spinBox_temperature->setValue(nodeThresholds[nodeIndex].temperature);
    ui->spinBox_light->setValue(nodeThresholds[nodeIndex].light);
    ui->spinBox_gas->setValue(nodeThresholds[nodeIndex].gas);
    ui->spinBox_humidity->setValue(nodeThresholds[nodeIndex].humidity);

    if (ui->textEdit_thresholds) { // 确保指针有效（假设控件名已改为 textEdit_thresholds）
        QString thresholdsText = QString("当前温度阈值: %1\n当前湿度阈值: %2\n当前光照阈值: %3\n当前气体阈值: %4")
                                     .arg(nodeThresholds[nodeIndex].temperature)
                                     .arg(nodeThresholds[nodeIndex].humidity)
                                     .arg(nodeThresholds[nodeIndex].light)
                                     .arg(nodeThresholds[nodeIndex].gas);

        // 使用 setPlainText() 方法设置文本
        ui->textEdit_thresholds->setPlainText(thresholdsText);
    }
}
void MainWindow::showSuccessDialog(const QString &message)
{
    SuccessDialog dialog(this);
    dialog.setMessage(message);
    dialog.exec();
}

SuccessDialog::SuccessDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("提示");
    setFixedSize(400, 200);
    messageLabel = new QLabel("保存成功!", this);
    messageLabel->setGeometry(50, 30, 300, 40);
    okButton = new QPushButton("确定", this);
    okButton->setGeometry(150, 80, 80, 30);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    restoreButton = new QPushButton("还原", this);
    restoreButton->setGeometry(250, 80, 80, 30);
}

void SuccessDialog::setMessage(const QString &message)
{
    if (messageLabel)
    {
        messageLabel->setText(message);
    }
}

void MainWindow::exportLog()//日志导出
{
    // 打开文件对话框选择保存位置（改为TXT格式）
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "导出日志",
        QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".txt",
        "TXT文件 (*.txt);;所有文件 (*)"
        );

    if (filePath.isEmpty())
        return; // 用户取消了操作

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << filePath;
        showSuccessDialog("日志导出失败");
        return;
    }

    QTextStream out(&file);
    out << "================= 环境监测系统日志 =================" << Qt::endl;
    out << "导出时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << Qt::endl << Qt::endl;

    // 记录当前曲线类型
    QString curveTypeName;
    if (currentCurveType == LightCurves) {
        curveTypeName = "光照曲线";
    } else if (currentCurveType == TemperatureCurves) {
        curveTypeName = "温度曲线";
    } else {
        curveTypeName = "气体浓度曲线";
    }
    out << "当前曲线类型: " << curveTypeName << Qt::endl << Qt::endl;

    // 记录阈值信息
    out << "================= 阈值设置 ==================" << Qt::endl;
    out << "光照阈值: " << ui->spinBox_light->value() << Qt::endl;
    out << "温度阈值: " << ui->spinBox_temperature->value() << Qt::endl;
    out << "气体浓度阈值: " << ui->spinBox_gas->value() << Qt::endl << Qt::endl;

    // 记录日志信息
    out << "================= 操作日志 ==================" << Qt::endl;
    out << ui->plainTextEdit->toPlainText() << Qt::endl;

    file.close();
    showSuccessDialog("日志已导出为TXT文件");
}

// 新增导出图像功能实现
void MainWindow::exportImage()//图像导出
{
    // 打开文件对话框选择保存位置和格式
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "导出图像",
        QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png",
        "PNG图像 (*.png);;JPEG图像 (*.jpg);;BMP图像 (*.bmp);;所有文件 (*)"
        );

    if (filePath.isEmpty())
        return; // 用户取消了操作

    // 获取图像并保存
    QPixmap pixmap = ui->lbHigh->grab();
    if (pixmap.save(filePath)) {
        showSuccessDialog("图像导出成功");
    } else {
        showSuccessDialog("图像导出失败");
    }
}
void MainWindow::loadThresholds()
{
    // 遍历所有节点，加载保存的阈值
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        NodeThresholds thresholds;
        thresholds.temperature = settings->value(QString("Node%1/Temperature").arg(i), 0).toInt();
        thresholds.light = settings->value(QString("Node%1/Light").arg(i), 0).toInt();
        thresholds.gas = settings->value(QString("Node%1/Gas").arg(i), 0).toInt();
        thresholds.humidity = settings->value(QString("Node%1/Humidity").arg(i), 0).toInt(); // 新增湿度加载
        nodeThresholds[i] = thresholds;
    }
    // 新增：如果没有保存的阈值，为节点一设置默认阈值
    if (nodeThresholds[0].temperature == 0) {
        nodeThresholds[0].temperature = 30; // 默认温度阈值
    }
    if (nodeThresholds[0].light == 0) {
        nodeThresholds[0].light = 500; // 默认光照阈值
    }
    if (nodeThresholds[0].gas == 0) {
        nodeThresholds[0].gas = 100; // 默认气体浓度阈值
    }
    if (nodeThresholds[0].humidity == 0) {
        nodeThresholds[0].humidity = 80; // 默认湿度阈值
    }
}
void MainWindow::saveThresholds()//保存阈值
{
    // 遍历所有节点，保存当前阈值
    for (int i = 0; i < nodeThresholds.size(); ++i) {
        settings->setValue(QString("Node%1/Temperature").arg(i), nodeThresholds[i].temperature);
        settings->setValue(QString("Node%1/Light").arg(i), nodeThresholds[i].light);
        settings->setValue(QString("Node%1/Gas").arg(i), nodeThresholds[i].gas);
        settings->setValue(QString("Node%1/Humidity").arg(i), nodeThresholds[i].humidity); // 新增湿度保存
    }
    // 确保立即写入磁盘
    settings->sync();
}


QMap<int, QStringList>& MainWindow::getNodeSpecificLogs()
{
    static QMap<int, QStringList> nodeSpecificLogs;
    return nodeSpecificLogs;
}

void MainWindow::updateThresholdDisplay(int nodeIndex)
{
    if (ui->textEdit_thresholds && nodeIndex >= 0 && nodeIndex < MAX_NODES) {
        QString thresholdsText = QString("当前温度阈值: %1\n当前湿度阈值: %2\n当前光照阈值: %3\n当前气体阈值: %4")
                                     .arg(nodeThresholds[nodeIndex].temperature)
                                     .arg(nodeThresholds[nodeIndex].humidity)
                                     .arg(nodeThresholds[nodeIndex].light)
                                     .arg(nodeThresholds[nodeIndex].gas);

        ui->textEdit_thresholds->setPlainText(thresholdsText);
    }
}
// 修复后的阈值保存函数示例
void MainWindow::saveLightThreshold()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (!currentItem) return;

    int nodeIndex = ui->listWidget->row(currentItem);
    nodeThresholds[nodeIndex].light = ui->spinBox_light->value();

    QString logMessage = QString("节点%1光照阈值设置成功").arg(nodeIndex + 1);
    addNodeLog(nodeIndex, logMessage, ParameterType::Light); // 只添加一次，指定参数类型

    showSuccessDialog("光照阈值保存成功");
    saveThresholds();
    updateThresholdDisplay(nodeIndex);

}


// 同样修复其他阈值保存函数
void MainWindow::saveTemperatureThreshold()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (!currentItem) return;

    int nodeIndex = ui->listWidget->row(currentItem);
    nodeThresholds[nodeIndex].temperature = ui->spinBox_temperature->value();

    QString logMessage = QString("节点%1温度阈值设置成功").arg(nodeIndex + 1);
    addNodeLog(nodeIndex, logMessage, ParameterType::Temperature);

    showSuccessDialog("温度阈值保存成功");
    saveThresholds();
    updateThresholdDisplay(nodeIndex);

}

void MainWindow::saveHumidityThreshold()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (!currentItem) return;

    int nodeIndex = ui->listWidget->row(currentItem);
    nodeThresholds[nodeIndex].humidity = ui->spinBox_humidity->value();

    QString logMessage = QString("节点%1湿度阈值设置成功").arg(nodeIndex + 1);
    addNodeLog(nodeIndex, logMessage, ParameterType::Humidity);

    showSuccessDialog("湿度阈值保存成功");
    saveThresholds();
    updateThresholdDisplay(nodeIndex);

}

void MainWindow::saveGasThreshold()
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (!currentItem) return;

    int nodeIndex = ui->listWidget->row(currentItem);
    nodeThresholds[nodeIndex].gas = ui->spinBox_gas->value();

    QString logMessage = QString("节点%1气体阈值设置成功").arg(nodeIndex + 1);
    addNodeLog(nodeIndex, logMessage, ParameterType::Gas);

    showSuccessDialog("气体阈值保存成功");
    saveThresholds();
    updateThresholdDisplay(nodeIndex);

}
// 添加两参数版本的 addNodeLog 函数实现
void MainWindow::addNodeLog(int nodeIndex, const QString& logMessage)
{
    // 调用三参数版本，使用默认参数类型
    addNodeLog(nodeIndex, logMessage, ParameterType::All);
}
void MainWindow::addNodeLog(int nodeIndex, const QString& logMessage, ParameterType parameterType)
{
    // 允许nodeIndex = -1表示系统日志
    if (nodeIndex < -1 || nodeIndex >= MAX_NODES) {
        qDebug() << "Invalid node index:" << nodeIndex;
        return;
    }

    // 添加时间戳
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString timestampedLog = QString("[%1] %2").arg(timestamp).arg(logMessage);

    // 系统日志单独存储
    if (nodeIndex == -1) {
        systemLogs.append(timestampedLog);
    } else {
        // 添加到统一的日志存储
        allLogs[nodeIndex].append(timestampedLog);

        // 更新参数日志存储（如果需要按参数类型分类）
        if (parameterType != ParameterType::All) {
            parameterLogs[nodeIndex][parameterType].append(timestampedLog);
        }
    }

    // 更新日志显示
    updateLogDisplay();
}

void MainWindow::applyLogFilters()
{
    updateLogDisplay(); // 直接调用更新函数，使用当前的过滤条件
}
// 修改后的filterLogByNode函数
void MainWindow::filterLogByNode(int comboBoxIndex)
{
    int currentNodeIndex = comboBoxIndex - 1;
    ui->plainTextEdit->clear();

    if (currentNodeIndex == -1) {
        // 显示所有节点的日志
        ui->plainTextEdit->insertPlainText("显示全部节点日志：\n");

        // 遍历所有节点的日志列表
        for (int nodeIndex = 0; nodeIndex < MAX_NODES; ++nodeIndex) {
            if (allLogs.contains(nodeIndex)) {
                foreach (const QString &log, allLogs[nodeIndex]) {
                    ui->plainTextEdit->appendPlainText(log);
                }
            }
        }
    } else {
        // 显示特定节点的日志
        ui->plainTextEdit->insertPlainText(QString("显示节点%1的日志：\n").arg(currentNodeIndex + 1));

        if (allLogs.contains(currentNodeIndex)) {
            foreach (const QString &log, allLogs[currentNodeIndex]) {
                ui->plainTextEdit->appendPlainText(log);
            }
        }
    }
}

// 修改后的filterLogByParameter函数
void MainWindow::filterLogByParameter(ParameterType parameterType)
{
    ui->plainTextEdit->clear();
    int currentNodeIndex = ui->comboBox->currentIndex() - 1;

    if (parameterType == ParameterType::All) {
        // 显示当前节点的所有参数日志（或全部节点的所有参数日志）
        filterLogByNode(ui->comboBox->currentIndex());
        return;
    }

    // 处理非All情况
    QString parameterName = getParameterName(parameterType);
    if (currentNodeIndex == -1) {
        ui->plainTextEdit->insertPlainText(QString("显示所有节点%1相关日志：\n").arg(parameterName));
        for (int nodeId = 0; nodeId < MAX_NODES; ++nodeId) {
            if (allLogs.contains(nodeId)) {
                foreach (const QString &log, allLogs[nodeId]) {
                    if (log.contains(parameterName)) {
                        ui->plainTextEdit->appendPlainText(log);
                    }
                }
            }
        }
    } else {
        ui->plainTextEdit->insertPlainText(QString("显示节点%1的%2日志：\n").arg(currentNodeIndex+1).arg(parameterName));
        if (allLogs.contains(currentNodeIndex)) {
            foreach (const QString &log, allLogs[currentNodeIndex]) {
                if (log.contains(parameterName)) {
                    ui->plainTextEdit->appendPlainText(log);
                }
            }
        }
    }
}


// ComboBox选项变化时触发
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    currentNodeIndex = index - 1; // 转换为节点索引（-1表示全部）
    filterLogByNode(index); // 传递ComboBox索引

    applyLogFilters(); // 调用统一过滤方法
}




// 检查所有节点和参数是否超过阈值
void MainWindow::checkThresholds()
{
    if (suppressAlerts) return;

    // 检查节点1 (红色)
    if(mTemperatureRed[0] > nodeThresholds[0].temperature)
        alertThresholdExceeded(0, "温度");
    if(mLightRed[0] > nodeThresholds[0].light)
        alertThresholdExceeded(0, "光照");
    if(mGasConcentrationRed[0] > nodeThresholds[0].gas)
        alertThresholdExceeded(0, "气体浓度");
    if(mHumidityRed[0] > nodeThresholds[0].humidity)
        alertThresholdExceeded(0, "湿度");

    // 检查节点2 (蓝色)
    if(mTemperatureBlue[0] > nodeThresholds[1].temperature)
        alertThresholdExceeded(1, "温度");
    if(mLightBlue[0] > nodeThresholds[1].light)
        alertThresholdExceeded(1, "光照");
    if(mGasConcentrationBlue[0] > nodeThresholds[1].gas)
        alertThresholdExceeded(1, "气体浓度");
    if(mHumidityBlue[0] > nodeThresholds[1].humidity)
        alertThresholdExceeded(1, "湿度");

    // 检查节点3 (橙色)
    if(mTemperature[0] > nodeThresholds[2].temperature)
        alertThresholdExceeded(2, "温度");
    if(mLight[0] > nodeThresholds[2].light)
        alertThresholdExceeded(2, "光照");
    if(mGasConcentration[0] > nodeThresholds[2].gas)
        alertThresholdExceeded(2, "气体浓度");
    if(mHumidity[0] > nodeThresholds[2].humidity)
        alertThresholdExceeded(2, "湿度");
}

// 新增自定义对话框实现
ThresholdAlertDialog::ThresholdAlertDialog(const QString &message, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("阈值超限警告");
    setFixedSize(500, 200);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint); // 确保对话框显示在最前

    // 设置样式
    setStyleSheet(R"(
        QDialog {
            background-color: #f8f9fa;
            border-radius: 8px;
        }
        QLabel {
            color: #dc3545;
            font-size: 16px;
            font-weight: bold;
            padding: 15px;
            text-align: center;
        }
        QPushButton {
            background-color: #007bff;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 14px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #0069d9;
        }
    )");

    // 创建内容
    QLabel *label = new QLabel(message, this);
    QPushButton *okButton = new QPushButton("确认", this);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label, 1, Qt::AlignCenter);
    layout->addWidget(okButton, 0, Qt::AlignCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(20, 20, 20, 20);
}



void MainWindow::alertThresholdExceeded(int nodeIndex, const QString &parameter)
{
    double currentValue = getCurrentValue(nodeIndex, parameter);

    // 简化消息格式，避免换行（添加参数值信息）
    QString message = QString("⚠️ 节点%1的%2参数（当前值: %3）超过阈值（阈值: %4） ⚠️")
                          .arg(nodeIndex + 1)
                          .arg(parameter)
                          .arg(currentValue, 0, 'f', 1) // 显示1位小数
                          .arg(getThresholdValue(nodeIndex, parameter));

    // 使用更新后的对话框（水平布局，单行显示）
    ThresholdAlertDialog dialog(message, this);
    dialog.exec();

    // 参数类型映射
    ParameterType paramType = ParameterType::All;
    if (parameter == "温度") paramType = ParameterType::Temperature;
    else if (parameter == "湿度") paramType = ParameterType::Humidity;
    else if (parameter == "光照") paramType = ParameterType::Light;
    else if (parameter == "气体浓度") paramType = ParameterType::Gas;

    addNodeLog(nodeIndex, message, paramType);
}


double MainWindow::getCurrentValue(int nodeIndex, const QString &parameter) const
{
    // 根据节点索引和参数类型返回当前值
    if (parameter == "温度") {
        if (nodeIndex == 0) return mTemperatureRed[0];
        if (nodeIndex == 1) return mTemperatureBlue[0];
        return mTemperature[0];
    }
    else if (parameter == "湿度") {
        if (nodeIndex == 0) return mHumidityRed[0];
        if (nodeIndex == 1) return mHumidityBlue[0];
        return mHumidity[0];
    }
    else if (parameter == "光照") {
        if (nodeIndex == 0) return mLightRed[0];
        if (nodeIndex == 1) return mLightBlue[0];
        return mLight[0];
    }
    else if (parameter == "气体浓度") {
        if (nodeIndex == 0) return mGasConcentrationRed[0];
        if (nodeIndex == 1) return mGasConcentrationBlue[0];
        return mGasConcentration[0];
    }
    return 0.0;
}

double MainWindow::getThresholdValue(int nodeIndex, const QString &parameter) const
{
    // 根据节点索引和参数类型返回阈值
    if (parameter == "温度") return nodeThresholds[nodeIndex].temperature;
    if (parameter == "湿度") return nodeThresholds[nodeIndex].humidity;
    if (parameter == "光照") return nodeThresholds[nodeIndex].light;
    if (parameter == "气体浓度") return nodeThresholds[nodeIndex].gas;
    return 0.0;
}
// 新增参数筛选ComboBox处理函数
void MainWindow::on_comboBoxParameter_currentIndexChanged(int index)
{
    switch (index) {
    case 0: currentParameterType = ParameterType::All; break;
    case 1: currentParameterType = ParameterType::Temperature; break;
    case 2: currentParameterType = ParameterType::Humidity; break;
    case 3: currentParameterType = ParameterType::Light; break;
    case 4: currentParameterType = ParameterType::Gas; break;
    default: currentParameterType = ParameterType::All;
    }

    // 结合节点和参数进行双重筛选
    filterLogByNode(ui->comboBox->currentIndex()); // 使用原有节点筛选函数
    filterLogByParameter(currentParameterType);

    applyLogFilters(); // 调用统一过滤方法
}

QString MainWindow::getParameterName(ParameterType type) const
{
    switch (type) {
    case ParameterType::Temperature: return "温度";
    case ParameterType::Humidity: return "湿度";
    case ParameterType::Light: return "光照";
    case ParameterType::Gas: return "气体浓度";
    case ParameterType::All: return "全部";
    default: return "未知";
    }
}

void MainWindow::filterLogByKeyword(const QString &keyword)
{
    currentSearchKeyword = keyword.trimmed();
    applyLogFilters(); // 应用所有过滤条件（节点+参数+关键字）
}

//Warning
void MainWindow::Smoke_Warning() {
    QString message = "mbj" ;
    QMetaObject::invokeMethod(workerThread, "slotWriteToLora", Qt::QueuedConnection,
                              Q_ARG(QString, message));
}
void MainWindow::Temp_Warning() {

    QString message = "tbj" ;
    QMetaObject::invokeMethod(workerThread, "slotWriteToLora", Qt::QueuedConnection,
                              Q_ARG(QString, message));
}
void MainWindow::OneTap_Warning() {

    QString message = "1jbj" ;
    QMetaObject::invokeMethod(workerThread, "slotWriteToLora", Qt::QueuedConnection,
                              Q_ARG(QString, message));
}
void MainWindow::Stop_Warning() {

    QString message = "1nbj";
    QMetaObject::invokeMethod(workerThread, "slotWriteToLora", Qt::QueuedConnection,
                              Q_ARG(QString, message));
}

//Send Key_value
void MainWindow::Send_Light_Value() {
    int light = ui->spinBox_light->value();

    QString message = "L" + QString::number(light);
    QMetaObject::invokeMethod(workerThread, "slotWriteToLora", Qt::QueuedConnection,
                              Q_ARG(QString, message));
}
void MainWindow::Send_Temp_Value() {
    int temp = ui->spinBox_temperature->value();

    QString message = "T" + QString::number(temp);
    QMetaObject::invokeMethod(workerThread, "slotWriteToLora", Qt::QueuedConnection,
                              Q_ARG(QString, message));
}
void MainWindow::Send_Gas_Value() {
    int gas = ui->spinBox_gas->value();

    QString message = "M" + QString::number(gas);
    QMetaObject::invokeMethod(workerThread, "slotWriteToLora", Qt::QueuedConnection,
                              Q_ARG(QString, message));
}

