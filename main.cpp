#include <QtGui/QApplication>
#include "src/gui/mainwindow.h"
#include "src/parsers/circle/forumdisplayparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ForumDisplayParser parser;
    parser.setPage(QUrl("http://forum.wowcircle.com/forumdisplay.php?f=13"));
    MainWindow w;
    w.show();
    return a.exec();
}
