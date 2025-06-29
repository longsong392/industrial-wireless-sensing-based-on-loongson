#include "workerthread.h"
#include <QDebug>
#include <QCoreApplication>

const char* ssid = "Loongnix";
const char* password = "loongson";
const char* device_name = "Loongson";
const char* product_id = "pS6FUN5xyz";
const char* auth_key = "version=2018-10-31&res=products%2FpS6FUN5xyz&et=1808369180&method=sha1&sign=8DkCU%2FpCwspRwvwslfpCrAM2BZY%3D";


WorkerThread::WorkerThread(QObject *parent) :
    QObject(nullptr),
    m_thread(new QThread(this)),
    lora(nullptr),
    esp(nullptr),
    running(false)
{
    this->moveToThread(m_thread);

    // 连接线程启动信号
    connect(m_thread, &QThread::started, this, &WorkerThread::process);

    // 启动线程（但不立即运行，等待 start() 调用）

}

WorkerThread::~WorkerThread()
{
    stop();
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
}

void WorkerThread::setPortNames(const QString &loraPort, const QString &espPort)
{
    loraPortName = loraPort;
    espPortName = espPort;
}

void WorkerThread::start()
{
    if (!running) {
        running = true;
        m_thread->start(); // 启动线程
    }
}

void WorkerThread::stop()
{
    running = false;  // 停止主循环
}

void WorkerThread::process()
{
    qDebug() << "WorkerThread running in thread:" << QThread::currentThread();

    // 初始化设备
    lora = new Lora(loraPortName);
    esp = new ESP8266(espPortName);

    lora->moveToThread(QThread::currentThread()); // ✅ Lora 被移动到当前线程（WorkerThread）
    esp->moveToThread(QThread::currentThread());

    // 打开设备
    if (!lora->open(115200)) {
        if (!lora->open(115200)) {
            if (!lora->open(115200)) {
                emit errorOccurred("无法打开LoRa串口");
                //return;
            }
        }
    }
    else {emit uploadStatus("Lora初始化完成");}

    if (!esp->open(115200)) {
        if (!esp->open(115200)) {
            if (!esp->open(115200)) {
                emit errorOccurred("无法打开ESP8266串口");
                //return;
            }
        }
    }
    else {emit uploadStatus("ESP8266初始化完成");}

    if(esp->isOpen() && lora->isOpen()){
        emit uploadStatus("所有设备将初始化完成");
    }

    // 连接WiFi和OneNET
    if (!esp->connectWiFi(ssid, password)) {
        emit errorOccurred("WiFi连接失败");
        //return;
    }
    else {emit uploadStatus("ESP8266连接WIFI成功");}

    if (!esp->connectOneNet(device_name, product_id,auth_key)) {
        emit errorOccurred("Onenet连接失败");
        //return;
    }
    else {emit uploadStatus("ESP8266连接Onenet成功");}


    // 连接LoRa数据信号
    connect(lora, &Lora::dataUpdated, this, &WorkerThread::handleLoraData, Qt::QueuedConnection);
    connect(lora, &Lora::ErrorUpload, this, &WorkerThread::errorOccurred);


    // 主循环
    while (running) {
        QCoreApplication::processEvents();
        QThread::msleep(100);
    }
}

// 用于安全地写入 Lora 数据
void WorkerThread::slotWriteToLora(const QString &message)
{
    if (lora) {
        lora->write(message);
    }
}

void WorkerThread::handleLoraData(const Lora::LoraData &data)
{
    emit dataReceived(data);

    // 上传数据到OneNET
    if (esp && esp->isConnected()) {
        if (data.nodeId == 1) {
            bool success = esp->upload_data_node1(product_id, device_name, data);
            emit uploadStatus(success ? "上传成功" : "上传失败");}
        if (data.nodeId == 2) {
            bool success = esp->upload_data_node1(product_id, device_name, data);
            emit uploadStatus(success ? "上传成功" : "上传失败");}
        if (data.nodeId == 3) {
            bool success = esp->upload_data_node1(product_id, device_name, data);
            emit uploadStatus(success ? "上传成功" : "上传失败");}
    }
}
