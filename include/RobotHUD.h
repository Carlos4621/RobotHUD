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
    void testController();
    void startTestTimer();

    void onLoRaConnected();
    void onUDPConnected();

private:
    Ui::RobotHUD *ui;

    QTimer* testTimer_m;
    Controller* controller_m;
    EasyLoRa_Widget* easyLoRaWidget_m;
    EasyUDP_Widget* easyUDPWidget_m;

    std::shared_ptr<IOCommons> device_m{ nullptr };
};

#endif // ROBOTHUD_H
