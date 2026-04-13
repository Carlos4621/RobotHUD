#ifndef CONTROLLER_WIDGET_H
#define CONTROLLER_WIDGET_H

#include <QtWidgets/QWidget>
#include "Controller.h"
#include <memory>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QAbstractItemView>
#include <QDialog>
#include <QTimer>

QT_BEGIN_NAMESPACE
class Ui_Controller_Widget;
QT_END_NAMESPACE

/// @brief Widget que permite la conexión y configuración del control
class Controller_Widget : public QDialog
{
    Q_OBJECT

public:
    /// @brief Cosntructor base
    /// @param parent Padre del widget
    explicit Controller_Widget(QWidget *parent = nullptr);
    
    ~Controller_Widget();

    /// @brief Obtiene el control conectado, debe ser usado junto a la señal succesfulConnection para evitar punteros nulos
    /// @return El control conectado actualmente, es null en caso de no haber dispositivo conectado
    [[nodiscard]]
    std::shared_ptr<Controller> getDevice() const noexcept;

signals:
    void controllerConnected();
    void controllerDisconnected();

private slots:
    void onPushConnect();
    void onPushApply();
    void updateDisplayedControllerData();

private:

    static constexpr std::chrono::milliseconds ID_Verification_Interval{ 100 };
    static constexpr std::chrono::milliseconds Data_Update_Interval{ 100 };

    Ui_Controller_Widget* ui;

    std::shared_ptr<Controller> controller_m;

    QTimer* portVerificationTimer_m;
    QTimer* dataUpdateTimer_m;
    QString connectedID_m;
    QString lastDisconnectedID_m;

    void showErrorMessage(std::string_view errorMessage);
 
    void listAvalaibleIDs();

    void disconnectController();

    void clearDisplayedData() noexcept;
    void printControllerData(const Controller_Data& data) noexcept;

    void throwIfSDLIsNotInitialized() const;

    void updateAvalaibleJoysticks();
};

#endif // CONTROLLER_WIDGET_H
