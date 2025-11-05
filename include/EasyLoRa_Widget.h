#ifndef EASYLORA_WIDGET_H
#define EASYLORA_WIDGET_H

#include <QtWidgets/QWidget>
#include "EasyLoRa.hpp"
#include <memory>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDialog>
#include <QTimer>

QT_BEGIN_NAMESPACE
class Ui_EasyLoRa_Widget;
QT_END_NAMESPACE

/// @brief Widget que permite la conexión y configuración del módulo EasyLoRa
class EasyLoRa_Widget : public QDialog
{
    Q_OBJECT

public:
    /// @brief Cosntructor base
    /// @param parent Padre del widget
    explicit EasyLoRa_Widget(QWidget *parent = nullptr);
    
    ~EasyLoRa_Widget();

    /// @brief Obtiene el dispositivo conectado, debe ser usado junto a la señal succesfulConnection para evitar punteros nulos
    /// @return El dispositivo conectado actualmente, es null en caso de no haber dispositivo conectado
    std::shared_ptr<EasyLoRa> getDevice() const noexcept;

signals:
    void succesfulConnection();
    void deviceDisconnected();

private slots:
    void onPushConnect();
    void onPushApply();

private:
    static constexpr uint8_t Byte_Shift{ 8 };
    static constexpr uint8_t HEX_Base{ 16 };
    static constexpr uint8_t Low_Address_Mask{ 0xFF };
    static constexpr std::chrono::milliseconds Port_Verification_Interval{ 100 };

    Ui_EasyLoRa_Widget *ui;

    std::shared_ptr<EasyLoRa> easyLoRa_m;

    QTimer* portVerificationTimer_m;
    QString connectedPort_m;

    void showErrorMessage(std::string_view errorMessage);
    void refreshConfiguration();

    void setAddress(const ModuleConfig& configuration);
    void setChannel(const ModuleConfig& configuration);
    void setNETID(const ModuleConfig& configuration);
    void setBaudRate(const ModuleConfig& configuration);
    void setParityByte(const ModuleConfig& configuration);
    void setAirRate(const ModuleConfig& configuration);
    void setSubpacketLength(const ModuleConfig& configuration);
    void setWORCycle(const ModuleConfig& configuration);
    void setRelayMode(const ModuleConfig& configuration);
    void setWORMode(const ModuleConfig& configuration);
    void setFixedTransmission(const ModuleConfig& configuration);
    void setRSSIEnabled(const ModuleConfig& configuration);
    void setLBTEnabled(const ModuleConfig& configuration);
    void setEnableRSSIByte(const ModuleConfig& configuration);
    void setEnableAbnormalLogs(const ModuleConfig& configuration);

    uint8_t getHighAddress();
    uint8_t getLowAddress();
    uint8_t getChannel();
    uint8_t getNETID();
    UARTBaudRate getBaudRate();
    SerialPortParityByte getParityByte();
    AirDataRate getAirRate();
    SubpacketLenght getSubpacketLength();
    WORCycle getWORCycle();
    bool getWORMode();
    bool getRelayMode();
    bool getFixedTransmission();
    bool getRSSIEnabled();
    bool getLBTEnabled();
    bool getEnableRSSIByte();
    bool getEnableAbnormalLogs();

    void listAvalaiblePorts();

    ModuleConfig getBuiltConfiguration();

    void disconnectDevice();
};

#endif // EASYLORA_WIDGET_H
