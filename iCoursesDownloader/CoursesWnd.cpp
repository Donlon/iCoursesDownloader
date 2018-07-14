#include <QMenu>
#include "CoursesWnd.h"
#include "StorageManager.h"

QList<Course*> coursesListTest;

extern QString STORAGE_PATH;

CoursesWnd::CoursesWnd(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	//ui.lw_allCourses->setContextMenuPolicy(Qt::CustomContextMenu);
	//ui.lw_allCourses->setViewMode(QListView::ListMode);
    //ui.lw_allCourses->setViewMode(QListView::IconMode);

	
    CourseListMenu = new QMenu();
    QAction *lw_action1 = CourseListMenu->addAction("&Open");
    QAction *lw_action2 = CourseListMenu->addAction("&Reload");
    QAction *lw_action3 = CourseListMenu->addAction("&Remove");
    connect(lw_action1, &QAction::triggered, this, &CoursesWnd::btn_open);
    connect(lw_action2, &QAction::triggered, this, &CoursesWnd::menu_reloadItem);
    connect(lw_action3, &QAction::triggered, this, &CoursesWnd::menu_removeItem);


	addCourseDlg = new AddCourseDialog(this);
	courseDetailsWnd = new CourseDetailsWnd();
	connect(courseDetailsWnd, &CourseDetailsWnd::closed, this, &CoursesWnd::onCourseDetailsWndClose);

	QList<Course*> courseList = StorageManager::loadCourseList();
	courseModelList = new QList<CourseModel*>();

	foreach(Course* course, courseList){
		courseModelList->append(buildModelForCourse(course));
	}
	courseList.clear();
	reflushList();

}

CourseModel* CoursesWnd::buildModelForCourse(Course* course){
	qDebug()<<course->title<<endl;
	CourseModel* model = new CourseModel(course);
	model->itemWidget = new CourseItemWidget();
	model->itemWidget->ui.lv_name->setText(course->title);
	//model->itemWidget->ui.lv_desc->setText(course->teacher + " | " + course->school);
    QFontMetrics elidfont(model->itemWidget->ui.lv_desc->font());
    model->itemWidget->ui.lv_desc->setText(
		elidfont.elidedText(course->teacher + " | " + course->school,
			Qt::ElideRight, model->itemWidget->ui.lv_desc->width()));

	model->itemWidget->setToolTip(course->title + " | " + course->school);

	model->loadedLogo = new QPixmap(STORAGE_PATH + model->courseInfo->logoLocalPath);
	model->itemWidget->ui.label->setPixmap(*(model->loadedLogo));
	model->itemWidget->ui.label->setScaledContents(true);

	
	model->listItem = new QListWidgetItem(ui.lw_allCourses);
	model->listItem->setSizeHint(QSize(160,130));

	return model;
}

void CoursesWnd::reflushList()//TODO: sth. wrong?
{
	ui.tv_CoursesCount->setText(QString("%1 items.").arg(courseModelList->size()));
	//ui.lw_allCourses->clear();

	foreach(CourseModel* model, *courseModelList){
		ui.lw_allCourses->addItem(model->listItem);
		ui.lw_allCourses->setItemWidget(model->listItem, model->itemWidget);
	}

}

void CoursesWnd::btn_add(){
	addCourseDlg->exec();

	if(addCourseDlg->execResult == AddCourseDialog::ResultCode::Failed){
		return;
	}

	Course *newCourse = addCourseDlg->newCourse;
	courseModelList->append(buildModelForCourse(newCourse));
	saveList();
	reflushList();
}

void CoursesWnd::btn_open(){
	if(ui.lw_allCourses->selectedItems().empty()){
		return;
	}
	courseDetailsWnd->courseModel = getCurrentSelectedCourse();

	Q_ASSERT(courseDetailsWnd->courseModel != nullptr);

	hide();
	courseDetailsWnd->show();
}

CourseModel *CoursesWnd::getCurrentSelectedCourse(){
	QListWidgetItem *selectedItem = ui.lw_allCourses->selectedItems().at(0);

	foreach(CourseModel *model, *courseModelList){
		if(model->listItem == selectedItem){
			return model;
		}
	}
	return nullptr;
}

void CoursesWnd::onCourseListContextMenu(const QPoint &pos){

	QModelIndex index = ui.lw_allCourses->indexAt(pos);

	if(!index.isValid()) {
		return;
	}
	treeListMenuTriggeredAt = &index;

    CourseListMenu->exec(QCursor::pos());
	//CourseListMenu->actionAt(QCursor::pos());
}

void CoursesWnd::menu_removeItem(bool checked){
	removeItem(getCurrentSelectedCourse());
}

void CoursesWnd::menu_reloadItem(bool checked){
	//TODO
}

void CoursesWnd::removeItem(CourseModel* item){//TODO

	ui.lw_allCourses->removeItemWidget(item->listItem);

	qDebug()<<"Size:"<<courseModelList->size();
	courseModelList->removeOne(item);
	qDebug()<<"Size:"<<courseModelList->size();
	QFile::remove(StorageManager::getLocalFilePath(item->courseInfo->logoLocalPath));
	QFile::remove(StorageManager::getLocalFilePath(item->courseInfo->charptersLocalPath));

	delete item;

	//reflushList();
	saveList();
}

void CoursesWnd::onCourseDetailsWndClose(){
	show();
}
