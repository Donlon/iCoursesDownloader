#ifndef COURCESWND_H
#define COURCESWND_H

#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include <QListWidgetItem>
#include "ui_courses.h"
#include "datatypes.h"
#include "AddCourseDlg.h"
#include "CourseItemWidget.h"

#include "DataManager.h"//TODO: Delete it
#include "CourseDetailsWnd.h"

class CoursesWnd : public QDialog
{
    Q_OBJECT
public:
    CoursesWnd(QWidget *parent = nullptr);
    CourseModel* getCurrentSelectedCourse();
    void reflushList();
    CourseModel* buildModelForCourse(Course* course);
protected slots:
    void btn_add();
    void btn_open();
    void onCourseListContextMenu(const QPoint& pos);
    void menu_removeItem(bool checked);
    void menu_reloadItem(bool checked);
    void removeItem(CourseModel* item);
    void onCourseDetailsWndClose();
    void saveList(){StorageManager::saveCourseList(courseModelList);};
private:
    Ui::CoursesWindow ui;
    AddCourseDialog *addCourseDlg;
    CourseDetailsWnd *courseDetailsWnd;
    QList<CourseModel*>* courseModelList;//Is this point old-fashioned too much?
    QMenu *CourseListMenu;
    QModelIndex *treeListMenuTriggeredAt;

};

#endif // COURCESWND_H
