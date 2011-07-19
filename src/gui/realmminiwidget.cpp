#include "realmminiwidget.h"
#include "ui_realmminiwidget.h"

RealmMiniWidget::RealmMiniWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RealmMiniWidget)
{
    ui->setupUi(this);
}

RealmMiniWidget::~RealmMiniWidget()
{
    delete ui;
}

void RealmMiniWidget::mousePressEvent(QMouseEvent *)
{
    emit clicked(this);
}

void RealmMiniWidget::setOnline(int n)
{
    ui->progressBar->setValue(n);
}

void RealmMiniWidget::setMaxOnline(int n)
{
    ui->progressBar->setMaximum(n);
}

void RealmMiniWidget::setRealmName(QString name)
{
    ui->realmName->setText(name);
}

int RealmMiniWidget::getOnline()
{
    return ui->progressBar->value();
}

int RealmMiniWidget::getMaxOnline()
{
    return ui->progressBar->maximum();
}

QString RealmMiniWidget::getRealmName()
{
    return ui->realmName->text();
}
