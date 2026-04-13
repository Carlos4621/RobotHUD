#include "RobotHUD.h"
#include "ui_RobotHUD.h"
#include "EasyLoRa_Widget.h"
#include "EasyUDP_Widget.h"
#include "MatLabel.h"
#include "VideoRecognizer_Widget.h"
#include "MultimediaData.pb.h"
#include <opencv2/imgcodecs.hpp>
#include <vector>

RobotHUD::RobotHUD(QWidget *parent)
: QMainWindow{ parent }
, ui{ new Ui::RobotHUD{} }
, controllerWidget_m{ new Controller_Widget{this} }
, sendTimer_m{ new QTimer{this} }
, wifiImageTimer_m{ new QTimer{this} }
, easyLoRaWidget_m{ new EasyLoRa_Widget{this} }
, easyUDPWidget_m{ new EasyUDP_Widget{this} }
, videoRecognizerWidget_m{ nullptr }
{
    ui->setupUi(this);

    videoRecognizerWidget_m = new VideoRecognizer_Widget{
        this,
        ui->mainCameraLabel,
        ui->secondaryCameraLabel,
        ui->detectQR,
        ui->detectMovement,
        ui->detectHazmat,
        ui->QROutput,
        ui->movementOutput,
        ui->hazmatOutput
    };

    sendTimer_m->setInterval(Send_Interval);
    wifiImageTimer_m->setInterval(Image_Interval_ms);

    connect(ui->actionLoRa, &QAction::triggered, easyLoRaWidget_m, &QDialog::exec);
    connect(ui->actionWiFi, &QAction::triggered, easyUDPWidget_m, &QDialog::exec);
    connect(ui->actionControl, &QAction::triggered, controllerWidget_m, &QDialog::exec);
    connect(ui->actionReconocimiento, &QAction::triggered, videoRecognizerWidget_m, &VideoRecognizer_Widget::showConfigurationDialog);

    connect(ui->startButton, &QPushButton::pressed, this, &RobotHUD::onStartButtonPressed);
    connect(sendTimer_m, &QTimer::timeout, this, &RobotHUD::sendMessage);
    connect(wifiImageTimer_m, &QTimer::timeout, this, &RobotHUD::receiveWiFiImage);

    connect(easyLoRaWidget_m, &EasyLoRa_Widget::succesfulConnection, this, &RobotHUD::onLoRaConnected);
    connect(easyUDPWidget_m, &EasyUDP_Widget::succesfulConnection, this, &RobotHUD::onUDPConnected);

    connect(controllerWidget_m, &Controller_Widget::controllerConnected, this, &RobotHUD::onControllerConected);
    connect(controllerWidget_m, &Controller_Widget::controllerDisconnected, this, &RobotHUD::onControllerDisconected);

    connect(ui->mainCameraLabel, &MatLabel::rightClicked, videoRecognizerWidget_m, &VideoRecognizer_Widget::swapCameraViews);
    connect(ui->secondaryCameraLabel, &MatLabel::rightClicked, videoRecognizerWidget_m, &VideoRecognizer_Widget::swapCameraViews);
}

RobotHUD::~RobotHUD() {
    delete ui;
}

void RobotHUD::onStartButtonPressed() {
    if (controllerConnected_m) {
        sendTimer_m->start();
        ui->startButton->setEnabled(false);
    }
}

void RobotHUD::onLoRaConnected() {
    LoRaConnected_m = true;
    LoRaDevice_m = easyLoRaWidget_m->getDevice();
    ui->labelLoRaStatus->setText("Connected!");
}

void RobotHUD::onUDPConnected() {
    WiFiConnected_m = true;
    UDP_Device_m = easyUDPWidget_m->getDevice();
    ui->labelWiFiStatus->setText("Connected!");
    wifiImageTimer_m->start();
}

void RobotHUD::onControllerConected() {
    controllerConnected_m = true;
    controller_m = controllerWidget_m->getDevice();
    ui->labelControllerStatus->setText("Connected!");
}

void RobotHUD::onControllerDisconected() {
    QMessageBox::warning(this, "Control desconectado", "Por favor, conecta el control");

    controllerConnected_m = false;
    ui->labelControllerStatus->setText("No connection");
}

void RobotHUD::onLoRaDisconnected() {
    QMessageBox::warning(this, "LoRa desconectado", "Por favor, conecta el dispositivo");

    LoRaConnected_m = false;
    ui->labelLoRaStatus->setText("No connection");
}

void RobotHUD::onWiFiDisconnected() {
    QMessageBox::warning(this, "WiFi desconectado", "Por favor, intente rehacer la conexión");

    WiFiConnected_m = false;
    wifiImageTimer_m->stop();
    ui->labelWiFiStatus->setText("No connection");
}

void RobotHUD::sendMessage() {
    try {
        const auto data{ controller_m->getData() };

        std::string toSend;
        if (!data.SerializeToString(&toSend)) {
            throw std::runtime_error{ "DesearilizeError" };
        }
        
        qDebug() << data.leftjoystick().y_axis() << "|" << data.rightjoystick().y_axis();

        LoRaDevice_m->sendData(toSend);
    }
    catch(const std::exception& e) {
        qDebug() << e.what();
    }
}

void RobotHUD::receiveWiFiImage() {
    if (!WiFiConnected_m || !UDP_Device_m) {
        return;
    }

    try {
        const auto payload{ UDP_Device_m->receiveData() };

        if (payload.empty()) {
            return;
        }

        MultimediaData packet;
        if (!packet.ParseFromString(payload)) {
            return;
        }

        const auto decodeImage = [](const std::string& data) -> cv::Mat {
            if (data.empty()) {
                return {};
            }

            std::vector<unsigned char> buffer(data.begin(), data.end());
            return cv::imdecode(buffer, cv::IMREAD_GRAYSCALE);
        };

        const auto frame{ decodeImage(packet.maincamera()) };
        const auto secondaryFrame{ decodeImage(packet.secondarycamera()) };

        videoRecognizerWidget_m->submitFrames(frame, secondaryFrame);
    }
    catch (const std::exception& e) {
        qDebug() << e.what();
    }
}