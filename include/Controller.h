#ifndef CONTROLLER_HEADER
#define CONTROLLER_HEADER

#include <SDL.h>
#include "RobotHUDExceptions.h"
#include "Controller_Data.pb.h"

class Controller {
public:

    /// @brief Constructor base
    /// @param ID ID del control a conectar
    explicit Controller(uint8_t ID);

    ~Controller() noexcept;

    /// @brief Obtiene la última data obtenida del control
    /// @return Última data obtenida
    [[nodiscard]]
    Controller_Data getData() noexcept;

    /// @brief Cambia la zona muerta del control
    /// @param deadZone Nueva zona muerta
    void setDeadZone(uint8_t deadZone) noexcept;

private:
    static constexpr uint8_t Default_Dead_Zone{ 10 };
    static constexpr uint32_t Max_UnMaped_Value{ 32768 };
    static constexpr uint8_t Max_Maped_Value{ 100 };

    SDL_GameController* controller_m{ nullptr };
    Controller_Data data_m;
    uint8_t deadZone_m{ Default_Dead_Zone };
    uint8_t controllerID_m;

    [[nodiscard]]
    int8_t mapAxis(int32_t axis) const noexcept;

    [[nodiscard]]
    uint8_t mapTrigger(int32_t axis) const noexcept;

    [[nodiscard]]
    bool isConnected() const noexcept;

    void refreshController();
};

#endif // !CONTROLLER_HEADER