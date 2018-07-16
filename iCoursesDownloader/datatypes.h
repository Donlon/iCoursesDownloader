
#pragma once

#include <QString>
#include <QListWidgetItem>
#include "CourseItemWidget.h"


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
		dataStream<<course.resLocalPaths[0];
		dataStream<<course.resLocalPaths[1];
		dataStream<<course.resLocalPaths[2];
		dataStream<<course.resLocalPaths[3];
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
		dataStream>>course.resLocalPaths[0];
		dataStream>>course.resLocalPaths[1];
		dataStream>>course.resLocalPaths[2];
		dataStream>>course.resLocalPaths[3];
		return dataStream;
	}


	int id;
	QString title;
	QString teacher;
	QString school;
	QString logoPath;
 
	QString logoLocalPath;

	QString resLocalPaths[4];

	//QString charptersLocalPath;
	//QString exercisesLocalPath;
	//QString testPapersLocalPath;
	//QString otherResLocalPath;
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

	QPixmap loadedLogo;
};

