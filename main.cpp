#include <QtGui/QApplication>
#include "src/gui/mainwindow.h"
#include "src/httpdownload.h"
#include "src/parsers/curse/cursesearchparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    CurseSearchParser search;
//    search.setIndexTerm("za");
    MainWindow w;
    w.show();
    return a.exec();
}
