#ifndef DOWNLOADERWND_H
#define DOWNLOADERWND_H

#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include "ui_downloader.h"

class DownloaderWnd : public QDialog
{
	Q_OBJECT
public:
	DownloaderWnd(QWidget *parent = 0);
private:
	Ui::DownloaderWindow ui;
};

#endif // DOWNLOADERWND_H
