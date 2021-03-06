#pragma once
#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QMainWindow>

#include "datatypes.h"
#include "CourseResourcesSelectionModel.h"
#include "CourseResourcesManager.h"

#include "ui_course_details.h"

class CourseDetailsWnd : public QMainWindow
{
    Q_OBJECT
public:
    CourseModel *courseModel;
    CourseDetailsWnd(QWidget *parent = 0);
protected:
    void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent*) Q_DECL_OVERRIDE;
    void resourcesSortChange(int sort);
protected slots:
    void resourcesList_ctxMenu(QPoint pos);
    void menu_download();
    void menu_copyLink();
    void btn_t1(){resourcesSortChange(0);}
    void btn_t2(){resourcesSortChange(1);}
    void btn_t3(){resourcesSortChange(2);}
    void btn_t4(){resourcesSortChange(3);}
    void btn_export();
    void btn_rename();
    void cbx_typeFilter(int index);
signals:
    void closed();
private:
    Ui::CourseDetailsWindow ui;
    QMenu *resourcesListMenu;

    QModelIndex treeListMenuTriggeredAt;
    CourseResourcesModel *currentListModel;
    CourseResourcesSelectionModel *listSelectionModel;
    int selectedSort;
};
