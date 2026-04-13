#ifndef ROBOTHUD_H
#define ROBOTHUD_H

#include <QMainWindow>
#include <QTimer>
#include "Controller.h"
#include "EasyLoRa_Widget.h"
#include "EasyUDP_Widget.h"
#include "Controller_Widget.h"
#include "VideoRecognizer_Widget.h"

QT_BEGIN_NAMESPACE
class QWidget;
namespace Ui { class RobotHUD; }
QT_END_NAMESPACE

/// @brief Clase principal del programa
class RobotHUD : public QMainWindow
{
    Q_OBJECT

public:

    /// @brief Constructor del widget
    /// @param parent Padre del widget
    explicit RobotHUD(QWidget *parent = nullptr);

    ~RobotHUD() noexcept;

private slots:
    void sendMessage();
    void receiveWiFiImage();
    void onStartButtonPressed();

    void onLoRaConnected();
    void onUDPConnected();

    void onControllerConected();
    void onControllerDisconected();

    void onLoRaDisconnected();
    void onWiFiDisconnected();

private:
    static constexpr std::chrono::milliseconds Send_Interval{ 100 };
    static constexpr int Image_Interval_ms{ 33 };
    static constexpr uint8_t First_Controller_ID{ 0 };

    Ui::RobotHUD *ui;

    QTimer* sendTimer_m;
    QTimer* LoRaSendTimer_m;
    QTimer* wifiImageTimer_m;

    VideoRecognizer_Widget* videoRecognizerWidget_m;

    Controller_Widget* controllerWidget_m;
    EasyLoRa_Widget* easyLoRaWidget_m;
    EasyUDP_Widget* easyUDPWidget_m;

    std::shared_ptr<EasyLoRa> LoRaDevice_m;
    std::shared_ptr<EasyUDP> UDP_Device_m;
    std::shared_ptr<Controller> controller_m;

    bool controllerConnected_m{ false };;

    bool LoRaConnected_m{ false };
    bool WiFiConnected_m{ false };
};

#endif // ROBOTHUD_H
