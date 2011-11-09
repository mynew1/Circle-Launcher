#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include "src/settings.h"
#include <QFileDialog>

namespace Ui {
    class SettingsForm;
}


class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();
    void setSettings(Settings *_settings);
    void show();

signals:
    void RealmsChanged();

private slots:
    void on_deleteButton_clicked();

    void on_browseButton_clicked();

    void on_addButton_clicked();

    void on_generalButton_clicked();

    void on_guiButton_clicked();

    void on_realmsButton_clicked();

    void on_gamePath_textChanged(QString );

    void on_wtfCheck_stateChanged(int );

    void on_cacheCheck_stateChanged(int );

    void on_updateTime_valueChanged(int );

    void on_exitCheck_stateChanged(int );

    void on_forum1_textEdited(QString );

    void on_forum2_textEdited(QString );

    void on_forum3_textEdited(QString );

    void on_realmsTable_cellChanged(int row, int column);

    void on_clearOnlineButton_clicked();

private:
    Ui::SettingsForm *ui;
    Settings *settings;

    bool haveSettings;

    void LoadSettings();
    void SaveSettings();
    void UpdateRealms();
    void ActivateButton(QPushButton *button, QString img);
    void UnActivateButtons();
};

#endif // SETTINGSFORM_H
