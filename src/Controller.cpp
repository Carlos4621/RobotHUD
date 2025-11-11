#include "Controller.h"

Controller::Controller(QObject *parent) 
: QObject{ parent }
, refreshTimer_m{ new QTimer{this} }
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
        throw SDLError{ SDL_GetError() };
    }

    connect(refreshTimer_m, &QTimer::timeout, this, &Controller::refreshController);

    refreshTimer_m->setInterval(Refresh_Rate);
    refreshTimer_m->start();
}

Controller::~Controller() noexcept {
    SDL_GameControllerClose(controller_m);
}

void Controller::setID(uint8_t ID) {
    controller_m = SDL_GameControllerOpen(ID);

    if (!isConnected()) {
        emit controllerDisconnected();
    } else {
        emit controllerConnected();
    }

    controllerID_m = ID;
}

Controller_Data Controller::getData() const noexcept {
    if (!isConnected()) {
        return {};
    }

    return data_m;
}

void Controller::setDeadZone(uint8_t deadZone) noexcept {
    deadZone_m = deadZone;
}

int8_t Controller::mapAxis(int32_t axis) const noexcept {
    const auto convertedAxis{ static_cast<int8_t>((axis * Max_Maped_Value) / Max_UnMaped_Value) };

    if (std::abs(convertedAxis) < deadZone_m) {
        return 0;
    }

    return convertedAxis;
}

void Controller::tryReconnect() {
    if (!disconnectionNotified) {
        emit controllerDisconnected();
        disconnectionNotified = true;
    }
    
    controller_m = SDL_GameControllerOpen(controllerID_m);

    if (isConnected()) {
        emit controllerConnected();
        disconnectionNotified = false;
    }
}

bool Controller::isConnected() const noexcept {
    return controller_m && SDL_GameControllerGetAttached(controller_m);
}

void Controller::refreshController() {
    if (!isConnected()) {
        tryReconnect();
        return;
    }

    SDL_GameControllerUpdate();
    
    data_m.mutable_rightjoystick()->set_x_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_RIGHTX)));
    data_m.mutable_rightjoystick()->set_y_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_RIGHTY)));

    data_m.mutable_leftjoystick()->set_x_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_LEFTX)));
    data_m.mutable_leftjoystick()->set_y_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_LEFTY)));
}

