#ifndef SETTINGSWND_H
#define SETTINGSWND_H

#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include "ui_settings.h"

class SettingsWnd : public QDialog
{
    Q_OBJECT

public:
    SettingsWnd(QWidget *parent = 0);
private:
    Ui::SettingsWindow ui;
};

#endif // SETTINGSWND_H
