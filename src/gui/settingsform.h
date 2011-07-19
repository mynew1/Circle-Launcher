#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QDialog>
#include "src/settings.h"
#include <QFileDialog>

namespace Ui {
    class SettingsForm;
}

class SettingsForm : public QDialog
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
    void on_buttonBox_accepted();

    void on_deleteButton_clicked();

    void on_browseButton_clicked();

    void on_addButton_clicked();

private:
    Ui::SettingsForm *ui;
    Settings *settings;

    bool haveSettings;

    void LoadSettings();
    void SaveSettings();
    void UpdateRealms();
};

#endif // SETTINGSFORM_H
