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

class CantOpenController : public std::exception {
public:
    explicit CantOpenController(uint8_t ID) 
    : message_m{ std::string("Cant open controller with ID: ") + std::to_string(ID) }
    {}

    [[nodiscard]]
    const char* what() const noexcept override {
        return message_m.c_str();
    }

private:
    std::string message_m;
};

#endif // !ROBOT_HUD_EXCEPTIONS_HEADER