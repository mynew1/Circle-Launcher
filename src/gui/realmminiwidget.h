#ifndef REALMMINIWIDGET_H
#define REALMMINIWIDGET_H

#include <QWidget>
#include <QDebug>

namespace Ui {
    class RealmMiniWidget;
}

class RealmMiniWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RealmMiniWidget(QWidget *parent = 0);
    ~RealmMiniWidget();

    void setOnline(int n);
    void setMaxOnline(int n);
    void setIndex(int n) { index = n; }
    void setRealmName(QString name);

    int getOnline();
    int getMaxOnline();
    int getIndex() { return index;}
    QString getRealmName();

signals:
    void clicked(RealmMiniWidget *);

private:
    Ui::RealmMiniWidget *ui;
    int index;
    void mousePressEvent(QMouseEvent*/* event*/);
};

#endif // REALMMINIWIDGET_H
