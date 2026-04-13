#ifndef ROBOT_HUD_EXCEPTIONS_HEADER
#define ROBOT_HUD_EXCEPTIONS_HEADER

#include <exception>
#include <string>
#include <cstdint>

class SDLError : public std::exception {
public:
    explicit SDLError(std::string_view message) 
    : message_m{ std::string("SDL Error: ") + message.data() }
    {}

    [[nodiscard]]
    const char* what() const noexcept override {
        return message_m.c_str();
    }

private:
    std::string message_m;

};

class ControllerNotConnected : public std::exception {
public:
    explicit ControllerNotConnected(uint8_t ID)
    : ID_m{ ID }
    {}

    [[nodiscard]]
    const char* what() const noexcept override {
        return std::format("El control con el ID {} no pudo ser conectado", ID_m).c_str();
    }

private:
    uint8_t ID_m;
};

#endif // !ROBOT_HUD_EXCEPTIONS_HEADER