#pragma once

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include "ui_course_details.h"
#include "datatypes.h"
#include <QMainWindow>
#include "CourseResourcesSelectionModel.h"

class CourseDetailsWnd : public QMainWindow
{
	Q_OBJECT
public:
	CourseModel *courseModel;
	CourseDetailsWnd(QWidget *parent = 0);
protected:
	void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent*) Q_DECL_OVERRIDE {
		//ui.tw_courseDetails->clear();
		emit closed();
	};
	void loadCharpters();
protected slots:
	void resourcesList_ctxMenu(QPoint pos);
	void menu_download();
signals:
	void closed();
private:
	Ui::CourseDetailsWindow ui;
	QMenu *resourcesListMenu;

	QModelIndex *treeListMenuTriggeredAt;
	//CourseResourcesSelectionModel *listSelectionModel;
	CourseResourcesSelectionModel *listSelectionModel;
};
