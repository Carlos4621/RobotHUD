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

class RobotHUD : public QMainWindow
{
    Q_OBJECT

public:
    explicit RobotHUD(QWidget *parent = nullptr);
    ~RobotHUD() noexcept;

private slots:
    void testController();
    void startTestTimer();

private:
    Ui::RobotHUD *ui;

    QTimer* testTimer_m;
    Controller* controller_m;
    EasyLoRa_Widget* easyLoRaWidget_m;
    EasyUDP_Widget* easyUDPWidget_m;
};

#endif // ROBOTHUD_H
