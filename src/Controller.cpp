#include "Controller.h"

Controller::Controller(uint8_t ID)
: controllerID_m{ ID }
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
        throw SDLError{ SDL_GetError() };
    }

    controller_m = SDL_GameControllerOpen(controllerID_m);

    if (!isConnected()) {
        throw ControllerNotConnected{ controllerID_m };
    }
}

Controller::~Controller() noexcept {
    SDL_GameControllerClose(controller_m);
}

Controller_Data Controller::getData() noexcept {
    if (!isConnected()) {
        return {};
    }

    refreshController();
    
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

uint8_t Controller::mapTrigger(int32_t axis) const noexcept {
    if (axis <= 0) {
        return 0;
    }

    const auto convertedAxis{ static_cast<uint8_t>((axis * Max_Maped_Value) / Max_UnMaped_Value) };

    return convertedAxis > Max_Maped_Value ? Max_Maped_Value : convertedAxis;
}

bool Controller::isConnected() const noexcept {
    return controller_m && SDL_GameControllerGetAttached(controller_m);
}

void Controller::refreshController() {
    if (!isConnected()) {
        return;
    }

    SDL_GameControllerUpdate();
    
    data_m.mutable_rightjoystick()->set_x_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_RIGHTX)));
    data_m.mutable_rightjoystick()->set_y_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_RIGHTY)));

    data_m.mutable_leftjoystick()->set_x_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_LEFTX)));
    data_m.mutable_leftjoystick()->set_y_axis(mapAxis(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_LEFTY)));

    data_m.set_righttrigger(mapTrigger(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)));
    data_m.set_lefttrigger(mapTrigger(SDL_GameControllerGetAxis(controller_m, SDL_CONTROLLER_AXIS_TRIGGERLEFT)));
}

