#ifndef LORA_H
#define LORA_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QMetaType>

class Lora: public QObject {
    Q_OBJECT
public:

    struct LoraData {
        uint8_t nodeId;
        float temp;
        int hum;
        int light;
        int gas;

        LoraData() : nodeId(0), temp(0), hum(0), light(0), gas(0) {}

        LoraData(uint8_t id, float t, int h, int l, int g)
            : nodeId(id), temp(t), hum(h), light(l), gas(g) {}
    };

    enum LoraState {
        IDLE,
        POLLING,
        SENDING
    };

    Lora(const QString &portName,QObject *parent = nullptr);
    ~Lora();

    bool open(int baudRate);
    bool isOpen() const;
    void write(const QString &message);

    void pollNode(int nodeId);
    void pausePolling();
    void restartPolling();

    LoraState getCurrentState() const;
signals:
    void dataReceived(const std::vector<unsigned char> &data);
    void dataUpdated(const LoraData &data);
    void ErrorUpload(const QString &message);

    void requestPausePolling();
    void requestRestartPolling();
private slots:
    void handleRawData(const std::vector<unsigned char> &rawData);
    void processBuffer();  // 缓冲区处理

private:
    QSerialPort lora_serial;
    QTimer* bufferTimer;  // 定时器
    std::vector<unsigned char> dataBuffer;  // 数据缓冲区

    unsigned char key[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
    unsigned char iv[16] = { 0 };
    LoraData parse(const std::vector<unsigned char> &data);

    QTimer* pollTimer;
    int currentNode = 1; // 当前轮询的节点
    int totalNodes = 3;

    LoraState currentState;

    bool opened = false;
};

Q_DECLARE_METATYPE(Lora::LoraData)
#endif // LORA_H

