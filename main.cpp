#include <QtGui/QApplication>
#include "src/gui/mainwindow.h"
#include "src/settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Settings *settings = new Settings;
    MainWindow w(settings);
    w.show();
    return a.exec();
}
