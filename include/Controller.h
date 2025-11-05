#ifndef CONTROLLER_HEADER
#define CONTROLLER_HEADER

#include <QObject>
#include <SDL.h>
#include <QTimer>
#include "RobotHUDExceptions.h"
#include "Controller_Data.pb.h"

class Controller : public QObject 
{
    Q_OBJECT

public:

    /// @brief Constructor base
    /// @param parent Padre del widget
    explicit Controller(QObject* parent = nullptr);

    ~Controller() noexcept;

    /// @brief Coloca el ID del control a usar y lo intenta abrir
    /// @param ID ID del control a usar
    void setID(uint8_t ID);

    /// @brief Obtiene la última data obtenida del control
    /// @return Última data obtenida
    [[nodiscard]]
    Controller_Data getData() const noexcept;

    /// @brief Cambia la zona muerta del control
    /// @param deadZone Nueva zona muerta
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
    Controller_Data data_m;
    uint8_t deadZone_m{ Default_Dead_Zone };

    [[nodiscard]]
    int8_t mapAxis(int32_t axis) const noexcept;

};

#endif // !CONTROLLER_HEADER