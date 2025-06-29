#include "lora.h"
#include "sm4.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <QDebug>
#include <QTimer>


Lora::Lora(const QString &portName, QObject *parent)
    : QObject(parent), lora_serial() {
    lora_serial.setPortName(portName);

    // 初始化缓存区接受定时器
    bufferTimer = new QTimer(this);
    bufferTimer->setInterval(2000); // 2秒超时
    bufferTimer->setSingleShot(true); // 单次触发
    connect(bufferTimer, &QTimer::timeout, this, &Lora::processBuffer);

    //初始化轮询定时器
    pollTimer = new QTimer(this);
    pollTimer->setInterval(4000); // 每2秒轮询一个节点
    connect(pollTimer, &QTimer::timeout, this, [this]() {
        pollNode(currentNode);
        currentNode = (currentNode % totalNodes) + 1; // 循环1-3
    });

    connect(this, &Lora::dataReceived, this, &Lora::handleRawData);
    connect(&lora_serial, &QSerialPort::readyRead, this, [this]() {
        auto data = lora_serial.readAll();
        std::vector<unsigned char> rawData(data.begin(), data.end());
        emit dataReceived(rawData);
    });


}

void Lora::pollNode(int nodeId) {
    if (lora_serial.isOpen()) {
        QString command = QString("%1send").arg(nodeId);
        lora_serial.clear(QSerialPort::Input); // 清空接收缓冲区
        lora_serial.write(command.toUtf8());
        qDebug() << " Polling Node:" << nodeId;
    }

}


// 暂停/恢复轮询函数
void Lora::pausePolling() {
    if (pollTimer->isActive()) {
        pollTimer->stop();

        qDebug() << " Polling Stopped ";
    }
}

void Lora::restartPolling() {
    if (!pollTimer->isActive()) {
        pollTimer->start();

        qDebug() << " Polling Restarted ";
    }
}

bool Lora::open(int baudRate){
    if (!lora_serial.open(QIODevice::ReadWrite)) {
        qWarning() << " Fail to Open SerialPort ：" << lora_serial.errorString();
        return false;
    }
    lora_serial.setBaudRate(baudRate);
    lora_serial.setDataBits(QSerialPort::Data8);
    lora_serial.setStopBits(QSerialPort::OneStop);
    lora_serial.setParity(QSerialPort::NoParity);

    lora_serial.clear();
    /*if (lora_serial.isOpen()) {
            pollTimer->start();
    }*/
    opened =true;
    return true;
}

bool Lora::isOpen() const
{
    return opened;
}

Lora::~Lora() {

    if (pollTimer) {
        pollTimer->stop();
        delete pollTimer;
    }

    if (bufferTimer) {
        bufferTimer->stop();
        delete bufferTimer;
    }

    if (lora_serial.isOpen()) {
        lora_serial.clear();  // 清空缓冲区
        lora_serial.close();  // 关闭串口

    }
}

void Lora::write(const QString &message) {

    if (lora_serial.isOpen()) {
        pausePolling();
        lora_serial.clear(QSerialPort::Input);
        lora_serial.write(message.toUtf8());
        qDebug() << " The Command Send Successfully！";

        QTimer::singleShot(1000, this, [this]() {
            restartPolling();  // 1 秒后恢复轮询
        });
    }

}

//将原始数据添加到缓存区
void Lora::handleRawData(const std::vector<unsigned char> &rawData) {

    // 将新数据添加到缓冲区
    dataBuffer.insert(dataBuffer.end(), rawData.begin(), rawData.end());

    // 重置定时器
    bufferTimer->start();
}


void Lora::processBuffer() {
    if (!dataBuffer.empty()) {

        if (dataBuffer.size() == 3 &&
            dataBuffer[0] == '1' &&
            dataBuffer[1] == 'o' &&
            dataBuffer[2] == 'n') {
            pollTimer->start();
        }

        if (dataBuffer.size() >= 16) {
            LoraData result = parse(dataBuffer);
            if (result.nodeId == 0) {
                qDebug() << " Invalid Length of The Data ！";
            } else {
                emit dataUpdated(result);
            }
        }

        dataBuffer.clear();
    }
}

// 十六进制字符串转换为字节数组
std::vector<unsigned char> hexToBytes(const std::string& hex) {
    std::vector<unsigned char> bytes;

    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

// 去除可能的非十六进制字符
std::string cleanHexString(const std::string& input) {
    std::string result;
    std::copy_if(input.begin(), input.end(), std::back_inserter(result),
                 [](char c) { return isxdigit(c); });

    return result;
}

//解密解析数据
Lora::LoraData Lora::parse(const std::vector<unsigned char> &data) {
    LoraData result{};
    // 只保留十六进制字符
    std::string hexStr(data.begin(), data.end());
    hexStr = cleanHexString(hexStr);

    std::cout << "Received : " << hexStr << std::endl;
    // 检查长度（加密数据是16字节的倍数，十六进制是32字符倍数）
    if (hexStr.length() % 32 != 0) {
        std::cerr << " 不合理密文长度 ：" << hexStr.length() << std::endl;
        return result;
    }

    // 将十六进制字符串转换为字节数组
    std::vector<unsigned char> encryptedData = hexToBytes(hexStr);

    std::vector<unsigned char> decrypted(encryptedData.size());

    size_t decryptedLen = encryptedData.size();
    memset(iv, 0, sizeof(iv));  // 重置IV
    SM4_Dec(encryptedData.data(), decrypted.data(), &decryptedLen, iv, key);

    // 移除可能的填充
    if (decryptedLen > 0 && decryptedLen < decrypted.size()) {
        decrypted.resize(decryptedLen);
    }

    // 转换为字符串
    std::string decryptedStr(decrypted.begin(), decrypted.end());
    std::cout << "Decrypted :" << decryptedStr << std::endl;
    // 解析数据
    std::istringstream iss(decryptedStr);

    int nodeId, temp, hum, light, gas;

    iss >> nodeId >>  temp >>  hum >>  light  >> gas;

    result.nodeId = nodeId;
    result.temp = temp/10.0;
    result.hum = hum;
    result.light = light;
    result.gas = gas;
    return result;
}

