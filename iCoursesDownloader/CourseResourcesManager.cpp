#include "CourseResourcesManager.h"
#include "ResourcesJsonParser.h"


QMap<int, QList<CourseResourcesTree*>> g_data;
QMap<int, QList<CourseResourcesModel*>> g_models;

void CourseResourcesManager::loadResources(QList<CourseModel*>* courseModelList){
	foreach(CourseModel* modelItem, *courseModelList){
		
		QList<CourseResourcesTree*> treeList;
		QList<CourseResourcesModel*> modelsList;

		for(int i=0; i < 4/*TODO*/; i++){
			ResourcesJsonParser charptersJsonParser;
			charptersJsonParser.parse(modelItem->courseInfo->charptersLocalPath);

			treeList.append(charptersJsonParser.resultTreeNode);
			modelsList.append(new CourseResourcesModel(charptersJsonParser.resultTreeNode));
		}
		
		g_data.insert(modelItem->courseInfo->id, treeList);
		g_models.insert(modelItem->courseInfo->id, modelsList);
	}
}

CourseResourcesModel* CourseResourcesManager::getListModel(int courseId, int sort){
	if(!g_models.contains(courseId)){
		return nullptr;
	}
	return g_models.value(courseId).at(sort);
}

CourseResourcesTree* CourseResourcesManager::getResourcesTree(int courseId, int sort){
	if(!g_data.contains(courseId)){
		return nullptr;
	}
	return g_data.value(courseId).at(sort);
}