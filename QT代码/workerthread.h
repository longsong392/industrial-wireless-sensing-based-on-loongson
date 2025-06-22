#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include <QThread>
#include "lora.h"
#include "esp8266.h"

class WorkerThread : public QObject
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = nullptr);
    ~WorkerThread();

    void setPortNames(const QString &loraPort, const QString &espPort);
    void start();
    void stop();

    // 安全接口，MainWindow可以请求写入 Lora 数据
    Q_INVOKABLE void slotWriteToLora(const QString &message);

signals:
    void dataReceived(const Lora::LoraData &data);
    void uploadStatus(const QString &message);
    void errorOccurred(const QString &error);

private:
    QThread *m_thread;
    Lora *lora;
    ESP8266 *esp;
    QString loraPortName;
    QString espPortName;
    bool running;

    void process();
    void handleLoraData(const Lora::LoraData &data);
};

#endif // WORKERTHREAD_H
