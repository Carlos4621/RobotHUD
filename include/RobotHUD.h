#ifndef ROBOTHUD_H
#define ROBOTHUD_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <SDL2/SDL.h>
#include "Controller.h"
#include "EasyLoRa_Widget.h"

QT_BEGIN_NAMESPACE
class QWidget;
namespace Ui { class RobotHUD; }
QT_END_NAMESPACE

class RobotHUD : public QMainWindow
{
    Q_OBJECT

public:
    RobotHUD(QWidget *parent = nullptr);
    ~RobotHUD();

private slots:
    void testController();

    void onPushOpenLoRaMenu();

private:
    Ui::RobotHUD *ui;

    QTimer* testTimer_m;
    Controller* controller_m;
    QDialog* LoRaConfigurationMenu_m;
    EasyLoRa_Widget* easyLoRaWidget_m;

    void setupLoRaMenuDialog();
};

#endif // ROBOTHUD_H
