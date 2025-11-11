#ifndef ROBOTHUD_H
#define ROBOTHUD_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <SDL2/SDL.h>
#include "Controller.h"
#include "EasyLoRa_Widget.h"
#include "EasyUDP_Widget.h"

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
    void onStartButtonPressed();

    void onLoRaConnected();
    void onUDPConnected();

    void onControllerConected();
    void onControllerDisconected();

    void onDeviceConected();
    void onDeviceDisconnected();

private:
    static constexpr std::chrono::milliseconds Send_Interval{ 100 };
    static constexpr uint8_t First_Controller_ID{ 0 };

    Ui::RobotHUD *ui;

    QTimer* sendTimer_m;
    Controller* controller_m;
    EasyLoRa_Widget* easyLoRaWidget_m;
    EasyUDP_Widget* easyUDPWidget_m;

    std::shared_ptr<IOCommons> device_m{ nullptr };

    bool controllerConnected_m{ false };
    bool deviceConnected_m{ false };
};

#endif // ROBOTHUD_H
