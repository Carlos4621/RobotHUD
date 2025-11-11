#include "RobotHUD.h"
#include "ui_RobotHUD.h"
#include "EasyLoRa_Widget.h"
#include "EasyUDP_Widget.h"

RobotHUD::RobotHUD(QWidget *parent)
: QMainWindow{ parent }
, ui{ new Ui::RobotHUD{} }
, controller_m{ new Controller{this} }
, sendTimer_m{ new QTimer{this} }
, easyLoRaWidget_m{ new EasyLoRa_Widget{this} }
, easyUDPWidget_m{ new EasyUDP_Widget{this} }
{
    ui->setupUi(this);

    sendTimer_m->setInterval(Send_Interval);

    connect(ui->actionLoRa, &QAction::triggered, easyLoRaWidget_m, &QDialog::exec);
    connect(ui->actionWiFi, &QAction::triggered, easyUDPWidget_m, &QDialog::exec);

    connect(ui->startButton, &QPushButton::pressed, this, &RobotHUD::onStartButtonPressed);
    connect(sendTimer_m, &QTimer::timeout, this, &RobotHUD::sendMessage);

    connect(easyLoRaWidget_m, &EasyLoRa_Widget::succesfulConnection, this, &RobotHUD::onLoRaConnected);
    connect(easyUDPWidget_m, &EasyUDP_Widget::succesfulConnection, this, &RobotHUD::onUDPConnected);

    connect(controller_m, &Controller::controllerConnected, this, &RobotHUD::onControllerConected);
    connect(controller_m, &Controller::controllerDisconnected, this, &RobotHUD::onControllerDisconected);

    connect(easyLoRaWidget_m, &EasyLoRa_Widget::deviceDisconnected, this, &RobotHUD::onDeviceDisconnected);

    controller_m->setID(First_Controller_ID);
}

RobotHUD::~RobotHUD() {
    delete ui;
}

void RobotHUD::onStartButtonPressed() {
    if (controllerConnected_m && deviceConnected_m) {
        sendTimer_m->start();
        ui->startButton->setEnabled(false);
    }
}

void RobotHUD::onLoRaConnected() {
    device_m = easyLoRaWidget_m->getDevice();
    ui->labelDisplayConnection->setText("LoRa");
    onDeviceConected();
}

void RobotHUD::onUDPConnected() {
    device_m = easyUDPWidget_m->getDevice();
    ui->labelDisplayConnection->setText("WiFi");
    onDeviceConected();
}

void RobotHUD::onControllerConected() {
    controllerConnected_m = true;

    if (deviceConnected_m && !sendTimer_m->isActive()) {
        ui->startButton->setEnabled(true);
    }
}

void RobotHUD::onControllerDisconected() {
    QMessageBox::warning(this, "Control desconectado", "Por favor, conecta el control");

    sendTimer_m->stop();
    ui->startButton->setEnabled(false);
    controllerConnected_m = false;
}

void RobotHUD::onDeviceConected() {
    deviceConnected_m = true;

    if (controllerConnected_m && !sendTimer_m->isActive()) {
        ui->startButton->setEnabled(true);
    }
}

void RobotHUD::onDeviceDisconnected() {
    QMessageBox::warning(this, "Dispositivo desconectado", "Por favor, conecte el dispositivo");

    sendTimer_m->stop();
    ui->startButton->setEnabled(false);
    deviceConnected_m = false;
}

void RobotHUD::sendMessage() {
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