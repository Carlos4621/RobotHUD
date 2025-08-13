#include "EasyUDP_Widget.h"
#include "ui_EasyUDP_Widget.h"

EasyUDP_Widget::EasyUDP_Widget(QWidget *parent)
: QDialog{ parent }
, ui{ new Ui::Dialog{} }
{
    ui->setupUi(this);

    connect(ui->buttonConnect, &QPushButton::pressed, this, &EasyUDP_Widget::onPushConnect);
    connect(ui->buttonCancel, &QPushButton::pressed, this, &EasyUDP_Widget::close);
}

EasyUDP_Widget::~EasyUDP_Widget() noexcept {
    delete ui;
}

void EasyUDP_Widget::onPushConnect() {
    device_m = std::make_shared<EasyUDP>(ui->lineIP->text().toStdString(), ui->linePort->text().toStdString());
}

std::shared_ptr<EasyUDP> EasyUDP_Widget::getDevice() {
    return device_m;
}
