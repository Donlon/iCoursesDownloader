
#pragma once

#include <QString>
#include "CourseItemWidget.h"
#include <QListWidgetItem>


class Course{
public:
	Course(int id, QString title, QString teacher, QString school, QString picturePath)
	:id(id), title(title), teacher(teacher), school(school), logoPath(picturePath){
	}
	Course()
	{
		id = -1;
	}
	friend QDataStream &operator<<(QDataStream & dataStream, Course & course)
	{
		dataStream<<course.id;
		dataStream<<course.title;
		dataStream<<course.teacher;
		dataStream<<course.school;
		dataStream<<course.logoPath;
		dataStream<<course.logoLocalPath;
		dataStream<<course.charptersLocalPath;
		dataStream<<course.exercisesLocalPath;
		dataStream<<course.testPapersLocalPath;
		dataStream<<course.otherResLocalPath;
		return dataStream;
	}
	friend QDataStream &operator>>(QDataStream & dataStream, Course & course)
	{
		dataStream>>course.id;
		dataStream>>course.title;
		dataStream>>course.teacher;
		dataStream>>course.school;
		dataStream>>course.logoPath;
		dataStream>>course.logoLocalPath;
		dataStream>>course.charptersLocalPath;
		dataStream>>course.exercisesLocalPath;
		dataStream>>course.testPapersLocalPath;
		dataStream>>course.otherResLocalPath;
		return dataStream;
	}


	int id;
	QString title;
	QString teacher;
	QString school;
	QString logoPath;
 
	QString logoLocalPath;

	QString charptersLocalPath;
	QString exercisesLocalPath;
	QString testPapersLocalPath;
	QString otherResLocalPath;
};

class CourseModel{
public:
	CourseModel(Course *courseInfo):
									courseInfo(courseInfo),
									//(nullptr),
									itemWidget(nullptr){
	}
	Course *courseInfo;

	CourseItemWidget *itemWidget;

	QListWidgetItem *listItem;

	QPixmap *loadedLogo;
};

