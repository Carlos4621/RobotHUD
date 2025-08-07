#include "RobotHUD.h"
#include "ui_RobotHUD.h"
#include "EasyLoRa_Widget.h"

RobotHUD::RobotHUD(QWidget *parent)
: QMainWindow{ parent }
, ui{ new Ui::RobotHUD{} }
, controller_m{ new Controller{this} }
, testTimer_m{ new QTimer{this} }
{
    ui->setupUi(this);

    connect(testTimer_m, &QTimer::timeout, this, &RobotHUD::testController);

    testTimer_m->setInterval(20);
    testTimer_m->start();
}

RobotHUD::~RobotHUD() {
    delete ui;
}

void RobotHUD::testController() {
    
}
