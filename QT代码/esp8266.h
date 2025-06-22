#ifndef ESP8266_H
#define ESP8266_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include "lora.h"
class ESP8266: public QObject {
    Q_OBJECT
public:
    ESP8266(const QString &portName,QObject *parent = nullptr);
    bool open(int baudRate);
    bool isOpen() const;
    bool isConnected() const;
    bool sendATCommand(const std::string& command,const std::string& expected ,int timeout=1000);

    bool connectWiFi(const std::string& ssid,const std::string& password);
    bool connectOneNet(const std::string& device_name,const std::string& product_id,const std::string& auth_key);

    bool upload_data_node1(const std::string& product_id,const std::string& device_name,const Lora::LoraData& data);
    bool upload_data_node2(const std::string& product_id,const std::string& device_name,const Lora::LoraData& data);
    bool upload_data_node3(const std::string& product_id,const std::string& device_name,const Lora::LoraData& data);

private:
    QSerialPort esp_serial;
    bool connected_ = false;
    bool opened = false;
};

#endif // ESP8266_H
