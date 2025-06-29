#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QEvent>
#include <QPainter>
#include <QPushButton>
#include <QRandomGenerator>
#include <QListWidget>
#include<QDialog>
#include<QLabel>
#include <QSettings>
#include <QComboBox>
#include <QStringList>
#include <QTimer>
#include <QScrollBar>
#include <QDateTime>

#include "lora.h"
#include "esp8266.h"
#include "workerthread.h"

struct PointInfo {
    double x, y;      // 屏幕坐标
    double value;     // 数据值
    int nodeIndex;    // 节点索引
    int pointIndex;   // 点在数组中的索引
    QTime time; // 添加时间成员
};


struct NodeThresholds {
    int temperature;
    int light;
    int gas;
    int humidity;
    NodeThresholds() : temperature(0), light(0), gas(0), humidity(0) {}

};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


enum CurveType {
    LightCurves,
    TemperatureCurves,
    GasConcentrationCurves,
    HumidityCurves
};

enum class ParameterType {
    All,
    Temperature,
    Humidity,
    Light,
    Gas
};


class ThresholdAlertDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ThresholdAlertDialog(const QString &message, QWidget *parent = nullptr);
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)  override;

private slots:
    void paintLight();//绘制曲线
    //坐标轴刻度优化
    void calculateNiceScale(double min, double max, int numTicks,double *niceMin, double *niceMax, double *tickSpacing);
    void calculateYAxisRange();//自适应Y轴

    void drawLegend(QPainter &painter);//绘制图例
    void drawArrow(QPainter &painter, int x, int y, bool isHorizontal);//绘制箭头

    double niceNumber(double value, bool round);// 辅助函数：生成"美观"的数字

    void updateLight();//生成随机数据

    void showLightCurves();
    void showTemperatureCurves();
    void showGasConcentrationCurves();
    void showHumidityCurves();
    void onListWidgetItemClicked(QListWidgetItem *item);//点击列表项时更新界面上的阈值显示

    void saveTemperatureThreshold();
    void saveLightThreshold();
    void saveGasThreshold();
    void saveHumidityThreshold();
    void updateThresholdDisplay(int nodeIndex);
    void showSuccessDialog(const QString &message);

    void exportLog();//日志导出
    void exportImage();//图像导出

    void loadThresholds();
    void saveThresholds();//保存阈值
    void checkThresholds(); // 检查所有节点和参数是否超过阈值
    void alertThresholdExceeded(int nodeIndex, const QString &parameter);//阈值超限警报

    void addNodeLog(int nodeIndex, const QString& logMessage);// 处理日志添加逻辑
    void filterLogByNode(int nodeIndex); // 按节点过滤日志
    void filterLogByParameter(ParameterType parameterType);//根据参数类型对日志进行筛选
    void filterLogByKeyword(const QString &keyword); // 按关键字过滤日志


    //Back
    void showLoraData(const Lora::LoraData &data);
    void checkCurrentDataThresholds(int nodeIndex, const Lora::LoraData &data);

    void updateTimeDisplay();

    void Send_Light_Value();
    void Send_Temp_Value();
    void Send_Gas_Value();

    void Smoke_Warning();
    void Temp_Warning();
    void OneTap_Warning();
    void Stop_Warning();

    void updateAlarmButtonTexts();


private:
    Ui::MainWindow *ui;

    double mLight[10];
    double mLightBlue[10];
    double mLightRed[10];
    double mTemperature[10];
    double mTemperatureBlue[10];
    double mTemperatureRed[10];
    double mGasConcentration[10];
    double mGasConcentrationBlue[10];
    double mGasConcentrationRed[10];
    double mHumidity[10];
    double mHumidityBlue[10];
    double mHumidityRed[10];
    double currentMinY = 0.0; // 当前曲线的最小Y值
    double currentMaxY = 1000.0; // 当前曲线的最大Y值

    QVector<QTime> pointTimes; // 存储每个数据点的时间
    QTime initialTime; // 初始时间
    QTimer *timeUpdateTimer; // 时间更新定时器
    int timeInterval = 10; // 时间间隔（秒）
    double timeScaleFactor;

    int currentSelectedNode;

    QSettings* settings;
    int currentNodeIndex;
    int lastNodeIndex;

    CurveType currentCurveType;
    QMap<int, NodeThresholds> nodeThresholds;
    QMap<int, QStringList>& getNodeSpecificLogs(); // 声明获取节点特定日志的方法
    QMap<int, QMap<ParameterType, QStringList>> parameterLogs;

    QTimer* thresholdCheckTimer; // 定时检查阈值的计时器
    bool suppressAlerts = false; // 临时禁用警报（用于初始化时避免误报）

    QString currentSearchKeyword; // 当前搜索关键字
    ParameterType currentParameterType = ParameterType::All; // 当前筛选的参数类型
    QLineEdit *searchBox; // 搜索框指针（假设已在UI设计师中命名为lineEditSearch）

    const double* getCurrentCurveData(int nodeIndex) const;
    void addNodeLog(int nodeIndex, const QString& logMessage, ParameterType parameterType);
    QString getParameterName(ParameterType type) const;

    void applyLogFilters();
    void on_comboBox_currentIndexChanged(int index);//ComboBox选项变化事件
    void on_comboBoxParameter_currentIndexChanged(int index);
    double getCurrentValue(int nodeIndex, const QString &parameter) const;
    double getThresholdValue(int nodeIndex, const QString &parameter) const;

    //Back
    WorkerThread* workerThread;

    ParameterType getParameterTypeFromData(const Lora::LoraData &data);
    // 新增：日志显示更新函数
    void updateLogDisplay();
    QMap<int, QStringList> allLogs;

    QList<PointInfo> pointData;  // 存储所有数据点信息
    int hoveredPointIndex = -1;  // 当前悬停的数据点索引
    QStringList systemLogs;
};


class SuccessDialog : public QDialog
{
public:
    SuccessDialog(QWidget *parent = nullptr);
    void setMessage(const QString &message);
private:
    QPushButton *okButton;
    QPushButton *restoreButton;
    QLabel *messageLabel;
};

#endif // MAINWINDOW_H
