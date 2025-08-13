#include "RobotHUD.h"
#include "ui_RobotHUD.h"
#include "EasyLoRa_Widget.h"
#include "EasyUDP_Widget.h"

RobotHUD::RobotHUD(QWidget *parent)
: QMainWindow{ parent }
, ui{ new Ui::RobotHUD{} }
, controller_m{ new Controller{this} }
, testTimer_m{ new QTimer{this} }
, easyLoRaWidget_m{ new EasyLoRa_Widget{this} }
, easyUDPWidget_m{ new EasyUDP_Widget{this} }
{
    ui->setupUi(this);
    testTimer_m->setInterval(100);

    controller_m->setID(0);

    connect(ui->actionLoRa, &QAction::triggered, easyLoRaWidget_m, &QDialog::exec);

    connect(ui->actionWiFi, &QAction::triggered, easyUDPWidget_m, &QDialog::exec);

    connect(ui->buttonTest, &QPushButton::pressed, this, &RobotHUD::startTestTimer);
    connect(testTimer_m, &QTimer::timeout, this, &RobotHUD::testController);
}

RobotHUD::~RobotHUD() {
    delete ui;
}

void RobotHUD::startTestTimer() {
    testTimer_m->start();
}

void RobotHUD::testController() {
    const auto controllerData{ controller_m->getData() };

    std::string toSend;
    controllerData.SerializeToString(&toSend);

    easyUDPWidget_m->getDevice()->sendData(toSend);
}