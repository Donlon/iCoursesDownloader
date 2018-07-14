#include "CourseDetailsWnd.h"
#include <QFileSystemModel>
#include <QStyledItemDelegate>
#include <QDir>
#include <QStandardItemModel>
#include <QJsonParseError>
#include "StorageManager.h"

#include "ResourcesJsonParser.h"
#include "CourseResourcesModel.h"
#include "CourseResourcesSelectionModel.h"

CourseDetailsWnd::CourseDetailsWnd(QWidget* parent)	: QMainWindow(parent){
	//if(parent == nullptr){
	//	__asm{int 3}
	//	return;
	//}
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
	

	loadCharpters();
}

void CourseDetailsWnd::loadCharpters()
{
	//QFileSystemModel *model = new QFileSystemModel;
	//model->setRootPath(QDir::currentPath());

	//	QStandardItemModel* model = new QStandardItemModel(ui.tv_courseDetails);//TODO: parent
	//	model->setHorizontalHeaderLabels(QStringList()<<"Name"<<"Size"<<"Type");
	//	ui.tv_courseDetails->setModel(model);

	//ui.tw_courseDetails->setHeaderLabels(QStringList()<<"Title"<<"Size"<<"Type"<<"Downloaded");

	//ResourcesJsonParser parser;
	//parser.parseFileToTreeWidget(courseModel->courseInfo->charptersLocalPath, ui.tw_courseDetails);


	CourseSourcesTree* root = new CourseSourcesTree("root");

	CourseSourcesTree* foo = new CourseSourcesTree("foo", root);
	CourseSourcesTree* child;
	child = new CourseSourcesTree("Mark", foo);
	child = new CourseSourcesTree("Bob", foo);
	child = new CourseSourcesTree("Kent", foo);

	CourseSourcesTree* bar = new CourseSourcesTree("bar", root);

	child = new CourseSourcesTree("Ole", bar);
	child = new CourseSourcesTree("Trond", bar);
	child = new CourseSourcesTree("Kjetil", bar);
	child = new CourseSourcesTree("Lasse", bar);

	CourseSourcesTree* baz = new CourseSourcesTree("baz", root);
	child = new CourseSourcesTree("Bengt", baz);
	child = new CourseSourcesTree("Sven", baz);

	CourseSourcesTree* child2 = new CourseSourcesTree("Sven", child);
	child2 = new CourseSourcesTree("Sven", child);

	CourseResourcesModel* model = new CourseResourcesModel(root);

	ui.tv_courseDetails->setModel(model);
	ui.tv_courseDetails->setColumnWidth(0, 450);

	listSelectionModel = new CourseResourcesSelectionModel(model);
	ui.tv_courseDetails->setSelectionModel(listSelectionModel);

#if 0
	//TreeList tests
	QStandardItemModel* model = new QStandardItemModel(ui.tv_courseDetails);
	model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("项目名")<<QStringLiteral("信息"));
	
	QStandardItem* itemProject = new QStandardItem(QStringLiteral("项目"));
	model->setItem(model->indexFromItem(itemProject).row(), 1, new QStandardItem(QStringLiteral("项目信息说明")));
	model->appendRow(itemProject);
	
	QStandardItem* itemFolder1 = new QStandardItem(QStringLiteral("文件夹1"));
	itemProject->appendRow(itemFolder1);

	QStandardItem* itemFolder2 = new QStandardItem(QStringLiteral("文件夹2"));
	
	itemProject->appendRow(itemFolder2);
	itemProject->setChild(itemFolder2->index().row(), 1, new QStandardItem(QStringLiteral("文件夹2信息说明")));


	for(int i=1; i<=5; ++i){
		QStandardItem* itemgroup = new QStandardItem(QStringLiteral("组%1").arg(i));
		itemFolder2->appendRow(itemgroup);

		for(int j=1; j<=i; ++j){
			QStandardItem* itemchannel = new QStandardItem(QStringLiteral("频道%1").arg(j));
			itemgroup->appendRow(itemchannel);
			itemgroup->setChild(itemchannel->index().row(), 1, new QStandardItem(QStringLiteral("频道%1信息说明").arg(j)));
		}
	}

	ui.tv_courseDetails->setModel(model);
#endif
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
