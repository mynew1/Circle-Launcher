#include <QtGui/QApplication>
#include "src/gui/mainwindow.h"
#include "src/addon/addoninstaller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AddonInstaller in;
    qDebug() << in.isAlreadyInstalled("/home/walkline/download/AuctioneerSuite-5.8.4723.zip","/home/walkline/download/");
    in.installAddon("/home/walkline/download/AuctioneerSuite-5.8.4723.zip","/home/walkline/download/");
    MainWindow w;
    w.show();
    return a.exec();
}
