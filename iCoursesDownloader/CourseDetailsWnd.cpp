#include "CourseDetailsWnd.h"
#include <QFileSystemModel>
#include <QStyledItemDelegate>
#include <QFileDialog>
#include <QTextCodec>
#include <QDir>
#include <QStandardItemModel>
#include <QJsonParseError>
#include "DataManager.h"

#include "ResourcesJsonParser.h"
#include "CourseResourcesModel.h"
#include "CourseResourcesSelectionModel.h"
#include <iostream>

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

void CourseDetailsWnd::btn_export(){
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QString	fileName = QFileDialog::getSaveFileName(this,
													"Save Course Resources Info",
													StorageManager::getLocalFilePath("CoursesData"));
	QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)){
		return;
    }
	QTextStream fileOutStream(&file);
	int depth = 0;
	const int stackMax = 6;
	int *stack = new int[stackMax]();
	CourseResourcesTree* *treeStack = new CourseResourcesTree*[stackMax];

	treeStack[0] = CourseResourcesManager::getResourcesTree(courseModel->courseInfo->id, 0);
	depth++;
	while(depth > 0){//depth = 1,2,3...
		if(stack[depth] >= treeStack[depth - 1]->childrenList.size()){
			depth--;
			stack[depth]++;
			continue;
		}
		//Then, it's vertically not overflowed.
		treeStack[depth] = treeStack[depth - 1]->childrenList.at(stack[depth]);

		//if(treeStack[depth]->childrenList.empty()){//Luckly got what we need
		//if(treeStack[depth]->type != CourseResourcesTree::Folder){//Luckly got what we need
		if(treeStack[depth]->type == CourseResourcesTree::Document){//Luckly got what we need
			//Do something about tree tips
			/*int d=depth;
			while(d-->0){
				fileOutStream<<"   ";
			}
			fileOutStream<<treeStack[depth]->visualName;
			fileOutStream<<"\r\n";*/
			fileOutStream<<treeStack[depth]->resUrl;
			fileOutStream<<"\r\n";

			stack[depth]++;
			continue;
		}

		//However, this is a intermediate node. And go on.

		//Stack is overflowed!
		if(depth == stackMax - 1){
			depth--;
			continue;
		}
		
		/*int d=depth;
		while(d-->0){
				fileOutStream<<"   ";
		}
		fileOutStream<<treeStack[depth]->visualName;
		fileOutStream<<"\r\n";*/


		//go on.
		depth++;

		stack[depth] = 0;
	}
}

void CourseDetailsWnd::btn_rename(){
	QString baseDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                        "",
                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	int depth = 0;
	const int stackMax = 6;
	int *stack = new int[stackMax]();
	CourseResourcesTree* *treeStack = new CourseResourcesTree*[stackMax];

	treeStack[0] = CourseResourcesManager::getResourcesTree(courseModel->courseInfo->id, 0);
	depth++;

	while(depth > 0){//depth = 1,2,3...
		if(stack[depth] >= treeStack[depth - 1]->childrenList.size()){
			depth--;
			stack[depth]++;
			continue;
		}
		//Then, it's vertically not overflowed.
		treeStack[depth] = treeStack[depth - 1]->childrenList.at(stack[depth]);

		//if(treeStack[depth]->childrenList.empty()){//Luckly got what we need
		if(treeStack[depth]->type != CourseResourcesTree::Folder){//Luckly got what we need
			//Do something about tree tips
			qDebug()<<"Checking "<<baseDir % treeStack[depth]->resUrl.rightRef(treeStack[depth]->resUrl.size() - treeStack[depth]->resUrl.lastIndexOf('/'));
			QFile file(baseDir % treeStack[depth]->resUrl.rightRef(treeStack[depth]->resUrl.size() - treeStack[depth]->resUrl.lastIndexOf('/')));
			if(file.exists()){
				QString dest = baseDir;
				if(treeStack[depth]->type == CourseResourcesTree::Document){
					dest.append("/Doc");
				}
				for(int i = 1; i <= depth; i++){
					if(i == depth){
						QDir dir;
						if (!dir.exists(dest)){
							if(!dir.mkpath(dest)){
								__asm{int 3}
							};
						}
					}
					dest.append(QChar('\\'));
					dest.append(treeStack[i]->visualName);
				}
				dest.append(treeStack[depth]->resUrl.rightRef(treeStack[depth]->resUrl.size() - treeStack[depth]->resUrl.lastIndexOf('.')));
				qDebug()<<" Dest: "<<dest;
				file.rename(dest);
			}

			stack[depth]++;
			continue;
		}

		//However, this is a intermediate node. And go on.

		//Stack is overflowed!
		if(depth == stackMax - 1){
			depth--;
			stack[depth]++;//delta
			continue;
		}

		//go on.
		depth++;

		stack[depth] = 0;
	}
}
