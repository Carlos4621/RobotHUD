#ifndef CONTROLLER_HEADER
#define CONTROLLER_HEADER

#include <QObject>
#include <SDL.h>
#include <QTimer>
#include "RobotHUDExceptions.h"

struct ControllerData {
    int8_t rightAxisX;
    int8_t rightAxisY;

    int8_t leftAxisX;
    int8_t leftAxisY;

    int8_t leftTrigger;
    int8_t rightTrigger;

    bool buttonA;
    bool buttonB;
    bool buttonX;
    bool buttonY;
    bool buttonStart;
    bool buttonSelect;
    bool rightButton;
    bool leftButton;

    bool padUp;
    bool padDown;
    bool padLeft;
    bool pasRight;
};

class Controller : public QObject 
{
    Q_OBJECT

public:
    explicit Controller(QObject* parent);

    ~Controller() noexcept;

    /// @brief Coloca el ID del control a usar y lo intenta abrir
    /// @param ID ID del control a usar
    void setID(uint8_t ID);

    [[nodiscard]]
    ControllerData getData() const noexcept;

    void setDeadZone(uint8_t deadZone) noexcept;

private slots:
    void refreshController();

private:
    static constexpr uint8_t Refresh_Rate_In_Ms{ 16 };
    static constexpr uint8_t Default_Dead_Zone{ 10 };
    static constexpr uint32_t Max_UnMaped_Value{ 32768 };
    static constexpr uint8_t Max_Maped_Value{ 100 };

    SDL_GameController* controller_m{ nullptr };
    QTimer* refreshTimer_m;
    ControllerData data_m;
    uint8_t deadZone_m{ Default_Dead_Zone };

    [[nodiscard]]
    int8_t mapAxis(int32_t axis) const noexcept;

};

#endif // !CONTROLLER_HEADER