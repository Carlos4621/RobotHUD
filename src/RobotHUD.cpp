#include "RobotHUD.h"
#include "ui_RobotHUD.h"
#include "EasyLoRa_Widget.h"

RobotHUD::RobotHUD(QWidget *parent)
: QMainWindow{ parent }
, ui{ new Ui::RobotHUD{} }
, controller_m{ new Controller{this} }
, testTimer_m{ new QTimer{this} }
, LoRaConfigurationMenu_m{ new QDialog{this} }
, easyLoRaWidget_m{ new EasyLoRa_Widget{LoRaConfigurationMenu_m} }
{
    ui->setupUi(this);

    setupLoRaMenuDialog();

    connect(ui->actionLoRa, &QAction::triggered, this, &RobotHUD::onPushOpenLoRaMenu);
}

RobotHUD::~RobotHUD() {
    delete ui;
}

void RobotHUD::onPushOpenLoRaMenu() {
    LoRaConfigurationMenu_m->exec();
}

void RobotHUD::setupLoRaMenuDialog() {
    QVBoxLayout* layout{ new QVBoxLayout{LoRaConfigurationMenu_m} };
    layout->addWidget(easyLoRaWidget_m);
    LoRaConfigurationMenu_m->setLayout(layout);
    LoRaConfigurationMenu_m->setWindowTitle("LoRa Configuration");
    LoRaConfigurationMenu_m->setFixedSize(LoRaConfigurationMenu_m->sizeHint());
}

void RobotHUD::testController() {

}