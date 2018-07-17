#include "mainwnd.h"

MainWnd::MainWnd(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    coursesWnd = new CoursesWnd();
    settingsWnd = new SettingsWnd();
    downloaderWnd = new DownloaderWnd();
    
    connect(coursesWnd, &QDialog::finished,
        this, &MainWnd::onSubWndClose);
    connect(settingsWnd, &QDialog::finished,
        this, &MainWnd::onSubWndClose);
    connect(downloaderWnd, &QDialog::finished,
        this, &MainWnd::onSubWndClose);
    
    coursesWnd->show();
}

void MainWnd::btn_allCourses()
{
    this->hide();
    coursesWnd->show();
}

void MainWnd::btn_downloader()
{
    this->hide();
    downloaderWnd->show();
}

void MainWnd::btn_settings()
{
    this->hide();
    settingsWnd->show();
}

void MainWnd::onSubWndClose(int)
{
    this->show();
}