#include "EasyUDP_Widget.h"
#include "MultimediaSettings.pb.h"
#include "ui_EasyUDP_Widget.h"
#include <QMessageBox>

EasyUDP_Widget::EasyUDP_Widget(QWidget *parent)
: QDialog{ parent }
, ui{ new Ui::Dialog{} }
, settings_m{ "ControladorRobot", "WiFiSettings" }
{
    ui->setupUi(this);

    loadSettings();

    connect(ui->buttonConnect, &QPushButton::pressed, this, &EasyUDP_Widget::onPushConnect);
    connect(ui->buttonCancel, &QPushButton::pressed, this, &EasyUDP_Widget::close);
    connect(ui->applyButton, &QPushButton::pressed, this, &EasyUDP_Widget::onPushApply);
}

EasyUDP_Widget::~EasyUDP_Widget() noexcept {
    delete ui;
}

void EasyUDP_Widget::onPushConnect() {
    try {
        device_m = std::make_shared<EasyUDP>(ui->lineIP->text().toStdString(), ui->linePort->text().toStdString());
        ui->groupBox->setEnabled(true);
        saveSettings();
        emit succesfulConnection();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString::fromUtf8(e.what()));
    }
}

void EasyUDP_Widget::onPushApply() {
    if (!device_m) {
        QMessageBox::warning(this, "WiFi", "Primero conecta el dispositivo");
        return;
    }

    MultimediaSettings settings;
    settings.set_camerafps(static_cast<uint32_t>(ui->spinMaxFPS->value()));
    settings.set_quality(static_cast<uint32_t>(ui->spinQuality->value()));
    settings.set_grayscale(ui->checkGrayScale->isChecked());
    settings.set_resolution(static_cast<Resolutions>(ui->comboResolution->currentIndex()));

    std::string payload;
    if (!settings.SerializeToString(&payload)) {
        QMessageBox::critical(this, "WiFi", "No se pudo serializar la configuracion");
        return;
    }

    device_m->sendData(payload);
    saveSettings();
}

void EasyUDP_Widget::loadSettings() {
    settings_m.beginGroup("WiFi");
    ui->lineIP->setText(settings_m.value("ip", "").toString());
    ui->linePort->setText(settings_m.value("port", "").toString());
    ui->spinMaxFPS->setValue(settings_m.value("maxFps", ui->spinMaxFPS->value()).toInt());
    ui->spinQuality->setValue(settings_m.value("quality", ui->spinQuality->value()).toInt());
    ui->comboResolution->setCurrentIndex(settings_m.value("resolution", ui->comboResolution->currentIndex()).toInt());
    ui->checkGrayScale->setChecked(settings_m.value("grayScale", false).toBool());
    settings_m.endGroup();
}

void EasyUDP_Widget::saveSettings() {
    settings_m.beginGroup("WiFi");
    settings_m.setValue("ip", ui->lineIP->text());
    settings_m.setValue("port", ui->linePort->text());
    settings_m.setValue("maxFps", ui->spinMaxFPS->value());
    settings_m.setValue("quality", ui->spinQuality->value());
    settings_m.setValue("resolution", ui->comboResolution->currentIndex());
    settings_m.setValue("grayScale", ui->checkGrayScale->isChecked());
    settings_m.endGroup();
}

std::shared_ptr<EasyUDP> EasyUDP_Widget::getDevice() {
    return device_m;
}
