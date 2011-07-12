#include <QtGui/QApplication>
#include "src/gui/mainwindow.h"
#include "src/parsers/circle/forumthreadparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ForumThreadParser parser;
    parser.setPage(QUrl("http://forum.wowcircle.com/showthread.php?t=53664"));
    MainWindow w;
    w.show();
    return a.exec();
}
