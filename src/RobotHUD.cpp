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

    controller_m->setID(0);
    testTimer_m->setInterval(100);

    connect(ui->actionLoRa, &QAction::triggered, easyLoRaWidget_m, &QDialog::exec);

    connect(ui->actionWiFi, &QAction::triggered, easyUDPWidget_m, &QDialog::exec);

    connect(ui->buttonTest, &QPushButton::pressed, this, &RobotHUD::startTestTimer);
    connect(testTimer_m, &QTimer::timeout, this, &RobotHUD::testController);

    connect(easyLoRaWidget_m, &EasyLoRa_Widget::succesfulConnection, this, &RobotHUD::onLoRaConnected);
    connect(easyUDPWidget_m, &EasyUDP_Widget::succesfulConnection, this, &RobotHUD::onUDPConnected);
}

RobotHUD::~RobotHUD() {
    delete ui;
}

void RobotHUD::startTestTimer() {
    testTimer_m->start();
}

void RobotHUD::onLoRaConnected() {
    device_m = easyLoRaWidget_m->getDevice();
    ui->labelDisplayConnection->setText("LoRa");
}

void RobotHUD::onUDPConnected() {
    device_m = easyUDPWidget_m->getDevice();
    ui->labelDisplayConnection->setText("WiFi");
}

void RobotHUD::testController() {
    try {

        const auto data{ controller_m->getData() };

        std::string toSend;
        if (!data.SerializeToString(&toSend)) {
            throw std::runtime_error{ "DesearilizeError" };
        }
        
        qDebug() << data.leftjoystick().y_axis() << "|" << data.rightjoystick().y_axis();

        device_m->sendData(toSend);
    }
    catch(const std::exception& e) {
        qDebug() << e.what();
    }
}