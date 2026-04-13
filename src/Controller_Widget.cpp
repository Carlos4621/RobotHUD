#include "Controller_Widget.h"
#include "ui_Controller_Widget.h"

Controller_Widget::Controller_Widget(QWidget* parent) 
: QDialog{ parent }
, ui{ new Ui_Controller_Widget{} }
, portVerificationTimer_m{ new QTimer{this} }
, dataUpdateTimer_m{ new QTimer{this} }
{
    ui->setupUi(this);

    listAvalaibleIDs();
    portVerificationTimer_m->setInterval(ID_Verification_Interval);
    dataUpdateTimer_m->setInterval(Data_Update_Interval);

    connect(ui->button_connect, &QPushButton::pressed, this, &Controller_Widget::onPushConnect);
    connect(ui->button_cancel, &QPushButton::pressed, this, &Controller_Widget::close);
    connect(ui->button_apply, &QPushButton::pressed, this, &Controller_Widget::onPushApply);
    connect(portVerificationTimer_m, &QTimer::timeout, this, &Controller_Widget::listAvalaibleIDs);
    connect(dataUpdateTimer_m, &QTimer::timeout, this, &Controller_Widget::updateDisplayedControllerData);

    portVerificationTimer_m->start();
}

Controller_Widget::~Controller_Widget() {
    delete ui;
}

std::shared_ptr<Controller> Controller_Widget::getDevice() const noexcept {
    return controller_m;
}

void Controller_Widget::onPushConnect() {
    try {
        bool hasData{ false };
        const auto idData{ ui->combo_ID->currentData() };
        const auto controllerId{ static_cast<uint8_t>(idData.isValid() ? idData.toUInt(&hasData) : ui->combo_ID->currentText().toUInt(&hasData)) };

        if (!hasData) {
            showErrorMessage("ID de control invalido");
            return;
        }

        controller_m = std::make_shared<Controller>(controllerId);

        ui->widget_configurations->setEnabled(true);
        connectedID_m = QString::number(controllerId);
        lastDisconnectedID_m.clear();
        dataUpdateTimer_m->start();

        emit controllerConnected();
    }
    catch(const std::exception& e) {
        showErrorMessage(e.what());
    }
}

void Controller_Widget::onPushApply() {
    if (!controller_m) {
        return;
    }

    controller_m->setDeadZone(static_cast<uint8_t>(ui->edit_deadZone->text().toUInt()));
}

void Controller_Widget::updateDisplayedControllerData() {
    if (!controller_m) {
        clearDisplayedData();
        return;
    }

    const auto data{ controller_m->getData() };

    printControllerData(data);
}

void Controller_Widget::listAvalaibleIDs() {
    if (ui->combo_ID->view() && ui->combo_ID->view()->isVisible()) {
        return;
    }

    throwIfSDLIsNotInitialized();

    updateAvalaibleJoysticks();

    const bool hasPreviousConnection{ !connectedID_m.isEmpty() };
    const bool shouldAutoReconnect{ !lastDisconnectedID_m.isEmpty() && !controller_m };
    const QString previousSelection{ ui->combo_ID->currentText() };
    bool isControllerFound{ false };
    bool isAutoReconnectFound{ false };

    ui->combo_ID->clear();

    const auto joystickCount{ SDL_NumJoysticks() };
    for (int index{ 0 }; index < joystickCount; ++index) {
        if (!SDL_IsGameController(index)) {
            continue;
        }

        const auto controllerName{ SDL_GameControllerNameForIndex(index) };
        const auto displayName{ controllerName ? QString::fromUtf8(controllerName) : QStringLiteral("Unknown") };
        const auto displayText{ QStringLiteral("%1 - %2").arg(index).arg(displayName) };

        ui->combo_ID->addItem(displayText, index);

        if (hasPreviousConnection && (QString::number(index) == connectedID_m)) {
            isControllerFound = true;
        }

        if (shouldAutoReconnect && (QString::number(index) == lastDisconnectedID_m)) {
            isAutoReconnectFound = true;
        }
    }

    if (shouldAutoReconnect && isAutoReconnectFound) {
        const auto reconnectIndex{ ui->combo_ID->findData(lastDisconnectedID_m.toUInt()) };
        if (reconnectIndex >= 0) {
            ui->combo_ID->setCurrentIndex(reconnectIndex);
            onPushConnect();
            return;
        }
    }

    if (hasPreviousConnection && isControllerFound) {
        const auto previousIndex{ ui->combo_ID->findData(connectedID_m.toUInt()) };
        if (previousIndex >= 0) {
            ui->combo_ID->setCurrentIndex(previousIndex);
        }
    } else if (!hasPreviousConnection && !previousSelection.isEmpty()) {
        const auto selectionIndex{ ui->combo_ID->findText(previousSelection) };
        if (selectionIndex >= 0) {
            ui->combo_ID->setCurrentIndex(selectionIndex);
        }
    }

    if (hasPreviousConnection && !isControllerFound) {
        disconnectController();
    }
}

void Controller_Widget::showErrorMessage(std::string_view errorMessage) {
    QMessageBox::critical(this, "Error", QString::fromUtf8(errorMessage));
}

void Controller_Widget::disconnectController() {
    lastDisconnectedID_m = connectedID_m;
    connectedID_m.clear();
    ui->widget_configurations->setDisabled(true);
    controller_m.reset();

    clearDisplayedData();

    emit controllerDisconnected();
}

void Controller_Widget::clearDisplayedData() noexcept {
    ui->labelRightX->setText("0");
    ui->labelRightY->setText("0");
    ui->labelLeftX->setText("0");
    ui->labelLeftY->setText("0");
    ui->labelRightTrigger->setText("0");
    ui->labelLeftTrigger->setText("0");
}

void Controller_Widget::printControllerData(const Controller_Data &data) noexcept {
    ui->labelRightX->setText(QString::number(data.rightjoystick().x_axis()));
    ui->labelRightY->setText(QString::number(data.rightjoystick().y_axis()));
    ui->labelLeftX->setText(QString::number(data.leftjoystick().x_axis()));
    ui->labelLeftY->setText(QString::number(data.leftjoystick().y_axis()));
    ui->labelRightTrigger->setText(QString::number(data.righttrigger()));
    ui->labelLeftTrigger->setText(QString::number(data.lefttrigger()));
}

void Controller_Widget::throwIfSDLIsNotInitialized() const {
    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0) {
        if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
            throw SDLError{ SDL_GetError() };
        }
    }
}

void Controller_Widget::updateAvalaibleJoysticks() {
    SDL_PumpEvents();
    SDL_GameControllerUpdate();
    SDL_JoystickUpdate();
}
