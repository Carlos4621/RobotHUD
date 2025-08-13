#ifndef EASY_UDP_WIDGET_HEADER
#define EASY_UDP_WIDGET_HEADER

#include <QDialog>
#include <memory>
#include "EasyUDP.h"

QT_BEGIN_NAMESPACE
class QWidget;
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class EasyUDP_Widget : public QDialog 
{
    Q_OBJECT

public:
    explicit EasyUDP_Widget(QWidget* parent = nullptr);

    ~EasyUDP_Widget() noexcept;

    std::shared_ptr<EasyUDP> getDevice();

signals:
    void succesfulConnection();

private slots:

    void onPushConnect();

private:
    Ui::Dialog* ui;
    std::shared_ptr<EasyUDP> device_m{ nullptr };

};

#endif //!EASY_UDP_WIDGET_HEADER