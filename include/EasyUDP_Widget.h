#ifndef EASY_UDP_WIDGET_HEADER
#define EASY_UDP_WIDGET_HEADER

#include <QDialog>
#include <QSettings>
#include <memory>
#include "EasyUDP.h"
#include "ModuleConfiguration.pb.h"

QT_BEGIN_NAMESPACE
class QWidget;
namespace Ui { class Dialog; }
QT_END_NAMESPACE

/// @brief Widget que permite la conexión de un objeto EasyUDP
class EasyUDP_Widget : public QDialog 
{
    Q_OBJECT

public:

    /// @brief Constructor base
    /// @param parent Padre del widget
    explicit EasyUDP_Widget(QWidget* parent = nullptr);

    ~EasyUDP_Widget() noexcept;

    /// @brief Obtiene el objeto EasyUDP, debe ser usado junto a la señal succesfulConnection para evitar punteros nulos
    /// @return El dispositivo configurado, null en caso de no haber conexión
    [[nodiscard]]
    std::shared_ptr<EasyUDP> getDevice();

signals:
    void succesfulConnection();

private slots:

    void onPushConnect();
    void onPushApply();

private:
    Ui::Dialog* ui;
    std::shared_ptr<EasyUDP> device_m{ nullptr };
    QSettings settings_m;

    void loadSettings();
    void saveSettings();

};

#endif //!EASY_UDP_WIDGET_HEADER