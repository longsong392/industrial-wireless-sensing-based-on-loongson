#include "esp8266.h"
#include <sstream>
#include <iomanip>
#include <QDebug>
#include <QElapsedTimer>

ESP8266::ESP8266(const QString &portName,QObject *parent) : QObject(parent)
    , esp_serial(new QSerialPort(this)) {
    esp_serial.setPortName(portName);
}

bool ESP8266::open(int baudRate){
    if (!esp_serial.open(QIODevice::ReadWrite)) {
        qWarning() << "Failed to open port:" << esp_serial.errorString();
        return false;
    }
    esp_serial.setBaudRate(baudRate);
    esp_serial.setDataBits(QSerialPort::Data8);
    esp_serial.setStopBits(QSerialPort::OneStop);
    esp_serial.setParity(QSerialPort::NoParity);

    esp_serial.clear();

    opened = true;

    return true;
}

bool ESP8266::isOpen() const
{
    return opened;
}

bool ESP8266::isConnected() const
{
    return connected_;
}

bool ESP8266::sendATCommand(const std::string& command,const std::string& expected,int timeout){
    if (!esp_serial.isOpen()) {
        qDebug() << "SerialPort is NOT Opened !";
        return false;
    }
    esp_serial.clear(QSerialPort::Input);

    QByteArray data(command.data(), command.size());
    esp_serial.write(data);

    QByteArray response;
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < timeout) {
        if (esp_serial.waitForReadyRead(100)) { // 每100ms检查一次
            response += esp_serial.readAll();

            // 检查是否收到完整响应
            if (response.contains(expected.c_str())) {
                qDebug() << "Got Expected Response :" << response;
                return true;
            }

            // 检查错误响应
            if (response.contains("ERROR") || response.contains("FAIL")) {
                qDebug() << "Got Error Response :" << response;
                return false;
            }
        }
    }
    qDebug() << "TIMEOUT, Partial Response :" << response;
    return false;
}



bool ESP8266::connectWiFi(const std::string& ssid, const std::string& password) {
    if (!sendATCommand("AT\r\n","OK",30)) {
        qDebug() << "No Response to :" << "AT";
        return false;
    }

    if (!sendATCommand("AT+RST\r\n","ready",500)){
        qDebug() << "No Response to :" << "AT";
        return false;
    }

    if (!sendATCommand("ATE0\r\n","OK",30)){
        qDebug() << "No Response to :" << "AT";
        return false;
    }

    if (!sendATCommand("AT+CWMODE=1\r\n","OK",30)) {
        qDebug() << "No Response to :" << "AT+CWMODE=1";
        return false;
    }
    std::string WIFIisconnected = "AT+CIPSTATUS\r\n";
    std::string connectWIFI = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"\r\n";
    if(!sendATCommand(connectWIFI,"OK",3000)){
        if(!sendATCommand(connectWIFI,"OK",3000)){
            if(!sendATCommand(connectWIFI,"OK",3000)){
                qDebug() << "Failed to Connect to WiFi !";
                return false;
            }
        }
    }
    return true;
}

bool ESP8266::connectOneNet(const std::string& device_name,
                            const std::string& product_id,
                            const std::string& auth_key) {

    //  Config MQTT
    std::string configMQTT = "AT+MQTTUSERCFG=0,1,\"" + device_name + "\",\"" + product_id + "\",\"" + auth_key + "\",0,0,\"\"\r\n";
    if (!sendATCommand(configMQTT,"OK",3000)) {
        if (!sendATCommand(configMQTT,"OK",3000)) {
            if (!sendATCommand(configMQTT,"OK",3000)) {
                qDebug() << "Failed to Config to MQTT !" ;
                return false;
            }
        }
    }

    // Connect MQTT
    std::string connectMQTT = "AT+MQTTCONN=0,\"183.230.40.96\",1883,1\r\n";
    if (!sendATCommand(connectMQTT,"OK",3000)) {
        if (!sendATCommand(connectMQTT,"OK",3000)) {
            if (!sendATCommand(connectMQTT,"OK",3000)) {
                qDebug() << "Failed to Connect to MQTT !" ;
                return false;
            }
        }
    }

    std::string topic = "$sys/" + product_id + "/" + device_name + "/thing/property/post/reply";
    std::string subscribeTopic = "AT+MQTTSUB=0,\"" + topic + "\",1\r\n";
    if (!sendATCommand(subscribeTopic,"OK",3000)) {
        if (!sendATCommand(subscribeTopic,"OK",3000)) {
            if (!sendATCommand(subscribeTopic,"OK",3000)) {
                qDebug() << "Failed to Subscribe Topic !" ;
                return false;
            }
        }
    }
    connected_ =true;
    return true;
}

bool ESP8266::upload_data_node1(
    const std::string& product_id,
    const std::string& device_name,
    const Lora::LoraData& data) {

    std::ostringstream json;
    json << std::fixed << std::setprecision(1)
         << "{\\\"id\\\":\\\"123\\\"\\\,\\\"params\\\":{"
         << "\\\"temp_1\\\":{\\\"value\\\":" << data.temp<< "}\\\,"
         << "\\\"hum_1\\\":{\\\"value\\\":" << data.hum<< "}\\\,"
         << "\\\"light_1\\\":{\\\"value\\\":" << data.light << "}\\\,"
         << "\\\"gas_1\\\":{\\\"value\\\":" << data.gas << "}}}";


    std::string topic = "$sys/" + product_id + "/" + device_name + "/thing/property/post";
    std::string cmd = "AT+MQTTPUB=0,\"" + topic + "\",\"" + json.str() + "\",0,0\r\n";

    return sendATCommand(cmd, "OK", 3000);
}

bool ESP8266::upload_data_node2(
    const std::string& product_id,
    const std::string& device_name,
    const Lora::LoraData& data) {

    std::ostringstream json;
    json << std::fixed << std::setprecision(1)
         << "{\\\"id\\\":\\\"123\\\"\\\,\\\"params\\\":{"
         << "\\\"temp_2\\\":{\\\"value\\\":" << data.temp<< "}\\\,"
         << "\\\"hum_2\\\":{\\\"value\\\":" << data.hum<< "}\\\,"
         << "\\\"light_2\\\":{\\\"value\\\":" << data.light << "}\\\,"
         << "\\\"gas_2\\\":{\\\"value\\\":" << data.gas << "}}}";


    std::string topic = "$sys/" + product_id + "/" + device_name + "/thing/property/post";
    std::string cmd = "AT+MQTTPUB=0,\"" + topic + "\",\"" + json.str() + "\",0,0\r\n";

    return sendATCommand(cmd, "OK", 3000);
}

bool ESP8266::upload_data_node3(
    const std::string& product_id,
    const std::string& device_name,
    const Lora::LoraData& data) {

    std::ostringstream json;
    json << std::fixed << std::setprecision(1)
         << "{\\\"id\\\":\\\"123\\\"\\\,\\\"params\\\":{"
         << "\\\"temp_3\\\":{\\\"value\\\":" << data.temp<< "}\\\,"
         << "\\\"hum_3\\\":{\\\"value\\\":" << data.hum<< "}\\\,"
         << "\\\"light_3\\\":{\\\"value\\\":" << data.light << "}\\\,"
         << "\\\"gas_3\\\":{\\\"value\\\":" << data.gas << "}}}";


    std::string topic = "$sys/" + product_id + "/" + device_name + "/thing/property/post";
    std::string cmd = "AT+MQTTPUB=0,\"" + topic + "\",\"" + json.str() + "\",0,0\r\n";

    return sendATCommand(cmd, "OK", 3000);
}
