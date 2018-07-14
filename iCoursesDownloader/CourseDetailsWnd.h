#pragma once

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QMainWindow>
#include "ui_course_details.h"
#include "datatypes.h"
#include "CourseResourcesSelectionModel.h"
#include "CourseResourcesManager.h"

class CourseDetailsWnd : public QMainWindow
{
	Q_OBJECT
public:
	CourseModel *courseModel;
	CourseDetailsWnd(QWidget *parent = 0);
protected:
	void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent*) Q_DECL_OVERRIDE;
	void resourcesSortSelectionChanged(int sort);
protected slots:
	void resourcesList_ctxMenu(QPoint pos);
	void menu_download();
    void btn_t1(){resourcesSortSelectionChanged(0);}
    void btn_t2(){resourcesSortSelectionChanged(1);}
    void btn_t3(){resourcesSortSelectionChanged(2);}
    void btn_t4(){resourcesSortSelectionChanged(3);}
signals:
	void closed();
private:
	Ui::CourseDetailsWindow ui;
	QMenu *resourcesListMenu;

	QModelIndex *treeListMenuTriggeredAt;
	//CourseResourcesSelectionModel *listSelectionModel;
	CourseResourcesSelectionModel *listSelectionModel;
};
