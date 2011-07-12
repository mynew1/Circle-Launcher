#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "src/parsers/forummgr.h"

namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    ForumMgr page1;
    ForumMgr page2;
    ForumMgr page3;

    QString *pageData;
private slots:
    void Update1Forum();
    void Update2Forum();
    void Update3Forum();

    void ActiveForumButton(QPushButton *button);
    void UnActiveForumButton(QPushButton *button);

    void on_forumButton1_clicked();
    void on_forumButton2_clicked();
    void on_forumButton3_clicked();
};

#endif // MAINWIDGET_H
