#pragma once
#include <qmap.h>
#include "CourseResourcesTree.h"
#include "datatypes.h"
#include "CourseResourcesModel.h"

namespace CourseResourcesManager
{
	void loadResources(QList<CourseModel*> *courseModelList);
	void appendResources(CourseModel* model);
	bool loadSucceed();
	CourseResourcesModel* getListModel(int courseId, int sort);
	CourseResourcesTree* getResourcesTree(int courseId, int sort);
};