#include <QApplication>
#include "RobotHUD.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("RobotHUD");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Carlos Iván");
    
    RobotHUD window;
    window.show();
    
    return app.exec();
}
