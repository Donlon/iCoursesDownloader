#include "CourseDetailsWnd.h"
#include <QFileSystemModel>
#include <QStyledItemDelegate>
#include <QDir>
#include <QStandardItemModel>
#include <QJsonParseError>
#include "DataManager.h"

#include "ResourcesJsonParser.h"
#include "CourseResourcesModel.h"
#include "CourseResourcesSelectionModel.h"

CourseDetailsWnd::CourseDetailsWnd(QWidget* parent)	: QMainWindow(parent){
	ui.setupUi(this);
	ui.tv_title->setTextInteractionFlags(Qt::TextSelectableByMouse);
	ui.tv_school->setTextInteractionFlags(Qt::TextSelectableByMouse);

    resourcesListMenu = new QMenu();
    QAction *lw_action1 = resourcesListMenu->addAction("&Download");
    //QAction *lw_action2 = resourcesListMenu->addAction("&Reload");
    //QAction *lw_action3 = resourcesListMenu->addAction("&Remove");
    connect(lw_action1, &QAction::triggered, this, &CourseDetailsWnd::menu_download);
    //connect(lw_action2, &QAction::triggered, this, &CourseDetailsWnd::menu_reloadItem);
    //connect(lw_action3, &QAction::triggered, this, &CourseDetailsWnd::menu_removeItem);
}

void CourseDetailsWnd::showEvent(QShowEvent* event){
	Q_ASSERT (courseModel != nullptr);

	ui.tw_courseDetails->clear();

	setWindowTitle(courseModel->courseInfo->title + " | " + courseModel->courseInfo->school);
	ui.tv_title->setText(courseModel->courseInfo->title);
	ui.tv_school->setText(courseModel->courseInfo->school);
	ui.lbl_logo->setPixmap(*(courseModel->loadedLogo));
	ui.lbl_logo->setScaledContents(true);

	ui.tv_courseDetails->setModel(CourseResourcesManager::getListModel(courseModel->courseInfo->id, 0));
	ui.tv_courseDetails->setColumnWidth(0, 450);

	listSelectionModel = new CourseResourcesSelectionModel(reinterpret_cast<CourseResourcesModel*>(ui.tv_courseDetails->model()));
	ui.tv_courseDetails->setSelectionModel(listSelectionModel);
}

void CourseDetailsWnd::closeEvent(QCloseEvent*){
	emit closed();
}

void CourseDetailsWnd::resourcesSortSelectionChanged(int sort){
	ui.tv_courseDetails->setModel(CourseResourcesManager::getListModel(courseModel->courseInfo->id, sort));
	ui.tv_courseDetails->setColumnWidth(0, 450);
}

void CourseDetailsWnd::resourcesList_ctxMenu(QPoint pos){
	QModelIndex index = ui.tv_courseDetails->indexAt(pos);

	if(!index.isValid()) {
		return;
	}
	treeListMenuTriggeredAt = &index;
    resourcesListMenu->exec(QCursor::pos());
}

void CourseDetailsWnd::menu_download(){
	//treeListMenuTriggeredAt->data();
	//ui.tv_courseDetails->selectionModel()->select(treeListMenuTriggeredAt->parent(), QItemSelectionModel::Select);

	QItemSelectionModel *listSelectionModel = ui.tv_courseDetails->selectionModel();
	//listSelectionModel->select(treeListMenuTriggeredAt->parent(), QItemSelectionModel::Select);

	treeListMenuTriggeredAt = nullptr;
}
