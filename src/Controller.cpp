#include "Controller.h"

Controller::Controller(QObject *parent) 
: QObject{ parent }
, refreshTimer_m{ new QTimer{this} }
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
        throw SDLError{ SDL_GetError() };
    }

    connect(refreshTimer_m, &QTimer::timeout, this, &Controller::refreshController);

    refreshTimer_m->setInterval(Refresh_Rate_In_Ms);
    refreshTimer_m->start();
}

Controller::~Controller() noexcept {
    SDL_GameControllerClose(controller_m);
}

void Controller::setID(uint8_t ID) {
    controller_m = SDL_GameControllerOpen(ID);

    if (!controller_m) {
        throw CantOpenController{ ID };
    }
}

Controller_Data Controller::getData() const noexcept {
    if (!controller_m) {
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

void Controller::refreshController() {
    if (!controller_m) {
        return;
    }

    SDL_GameControllerUpdate();
    
    data_m.mutable_rightjoystick()->set_x_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_RIGHTX)));
    data_m.mutable_rightjoystick()->set_y_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_RIGHTY)));

    data_m.mutable_leftjoystick()->set_x_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_LEFTX)));
    data_m.mutable_leftjoystick()->set_y_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_LEFTY)));
}

