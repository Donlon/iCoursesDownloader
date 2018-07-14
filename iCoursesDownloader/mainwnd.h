#ifndef MAINWND_H
#define MAINWND_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwnd.h"

#include "CoursesWnd.h"
#include "SettingsWnd.h"
#include "DownloaderWnd.h"

class MainWnd : public QMainWindow{
	Q_OBJECT
	CoursesWnd *coursesWnd;
	SettingsWnd *settingsWnd;
	DownloaderWnd *downloaderWnd;
private slots:
	void btn_allCourses();
    void btn_downloader();
    void btn_settings();
    void onSubWndClose(int);
public:
	MainWnd(QWidget *parent = 0);
private:
	Ui::MainWndClass ui;
};

#endif // MAINWND_H
