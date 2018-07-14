#pragma once
#include <QWidget>
#include "ui_course_item_widget.h"
class CourseItemWidget : public QWidget{
public:
	CourseItemWidget(void){
		ui.setupUi(this);
	}
	Ui::CourseItem ui;
};

